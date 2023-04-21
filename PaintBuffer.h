//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef PAINTBUFFER_H
#define PAINTBUFFER_H

#include <inttypes.h>

#include "src/fonts.h"

// Effectively sets the origin of
// the display. ROTATE_0 is default.
enum DisplayOrientation {
 ROTATE_0,
 ROTATE_90,
 ROTATE_180,
 ROTATE_270,
};

// Colors supported by the display.
enum Color {
  WHITE = 0,
  BLACK = 1
};

// Text justification supported.
enum TextJustification {
  LEFT_JUST,
  CENTER_JUST,
  RIGHT_JUST
};

class PaintBuffer {
  public:
    PaintBuffer(uint8_t* buffer, uint16_t width, uint16_t height, uint16_t byteWidth, uint8_t numOffsetStteps, DisplayOrientation displayOrientation);

    // Erases the entire content of the buffer with the given color.
    void eraseBuffer(Color color = WHITE);

    // Returns the width of the buffer. Display rotation
    // affects this value.
    uint16_t getWidth();

    // Returns the height of the buffer. Display rotation
    // affects this value.
    uint16_t getHeight();

    // Returns the current color.
    Color getColor();

    // Sets the current color.
    void setColor(Color color);

    // Used only with partial paint buffers if drawing
    // should be in display coordinates and not buffer
    // coordinates.
    void useOffsetStep(bool useOffsetStep);

    // Called by the display to synchronize rendering
    // during refreshes. You should never need to call
    // it.
    void setOffsetStep(uint8_t step);

    // Draws a single pixel at the given location in the
    // current color.
    void drawPixel(uint16_t x, uint16_t y);
    
    // Draws a line between the given points in the
    // current color.
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    
    // Draws a rectangle between the given coordinates in the
    // current color.
    void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    
    // Draws a filled rectangle between the given coordinates in the
    // current color.
    void drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    // Draws a circle centered at the given point with the given
    // radius in the current color.
    void drawCircle(uint16_t x, uint16_t y, uint16_t radius);

    // Draws a filled circle centered at the given point with the given
    // radius in the current color.
    void drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius);

    // Draws a bitmap stored in regular RAM starting at the given
    // point. Set bits are rendered in the current color, unset bits
    // in the opposite color. When skipUnsetBits is true, then unset
    // bits are not rendered to the buffer, effectively treating
    // the set bits as an image mask.
    void drawBitmap(const uint8_t* image, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool skipUnsetBits = true);

    // Draws a bitmap stored in regular PROGMEM starting at the given
    // point. Set bits are rendered in the current color, unset bits
    // in the opposite color. When skipUnsetBits is true, then unset
    // bits are not rendered to the buffer, effectively treating
    // the set bits as an image mask.
    void drawBitmapFromProgMem(const uint8_t* image, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool skipUnsetBits = true);

    // Draws a string starting at the given point, using the given font, and justified,
    // in the current color.
    void drawStringAt(uint16_t x, uint16_t y, const char* text, sFONT* font, TextJustification textJustification = LEFT_JUST);
    
  private:
    void drawAbsolutePixel(uint16_t x, uint16_t y);
    void drawAbsolutePixel(uint16_t x, uint16_t y, Color color);
    void drawPixel(uint16_t x, uint16_t y, Color color);
    void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length);
    void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length);

    uint8_t* _buffer;
    uint16_t _width;
    uint16_t _height;
    uint16_t _byteWidth;
    uint16_t _bitWidth;
    Color _color;
    DisplayOrientation _displayOrientation;
    bool _useOffsetStep;
    uint16_t _numOffsetSteps;
    uint16_t _offsetStep;
};
#endif // PAINTBUFFER_H
