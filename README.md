# Arduino EPaperDisplay Example
## Introduction
Arduino code to drive a Waveshare e-Paper display and example to demonstrate usage.

This code is written specifically for this e-Paper display model from Waveshare:

[2.13" Touch e-Paper HAT for Raspberry Pi, 250×122, Black / White, SPI](https://www.waveshare.com/product/displays/e-paper/epaper-3/2.13inch-touch-e-paper-hat.htm)

While this display is very oriented for usage with a Raspberry Pi, it is still
compatible with Arduino. And in fact, Waveshare has Arduino example code to drive
the display (but not the touch screen) here:

[epd2in13_V3](https://github.com/waveshare/e-Paper/tree/master/Arduino/epd2in13_V3)

That Waveshare code is what I started with to create this version. Much of the structure
and code is the same, just cleaned up with some better class organization. I don't have
any documentation about the SPI commands, so I have just copied them over as-is. I will
add more comments and documentation as I learn more.

## Classes

### EPaperDisplay
This is the main class that interfaces with the e-Paper device. See the comments in
the EPaperDisplay.h file for an overview of the methods. The Arduino_EPaperDisplay sketch
demonstrates basic usage, but a short overview:

- The EPaperDisplay object is created. Right now it is hardcoded to 122 x 250 pixels provided
by the 2.13" display.
- The start() method is called to setup the pins and SPI bus.
- The setDisplayMode() is called to set the display for FULL mode. I don't exactly know what
this is doing or why one would use PART mode yet.
- The setDisplayOrientation() is called to 'rotate' the origin of the display. The picture
below shows the origin for each orientation. ROTATE_90 means the display will be in 'landscape'
mode (The default orientation is ROTATE_0).
![](https://github.com/markwomack/Arduino_EPaperDisplay/blob/main/2.13InchePaperDisplay.jpg)
- Then a PaintBuffer object is retrieved from the EPaperDisplay. This PaintBuffer is what is
used to render pixels into the display. You perform the drawing operations on the PaintBuffer
instance. When finished drawing, call the refresh() method on the ePaperDisplay to display
the current buffer.

That's basically it. Use the PaintBuffer to render into the buffer, call EPaperDisplay refresh()
to display the buffer. In between you can call sleep() so that the display uses much less power.

#### Memory usage
The current code allocates a full image buffer for the entire display. At 122 x 250 black or
white pixels this amounts to 4000 bytes.

### PaintBuffer
The PaintBuffer class has basic methods to render lines, rectangles, circles, and bitmaps. It
uses the same image buffer created by the EPaperDisplay instance, so memory is conserved.

#### Color
Elements are drawn in the current color (BLACK or WHITE) that is set on the PaintBuffer. The
default is BLACK. If you want to draw in inverse (BLACK background, WHITE drawings), then
use the eraseBuffer(BLACK) to fill the entire buffer with BLACK, use setColor(WHITE) to change
the drawing color, and render your desired objects.

## Pin Connections
This picture shows the pin connections using the RPi 40 pin header that is included on the
2.13" device. I have used these pins in my own usage of the device.
![](https://github.com/markwomack/Arduino_EPaperDisplay/blob/main/RPiPinsForDisplay.jpg)
