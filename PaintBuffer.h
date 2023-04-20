//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef PAINTBUFFER_H
#define PAINTBUFFER_H

#include <inttypes.h>

#include "display_constants.h"


class PaintBuffer {
  public:
    PaintBuffer(uint8_t* buffer, uint16_t width, uint16_t height, uint16_t byteWidth, DisplayRotation displayRotation);
    void eraseBuffer(Color color = WHITE);
    uint16_t getWidth();
    uint16_t getHeight();
    Color getColor();
    void setColor(Color color);
    void setDisplayRotation(DisplayRotation displayRotation);
    
    void drawAbsolutePixel(uint16_t x, uint16_t y);
    void drawPixel(uint16_t x, uint16_t y);
    //void drawCharAt(uint16_t x, uint16_t y, char ascii_char, sFONT* font);
    //void drawStringAt(uint16_t x, uint16_t y, const char* text, sFONT* font);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length);
    void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length);
    void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void drawCircle(uint16_t x, uint16_t y, uint16_t radius);
    void drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius);
    void drawFullImage(const uint8_t* image);
    
  private:
    uint8_t* _buffer;
    uint16_t _width;
    uint16_t _height;
    uint16_t _byteWidth;
    uint16_t _bitWidth;
    Color _color;
    DisplayRotation _displayRotation;
    bool _invertColor;
};
#endif // PAINTBUFFER_H
