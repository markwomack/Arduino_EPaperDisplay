//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#include <DebugMsgs.h>

#include "Touchscreen.h"

const int TOUCHSCREEN_ADDRESS(0x14);
const uint16_t REGISTER_VERSION(0x8140);
const uint16_t REGISTER_NEWSTATE(0x814E);
const uint16_t REGISTER_TOUCHINFO(0x814F);

bool gTouchInterruptFlag;
uint8_t gInterruptPin;

void touchInterruptCallback() {
  gTouchInterruptFlag = digitalRead(gInterruptPin);
  //DebugMsgs.debug().print("In touchInterrupt: ").println(gTouchInterruptFlag);
}

union TouchscreenStateWire {
  uint8_t bytes[sizeof(TouchscreenState)];
  TouchscreenState touchscreenState;
};

void Touchscreen::setPinsAndI2C(uint8_t resetPin, uint8_t interruptPin, TwoWire* i2cBus) {
  _resetPin = resetPin;
  _interruptPin = interruptPin;
  _i2cBus = i2cBus;

  pinMode(_resetPin, OUTPUT); 
  pinMode(_interruptPin, INPUT);
}

void Touchscreen::start() {
  reset();
  
  // attach to the interrupt pin
  gTouchInterruptFlag = false;
  gInterruptPin = _interruptPin;
  attachInterrupt(_interruptPin, &touchInterruptCallback, CHANGE);
}

void Touchscreen::stop() {
  // detach from the interrupt pin
  detachInterrupt(_interruptPin);
}

void Touchscreen::reset() {
  digitalWrite(_resetPin, HIGH);
  delay(100);
  digitalWrite(_resetPin, LOW);
  delay(100);
  digitalWrite(_resetPin, HIGH);
  delay(100);
}

void Touchscreen::getVersion() {
  uint8_t version[4];
  readRegister(REGISTER_VERSION, &version[0], 4);

  DebugMsgs.debug().print("Touchscreen Version: ")
    .print(version[0]).print('.')
    .print(version[1]).print('.')
    .print(version[2]).print('.')
    .println(version[3]);
}

bool touchAvailable() {
  return gTouchInterruptFlag;
}

bool Touchscreen::getState(TouchscreenState* state) {
//  if (!gTouchInterruptFlag) {
//    return false;
//  }
  
  uint8_t buffer[100];
  uint8_t mask[1] = { 0x00 };
  TouchscreenState returnState;

  //DebugMsgs.debug().println("1");
  readRegister(REGISTER_NEWSTATE, buffer, 1);
  //DebugMsgs.debug().println("2");
  if ((buffer[0] & 0x80) == 0x00) {
  //DebugMsgs.debug().println("3");
    // no new state, end interaction
    writeRegister(REGISTER_NEWSTATE, mask, 1);
    return false;
  } else {
    DebugMsgs.debug().println("4");
    returnState.flag = buffer[0] & 0x80;
    returnState.count = buffer[0] & 0x0F;
    if (returnState.count > 5 || returnState.count < 1) {
      // wrong number of  touch counts
      writeRegister(REGISTER_NEWSTATE, mask, 1);
      return false;
    }
    
    readRegister(REGISTER_TOUCHINFO, &buffer[1], returnState.count * 8);
    writeRegister(REGISTER_NEWSTATE, mask, 1);

    for (uint8_t i = 0; i < returnState.count; i++) {
      returnState.trackId[i] = buffer[1 + 8*i];
      returnState.x[i] = ((uint16_t)buffer[3 + 8*i] << 8) + buffer[2 + 8*i];
      returnState.y[i] = ((uint16_t)buffer[5 + 8*i] << 8) + buffer[4 + 8*i];
      returnState.size[i] = ((uint16_t)buffer[7 + 8*i] << 8) + buffer[6 + 8*i];
    }
  }

  *state = returnState;
  return true;
}

void Touchscreen::writeRegister(int register, uint8_t* buffer, int numBytes) {
  _i2cBus->beginTransmission(TOUCHSCREEN_ADDRESS);
  _i2cBus->write(REGISTER_VERSION >> 8);     // High byte
  _i2cBus->write(REGISTER_VERSION & 0x00FF); // Low byte
  for (int x = 0; x < numBytes; x++) {
    _i2cBus->write(buffer[x]);
  }
  int result = _i2cBus->endTransmission();

  if (result != 0) {
    DebugMsgs.debug().print("writeRegister endTransmission error: ").println(result);
  }
}

int Touchscreen::readRegister(int register, uint8_t* buffer, int numBytes) {
  _i2cBus->beginTransmission(TOUCHSCREEN_ADDRESS);
  _i2cBus->write(REGISTER_VERSION >> 8);     // High byte
  _i2cBus->write(REGISTER_VERSION & 0x00FF); // Low byte
  int result = _i2cBus->endTransmission();

  if (result != 0) {
    DebugMsgs.debug().print("readRegister endTransmission error: ").println(result);
    return 0;
  }
  
  return readData(buffer, numBytes);
}

int Touchscreen::readData(uint8_t* buffer, int numBytes) {
  _i2cBus->requestFrom(TOUCHSCREEN_ADDRESS, numBytes);

  int count = 0;
  while(_i2cBus->available()) {
    buffer[count++] = _i2cBus->read();
  }

  DebugMsgs.debug().println(count);
  
  return count;
}
