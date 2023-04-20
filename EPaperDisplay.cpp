//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

#include <DebugMsgs.h>

#include "EPaperDisplay.h"

const uint8_t lut_full_update[]= {
  0x80, 0x4A, 0x40, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
  0x40, 0x4A, 0x80, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
  0x80, 0x4A, 0x40, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
  0x40, 0x4A, 0x80, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
  0xF,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0xF,  0x0,  0x0,  0xF,  0x0,  0x0,  0x2,          
  0xF,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x1,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,          
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x0,  0x0,  0x0,      
  0x22, 0x17, 0x41, 0x0,  0x32, 0x36  
};

const uint8_t lut_partial_update[]= { 
  0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x14,0x0,0x0,0x0,0x0,0x0,0x0,  
  0x1,0x0,0x0,0x0,0x0,0x0,0x0,
  0x1,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
  0x22,0x17,0x41,0x00,0x32,0x36,
};

// Display resolution in pixels
const uint16_t DISPLAY_WIDTH(122);
const uint16_t DISPLAY_HEIGHT(250);

EPaperDisplay::EPaperDisplay() {
  _byteWidth = (DISPLAY_WIDTH % 8 == 0)? (DISPLAY_WIDTH / 8 ) : (DISPLAY_WIDTH / 8 + 1);
  _displayWidth = DISPLAY_WIDTH;
  _displayHeight = DISPLAY_HEIGHT;
  _imageBuffer = new uint8_t[_byteWidth * _displayHeight];
  _paintBuffer = new PaintBuffer(_imageBuffer, _displayWidth, _displayHeight, _byteWidth, _displayRotation);
}

void EPaperDisplay::start(uint8_t resetPin, uint8_t busyPin, uint8_t dcPin, uint8_t csPin, SPIClass* spi) {
  _resetPin = resetPin;
  _busyPin = busyPin;
  _dcPin = dcPin;
  _csPin = csPin;
  _spi = spi;
  
  pinMode(_resetPin, OUTPUT);
  pinMode(_dcPin, OUTPUT);
  pinMode(_csPin, OUTPUT); 
  pinMode(_busyPin, INPUT);
  
  _spi->begin();
  _spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

  DebugMsgs.print("_displayWidth: ").print(_displayWidth)
    .print(", _displayHeight: ").print(_displayHeight)
    .print(", _byteWidth: ").print(_byteWidth)
    .print(", num image bytes: ").println(_byteWidth * _displayHeight);
}

void EPaperDisplay::setDisplayMode(DisplayMode mode) {
    
  reset();
  
  switch(mode) {
    case FULL: {
      waitUntilIdle();
      sendCommand(0x12); // soft reset
      waitUntilIdle();
  
      sendCommand(0x01); //Driver output control
      sendData(0xF9);
      sendData(0x00);
      sendData(0x00);
  
      sendCommand(0x11); //data entry mode
      sendData(0x03);
  
      setWindow(0, 0, _displayWidth - 1, _displayHeight - 1);
      setCursor(0, 0);
    
      sendCommand(0x3C); //BorderWavefrom
      sendData(0x05); 
    
      sendCommand(0x21); //  Display update control
      sendData(0x00);
      sendData(0x80); 
    
      sendCommand(0x18); //Read built-in temperature sensor
      sendData(0x80); 
    
      waitUntilIdle();
      sendLutData(lut_full_update);
    }
    break;
    case PART: {

      digitalWrite(_resetPin, LOW);                //module reset
      delay(1);
      digitalWrite(_resetPin, HIGH);
  
      sendLutData(lut_partial_update);
    
      sendCommand(0x37);
      sendData(0x00);
      sendData(0x00);
      sendData(0x00);
      sendData(0x00);
      sendData(0x00);
      sendData(0x40);
      sendData(0x00);
      sendData(0x00);
      sendData(0x00);
      sendData(0x00);
  
      sendCommand(0x3C); //Border Wave form
      sendData(0x80); 
  
      sendCommand(0x22);  // Display Update Sequence Option
      sendData(0xC0);     // Enable clock and  Enable analog
      sendCommand(0x20);  // Activate Display Update Sequence
      waitUntilIdle();  
      
      setWindow(0, 0, _displayWidth - 1, _displayHeight - 1);
      setCursor(0, 0);
    }
  }
}

void EPaperDisplay::setDisplayRotation(DisplayRotation displayRotation) {
  _displayRotation = displayRotation;
  if (_paintBuffer != 0) {
    _paintBuffer->setDisplayRotation(displayRotation);
  }
}

void EPaperDisplay::setWindow(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd)
{
  sendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
  sendData((xStart >> 3) & 0xFF);
  sendData((xEnd >> 3) & 0xFF);

  sendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
  sendData(yStart & 0xFF);
  sendData((yStart >> 8) & 0xFF);
  sendData(yEnd & 0xFF);
  sendData((yEnd >> 8) & 0xFF);
}

void EPaperDisplay::setCursor(uint8_t xStart, uint8_t yStart) {
  sendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
  sendData(xStart & 0xFF);

  sendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
  sendData(yStart & 0xFF);
  sendData((yStart >> 8) & 0xFF);
}

PaintBuffer* EPaperDisplay::getPaintBuffer() {
  return _paintBuffer;
}

void EPaperDisplay::refresh() {

  sendCommand(0x24);
  
//  for(int i = 0; i < _byteWidth * _displayHeight; i++){
//    sendData(_imageBuffer[i]);
//  }
  for (int j = 0; j < _displayHeight; j++) {
    for (int i = 0; i < _byteWidth; i++) {
      sendData(_imageBuffer[i + j * _byteWidth]);
    }
  }

  // refresh display
  sendCommand(0x22);
  sendData(0xC7);
  sendCommand(0x20);
  waitUntilIdle();
}

void EPaperDisplay::clear() {
  sendCommand(0x24);
  for (int j = 0; j < _displayHeight; j++) {
    for (int i = 0; i < _byteWidth; i++) {
      sendData(0xFF);
    }
  }

  // Refresh display
  sendCommand(0x22);
  sendData(0xC7);
  sendCommand(0x20);
  waitUntilIdle();
}

void EPaperDisplay::reset() {
  digitalWrite(_resetPin, HIGH);
  delay(20);
  digitalWrite(_resetPin, LOW);                //module reset
  delay(2);
  digitalWrite(_resetPin, HIGH);
  delay(20);
}

void EPaperDisplay::sleep() {
  sendCommand(0x10); //enter deep sleep
  sendData(0x01);
  delay(200);
  digitalWrite(_resetPin, LOW);
}

void EPaperDisplay::sendLutData(const uint8_t* lutData)
{
  sendCommand(0x32);
  for(uint8_t count = 0; count < 153; count++) {
    sendData(lutData[count]);
  }

  sendCommand(0x3f);
  sendData(*(lutData+153));
  sendCommand(0x03);  // gate voltage
  sendData(*(lutData+154));
  sendCommand(0x04);  // source voltage
  sendData(*(lutData+155)); // VSH
  sendData(*(lutData+156)); // VSH2
  sendData(*(lutData+157)); // VSL
  sendCommand(0x2c);    // VCOM
  sendData(*(lutData+158));
}

void EPaperDisplay::sendCommand(uint8_t command) {
  digitalWrite(_dcPin, LOW);
  spiTransfer(command);
}

void EPaperDisplay::sendData(uint8_t data) {
  digitalWrite(_dcPin, HIGH);
  spiTransfer(data);
}

void EPaperDisplay::waitUntilIdle(void) {
  while(1) {      //LOW: idle, HIGH: busy
    if (digitalRead(_busyPin) == LOW) {
      break;
    }
    delay(10);
  }
}

void EPaperDisplay::spiTransfer(uint8_t data) {
  digitalWrite(_csPin, LOW);
  _spi->transfer(data);
  digitalWrite(_csPin, HIGH);
}
