//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <inttypes.h>
#include <Wire.h>

#define MAX_TOUCH 5

struct TouchscreenState {
  uint8_t flag;
  uint8_t count;
 
  uint8_t trackId[MAX_TOUCH];
  uint16_t x[MAX_TOUCH];
  uint16_t y[MAX_TOUCH];
  uint16_t size[MAX_TOUCH];
};

class Touchscreen {
  public:
    Touchscreen(){};

    void setPinsAndI2C(uint8_t resetPin, uint8_t interruptPin, TwoWire* i2cBus);
    void start();
    void stop();
    void reset();

    void getVersion();
    bool touchAvailable();
    bool getState(TouchscreenState* state);

  private:
    uint8_t _resetPin;
    uint8_t _interruptPin;
    TwoWire* _i2cBus;

    void writeRegister(int register, uint8_t* buffer, int numBytes);
    int readRegister(int register, uint8_t* buffer, int numBytes);
    int readData(uint8_t* buffer, int numBytes);
};

#endif // TOUCHSCREEN_H
