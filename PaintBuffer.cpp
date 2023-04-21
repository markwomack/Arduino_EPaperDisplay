//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

#include "PaintBuffer.h"

PaintBuffer::PaintBuffer(uint8_t* buffer, uint16_t displayWidth, uint16_t displayHeight,
    uint16_t byteWidth, uint8_t numOffsetSteps, DisplayRotation displayRotation) {
  _buffer = buffer;
  _width = displayWidth;
  _height = displayHeight;
  _byteWidth = byteWidth;
  _bitWidth = byteWidth * 8;
  _color = BLACK;
  _displayRotation = displayRotation;
  _useOffsetStep = false;
  _offsetStep = 0;
  _numOffsetSteps = numOffsetSteps;
}

void PaintBuffer::eraseBuffer(Color color) {
  uint8_t fillColor = color == WHITE ? 0xFF : 0x00;
  for (int i = 0; i < _byteWidth * _height; i++) {
    _buffer[i] = fillColor;
  }
}

uint16_t PaintBuffer::getWidth() {
  switch(_displayRotation) {
    case ROTATE_90:
    case ROTATE_270: {
      return _height;
    }
    
    default: { // ROTATE_0, ROTATE_180
      return _width;
    }
  }
}

uint16_t PaintBuffer::getHeight() {
  switch(_displayRotation) {
    case ROTATE_90:
    case ROTATE_270: {
      return _width;
    }
    
    default: { // ROTATE_0, ROTATE_180
      return _height;
    }
  }
}

Color PaintBuffer::getColor() {
  return _color;
}

void PaintBuffer::setColor(Color color) {
  _color = color;
}

void PaintBuffer::useOffsetStep(bool useOffsetStep) {
  _useOffsetStep = useOffsetStep;
}

void PaintBuffer::setOffsetStep(uint8_t step) {
  _offsetStep = step;
}

void PaintBuffer::drawAbsolutePixel(uint16_t x, uint16_t y) {
  drawAbsolutePixel(x, y, _color);
}

void PaintBuffer::drawAbsolutePixel(uint16_t x, uint16_t y, Color color) {
  if (x < 0 || x >= _width || y < 0 || y >= _height) {
    return;
  }
  
  if (color == WHITE) {
    _buffer[(x + y * _bitWidth) / 8] |= 0x80 >> (x % 8);
  } else {
    _buffer[(x + y * _bitWidth) / 8] &= ~(0x80 >> (x % 8));
  }
}

void PaintBuffer::drawPixel(uint16_t x, uint16_t y) {
  drawPixel(x, y, _color);
}

void PaintBuffer::drawPixel(uint16_t x, uint16_t y, Color color) {
  int point_temp;
  switch (_displayRotation) {
    case ROTATE_0: {
      if (_useOffsetStep) {
        y = y - (getHeight() * _offsetStep);
      }
      if(x < 0 || x >= _width || y < 0 || y >= _height) {
        return;
      }
      
      drawAbsolutePixel(x, y, color);
    }
    break;

    case ROTATE_90: {
      if (_useOffsetStep) {
        x = x - (getWidth() * _offsetStep);
      }
      if(x < 0 || x >= _height || y < 0 || y >= _width) {
        return;
      }
      
      point_temp = x;
      x = _width - y - 1;
      y = point_temp;
      drawAbsolutePixel(x, y, color);
    }
    break;

    case ROTATE_180: {
      if (_useOffsetStep) {
        y = y - (getHeight() * (_numOffsetSteps - _offsetStep - 1));
      }
      if(x < 0 || x >= _width || y < 0 || y >= _height) {
        return;
      }
      x = _width - x - 1;
      y = _height - y - 1;
      drawAbsolutePixel(x, y, color);
    }
    break;

    case ROTATE_270: {
      if (_useOffsetStep) {
        x = x - (getWidth() * (_numOffsetSteps - _offsetStep - 1));
      }
      if(x < 0 || x >= _height || y < 0 || y >= _width) {
        return;
      }
      point_temp = x;
      x = y;
      y = _height - point_temp - 1;
      drawAbsolutePixel(x, y, color);
    }
    break;
  }
}

void PaintBuffer::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  // Check for easy cases
  if (y0 == y1) {
    drawHorizontalLine(x0 < x1 ? x0 : x1, y0, abs(x1 - x0));
    return;
  } else if (x0 == x1) {
    drawVerticalLine(x0, y0 < y1 ? y0 : y1, abs(y1 - y0));
    return;
  }
  
  /* Bresenham algorithm */
  int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
  int sx = x0 < x1 ? 1 : -1;
  int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;

  while((x0 != x1) && (y0 != y1)) {
    drawPixel(x0, y0);
    if (2 * err >= dy) {     
      err += dy;
      x0 += sx;
    }
    if (2 * err <= dx) {
      err += dx; 
      y0 += sy;
    }
  }
}

void PaintBuffer::drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length) {
  int i;
  for (i = x; i < x + length; i++) {
    drawPixel(i, y);
  }
}

void PaintBuffer::drawVerticalLine(uint16_t x, uint16_t y, uint16_t length) {
  int i;
  for (i = y; i < y + length; i++) {
    drawPixel(x, i);
  }
}

void PaintBuffer::drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  int min_x, min_y, max_x, max_y;
  min_x = x1 > x0 ? x0 : x1;
  max_x = x1 > x0 ? x1 : x0;
  min_y = y1 > y0 ? y0 : y1;
  max_y = y1 > y0 ? y1 : y0;
  
  drawHorizontalLine(min_x, min_y, max_x - min_x + 1);
  drawHorizontalLine(min_x, max_y, max_x - min_x + 1);
  drawVerticalLine(min_x, min_y, max_y - min_y + 1);
  drawVerticalLine(max_x, min_y, max_y - min_y + 1);
}

void PaintBuffer::drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  int min_x, min_y, max_x, max_y;
  int i;
  min_x = x1 > x0 ? x0 : x1;
  max_x = x1 > x0 ? x1 : x0;
  min_y = y1 > y0 ? y0 : y1;
  max_y = y1 > y0 ? y1 : y0;
  
  for (i = min_x; i <= max_x; i++) {
    drawVerticalLine(i, min_y, max_y - min_y + 1);
  }
}

void PaintBuffer::drawCircle(uint16_t x, uint16_t y, uint16_t radius) {
  /* Bresenham algorithm */
  int x_pos = -radius;
  int y_pos = 0;
  int err = 2 - 2 * radius;
  int e2;

  do {
    drawPixel(x - x_pos, y + y_pos);
    drawPixel(x + x_pos, y + y_pos);
    drawPixel(x + x_pos, y - y_pos);
    drawPixel(x - x_pos, y - y_pos);
    e2 = err;
    if (e2 <= y_pos) {
      err += ++y_pos * 2 + 1;
      if (-x_pos == y_pos && e2 <= x_pos) {
        e2 = 0;
      }
    }
    if (e2 > x_pos) {
      err += ++x_pos * 2 + 1;
    }
  } while (x_pos <= 0);
}

void PaintBuffer::drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius) {
  /* Bresenham algorithm */
  int x_pos = -radius;
  int y_pos = 0;
  int err = 2 - 2 * radius;
  int e2;

  do {
    drawPixel(x - x_pos, y + y_pos);
    drawPixel(x + x_pos, y + y_pos);
    drawPixel(x + x_pos, y - y_pos);
    drawPixel(x - x_pos, y - y_pos);
    drawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1);
    drawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1);
    e2 = err;
    if (e2 <= y_pos) {
      err += ++y_pos * 2 + 1;
      if (-x_pos == y_pos && e2 <= x_pos) {
        e2 = 0;
      }
    }
    if (e2 > x_pos) {
      err += ++x_pos * 2 + 1;
    }
  } while(x_pos <= 0);
}

void PaintBuffer::drawBitmap(const uint8_t* image, uint16_t x, uint16_t y,
    uint16_t width, uint16_t height, bool skipUnsetBits) {

  int i, j;

  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      uint8_t bitVal = *image & (0x80 >> (i % 8));
      if (bitVal == 0 && !skipUnsetBits) {
        drawPixel(x + i, y + j, _color == WHITE ? BLACK : WHITE);
      } else if (bitVal != 0) {
        drawPixel(x + i, y + j);
      }
      if (i % 8 == 7) {
        image++;
      }
    }
    if (width % 8 != 0) {
      image++;
    }
  }
}

void PaintBuffer::drawBitmapFromProgMem(const uint8_t* image, uint16_t x, uint16_t y,
    uint16_t width, uint16_t height, bool skipUnsetBits) {

  int i, j;
  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      uint8_t bitVal = pgm_read_byte(image) & (0x80 >> (i % 8));
      if (bitVal == 0 && !skipUnsetBits) {
        drawPixel(x + i, y + j, _color == WHITE ? BLACK : WHITE);
      } else if (bitVal != 0) {
        drawPixel(x + i, y + j);
      }
      if (i % 8 == 7) {
        image++;
      }
    }
    if (width % 8 != 0) {
      image++;
    }
  }
}

void PaintBuffer::drawStringAt(uint16_t x, uint16_t y, const char* text, sFONT* font,
    TextJustification textJustification) {
  const char* p_text = text;
  int refColumn;

  switch(textJustification) {
    case CENTER_JUST: {
      refColumn = x - ((strlen(text) * font->width) / 2);
    }
    break;

    case RIGHT_JUST: {
      refColumn = x - (strlen(text) * font->width);
    }
    break;

    default: { // LEFT_JUST
      refColumn = x;
    }
    break;
  }
  
  while (*p_text != 0) {
    uint32_t char_offset = (*p_text - ' ') * font->height * (font->width / 8 + (font->width % 8 ? 1 : 0));
    const uint8_t* charPtr = &font->table[char_offset];
    
    /* Display one character on EPD */
    drawBitmap(charPtr, refColumn, y, font->width, font->height);

    /* Decrement the column position by 16 */
    refColumn += font->width;
    
    /* Point on the next character */
    p_text++;
  }
}
