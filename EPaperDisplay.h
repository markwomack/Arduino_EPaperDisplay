//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef EPAPERDISPLAY_H
#define EPAPERDISPLAY_H

#include <Arduino.h>
#include <inttypes.h>
#include <SPI.h>

#include "PaintBuffer.h"

enum BufferType {
  FULL_BUFFER,
  PARTIAL_BUFFER
};

enum Mode {
  FULL,
  PART
};

class EPaperDisplay {
  public:
    EPaperDisplay();

    // Set these before calling start(). Default display rotation is ROTATE_0.
    void setPinsAndSPI(uint8_t resetPin, uint8_t busyPin, uint8_t dcPin, uint8_t csPin, SPIClass* spi);
    void setDisplayOrientation(DisplayOrientation DisplayOrientation);

    // Start the display. Default is with a full size buffer, but can also
    // be with a PARTIAL_BUFFER instead.
    PaintBuffer* start(BufferType bufferType = FULL_BUFFER);

    // Stops the display. Can't be used until start() is
    // called again.
    void stop();

    // Refresh the display with the current buffer.
    void refresh();

    // Clears the display screen, but not the buffer.
    void clear();

    // Places the display into low power sleep mode.
    void sleep();

    // Resets the display.
    void reset();

    // Must be called before refreshing the device if
    // it has been placed in sleep mode. Not sure what
    // different mode makes yet.
    void setMode(Mode mode);

    // Not sure what these do yet.
    void setWindow(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd);
    void setCursor(uint8_t xStart, uint8_t yStart);

    // Returns the display width. Changing DisplayOrientation
    // affects this value.
    uint16_t getDisplayWidth();

    // Returns the display height. Changing DisplayOrientation
    // affects this value.
    uint16_t getDisplayHeight();
    
  private:
    uint16_t _displayWidth;
    uint16_t _displayHeight;
    DisplayOrientation _displayOrientation;
    BufferType _bufferType;
    uint8_t _partialRefreshCount;
    uint16_t _bufferByteWidth;
    uint16_t _bufferWidth;
    uint16_t _bufferHeight;
    uint8_t* _imageBuffer;
    PaintBuffer* _paintBuffer;
   
    uint8_t _resetPin;
    uint8_t _busyPin;
    uint8_t _dcPin;
    uint8_t _csPin;
    SPIClass* _spi;

    // Low level service methods.
    void sendLutData(const uint8_t* lutData);
    void sendCommand(uint8_t command);
    void sendData(uint8_t data);
    void waitUntilIdle();
    void spiTransfer(uint8_t data);
};
#endif // EPAPERDISPLAY_H
