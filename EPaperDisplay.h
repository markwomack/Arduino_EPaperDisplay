//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef EPAPERDISPLAY_H
#define EPAPERDISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <inttypes.h>

#include "display_constants.h"
#include "PaintBuffer.h"

class EPaperDisplay {
  public:
    EPaperDisplay();

    void start(uint8_t resetPin, uint8_t busyPin, uint8_t dcPin, uint8_t csPin, SPIClass* spi);
    void refresh();
    void clear();
    void reset();
    void sleep();
    void setDisplayMode(DisplayMode mode);
    void setDisplayRotation(DisplayRotation displayRotation);
    void setWindow(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd);
    void setCursor(uint8_t xStart, uint8_t yStart);
    PaintBuffer* getPaintBuffer();
    
  private:
    uint16_t _displayWidth;
    uint16_t _displayHeight;
    uint16_t _byteWidth;
    DisplayRotation _displayRotation;
    uint8_t* _imageBuffer;
    PaintBuffer* _paintBuffer;
   
    uint8_t _resetPin;
    uint8_t _busyPin;
    uint8_t _dcPin;
    uint8_t _csPin;
    SPIClass* _spi;

    void sendLutData(const uint8_t* lutData);
    void sendCommand(uint8_t command);
    void sendData(uint8_t data);
    void waitUntilIdle();
    void spiTransfer(uint8_t data);
};
#endif // EPAPERDISPLAY_H
