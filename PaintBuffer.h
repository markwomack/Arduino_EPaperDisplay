//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef PAINTBUFFER_H
#define PAINTBUFFER_H

#include <inttypes.h>

#include "fonts.h"

enum BufferRotation {
 ROTATE_0,
 ROTATE_90,
 ROTATE_180,
 ROTATE_270,
};

enum Color {
  WHITE = 0,
  BLACK = 1
};

enum TextJustification {
  LEFT_JUST,
  CENTER_JUST,
  RIGHT_JUST
};

class PaintBuffer {
  public:
    PaintBuffer(uint8_t* buffer, uint16_t width, uint16_t height, uint16_t byteWidth);
    void eraseBuffer(Color color = WHITE);
    uint16_t getWidth();
    uint16_t getHeight();
    Color getColor();
    void setColor(Color color);
    void setBufferRotation(BufferRotation bufferRotation);
    
    void drawAbsolutePixel(uint16_t x, uint16_t y);
    void drawAbsolutePixel(uint16_t x, uint16_t y, Color color);
    void drawPixel(uint16_t x, uint16_t y);
    void drawPixel(uint16_t x, uint16_t y, Color color);
    //void drawCharAt(uint16_t x, uint16_t y, char ascii_char, sFONT* font);
    //void drawStringAt(uint16_t x, uint16_t y, const char* text, sFONT* font);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length);
    void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length);
    void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void drawCircle(uint16_t x, uint16_t y, uint16_t radius);
    void drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius);
    void drawBitmap(const uint8_t* image, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool skipUnsetBits = true);
    void drawBitmapFromProgMem(const uint8_t* image, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool skipUnsetBits = true);
    void drawStringAt(uint16_t x, uint16_t y, const char* text, sFONT* font, TextJustification textJustification = LEFT_JUST);
    
  private:
    uint8_t* _buffer;
    uint16_t _width;
    uint16_t _height;
    uint16_t _byteWidth;
    uint16_t _bitWidth;
    Color _color;
    BufferRotation _bufferRotation;
};
#endif // PAINTBUFFER_H
