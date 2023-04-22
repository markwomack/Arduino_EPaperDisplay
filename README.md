# Arduino EPaperDisplay Example
## Introduction
Arduino code to drive a Waveshare e-Paper display and example to demonstrate usage.

This code is written specifically for this e-Paper display model from Waveshare:

[2.13" Touch e-Paper HAT for Raspberry Pi, 250×122, Black / White, SPI](https://www.waveshare.com/product/displays/e-paper/epaper-3/2.13inch-touch-e-paper-hat.htm)

While this display is meant for use with a Raspberry Pi, it is still
compatible with Arduino. And in fact, Waveshare has Arduino example code to drive
the display (but not the touch screen) located here:

[epd2in13_V3](https://github.com/waveshare/e-Paper/tree/master/Arduino/epd2in13_V3)

I started with that Waveshare code to create my code. Much of the structure
and code is the same, just cleaned up with some better class organization and some added
features. I don't have any documentation about the SPI commands, so I have just copied
them over as-is. I will add more comments and documentation as I learn more.

## Licenses
All of this code is released under the MIT license except where noted in any files. The
original Waveshare code that was heavily used as a working example for this code had
it's own permissive license, which is noted in the file LICENSE-Waveshare. The fonts
used in this demo come from STMicroelectronics, and were originally included in the
Waveshare demo code. They have their own permissive license included in those files.

## Usage

### EPaperDisplay
EPaperDisplay is the class that defines the features and functions of the display. You will
typically create an instance at the start of the program, configure it for usage, and then
start the display for rendering.

#### Configuration
There are two main methods used to configure the display before usage. Typically you will
call these methods once before calling the start method.

- **setPinsAndSPI** - This method is called to configure the physical pins that the display
is connected to on the microcontroller (Arduino). The display uses the SPI protocol to
receive commands and data. The connections on the physical display are made either to
the solder points on the display or through the 40 pin Raspberry Pi header. The Pi
connections are documented in the picture below.

![](https://github.com/markwomack/Arduino_EPaperDisplay/blob/main/docs/RPiPinsForDisplay.jpg)

- **setDisplayOrientation** - This method sets the orientation of the display, effectively
where the origin of the screen is when rendering. Once the display has been started, the
orientation cannot be changed. The default orientation is ROTATE_0, so if that matches
your usage, then no call is necessary. The picture below shows the origins of each
orientation.

![](https://github.com/markwomack/Arduino_EPaperDisplay/blob/main/docs/2.13InchePaperDisplay.jpg)

#### Using the Display

- **start** - The start method is called to initialize the display for usage. It returns a
PaintBuffer instance, which is used to render pixels, lines, rectangles, circles, fonts,
and bitmaps into the buffer for the display. More information on the
[PaintBuffer class](#paintbuffer) is below.
By default the display is created with an underlying buffer that is the full size of the
screen (FULL_BUFFER). For a black and white image at 122 x 250 pixels this is 4000 bytes.
If that is too much for your available runtime memory, you can pass the value
PARTIAL_BUFFER as a parameter to the start method and a smaller buffer will be created.
However, this has implications for rendering, which is discussed further in the [Demo Code
section](#demo-code) below. Typically you only need to call this method once, at the start
of the program, using a full size buffer.
- **stop** - The stop method is called to stop the display and release all of the memory
associated with it, this includes the underlying buffer and the associated PaintBuffer
object. After calling stop, the PaintBuffer instance returned from the call to the start
method should be discarded (it's already been freed in stop). Typically the stop method
is never called, but if there is a project that requires it, the display can be restarted
once again by another call to the start method.
- **setMode** - The setMode method must be called before the refresh method is called. It
'wakes' the display up from sleep mode and readys the display to receive data.
- **refresh** - The refresh method is used to render the current contents of the underlying
buffer to the display. If using a full size buffer (FULL_BUFFER), refresh only
needs to be called once to render the buffer to the display. If a partial buffer is used,
then 5 calls to refresh are required to render a full size buffer to the display. This is
discussed more in the [Demo Code section](#demo-code) below.
- **sleep** - After updating the display with the image data, you can call the sleep method
to place the display into low power sleep mode. In this mode the display will draw a 
very small amount of current, but the current image on the display will be maintained.
Once in sleep mode, use the setMode method to 'wake up' the display for rendering.
- **reset** - The reset method will perform a hardware reset of the display. This can be
used if the display becomes unresponsive.
- **getWidth** - This method returns the width of the display based on the current display
orientation. If the orientation is ROTATE_0 or ROTATE_180, the width will be 122, and
if the orientation is ROTATE_90 or ROTATE_270, the width will be 250.
- **getHeight** - This method returns the height of the display based on the current display
orientation. If the orientation is ROTATE_0 or ROTATE_180, the height will be 250, and
if the orientation is ROTATE_90 or ROTATE_270, the height will be 122.

#### Memory usage
The underlying buffer and PaintBuffer object are allocated with the call to the start method
and deallocated with the call to stop method. Typical usage has just the call to the start
method, so memory is not constantly allocated and deallocated. As mentioned above, by default
the EPaperDisplay class will allocated a buffer to match the full size of the display, which
is 122 x 250 black or white pixesl. It works out to 4000 bytes for the buffer.

### PaintBuffer
The PaintBuffer class has basic primitives to render points, lines, rectangles, circles, 
strings, and bitmaps. It uses the same underlying buffer created by the EPaperDisplay instance,
so memory is conserved. You get a reference to the paint buffer by calling the start method
of the display.

- **eraseBuffer** - Use eraseBuffer to clear the entire buffer to the given color. By default
this color is WHITE, but you can pass in BLACK as the value to set the entire buffer to black.
- **setColor** - Use the setColor method to set the color that elements will be drawn in. By
default this color is BLACK, but you can set the color to WHITE if drawing on a black
background. You can draw in inverse (BLACK background, WHITE drawings), then use the
eraseBuffer(BLACK) to fill the entire buffer with black, and use setColor(WHITE)
to change the drawing color, and render your desired objects.
- **getColor** - Returns the current color set in the PaintBuffer.
- **drawPixel** - Draws a pixel at the given point, in the current color.
- **drawLine** - Draws a line between two given points, in the current color.
- **drawRectangle** - Draws a rectangle between two given points, in the current color.
- **drawFilledRectangle** - Draws a filled rectangle between to given points, in the current
color.
- **drawCircle** - Draws a circle centered on a given point with the given radious, in the
current color.
- **drawFilledCircle** - Draws a filledcircle centered on a given point with the given radious,
in the current color.
- **drawBitmap** - Draws a bitmap, located in normal memory, of a given width and height with
the top left of the bitmap starting at a given point. Be default only the bits in the image
that are set will be rendered in the current color (skipUnsetBits = true). But if skipUnsetBits
parameter is set to false, then the set bits are rendered in the current color and the unset
bits are rendered in the opposite color.
- **drawBitmapFromProgMem** - Draws a bitmap, located in PROG_MEM, of a given width and height
with the top left of the bitmap starting at a given point. Be default only the bits in the image
that are set will be rendered in the current color (skipUnsetBits = true). But if skipUnsetBits
parameter is set to false, then the set bits are rendered in the current color and the unset
bits are rendered in the opposite color.
- **drawStringAt** - Draws a given string, using one of the built-in fonts located in PROG_MEM,
justified around a given point. If LEFT_JUSTIFIED, the default, the top/left of the string
will start at the given point. If CENTER_JUSTIFIED, the string will render to the left of the
point with the point being at the center/top of the rendered string. If RIGHT_JUSTIFIED, the
top/right of the string will end at the given point.
- **getWidth** - This method returns the width of the display based on the current display
orientation and buffer type. If the buffer type is FULL_BUFFER and the orientation is
ROTATE_0 or ROTATE_180, the width will be 122, and if the orientation is ROTATE_90 or
ROTATE_270, the width will be 250. If the buffer type is PARTIAL_BUFFER, then the buffer
is smaller, only 1/5th the size. If the orientation is ROTATE_0 or ROTATE_180, the width will
be 122, and if the orientation is ROTATE_90 or ROTATE_270, the width will be 50.
- **getHeight**- This method returns the height of the display based on the current display
orientation and buffer type. If the buffer type is FULL_BUFFER and the orientation is
ROTATE_0 or ROTATE_180, the height will be 250, and if the orientation is ROTATE_90 or
ROTATE_270, the height will be 122. If the buffer type is PARTIAL_BUFFER, then the buffer
is smaller, only 1/5th the size. If the orientation is ROTATE_0 or ROTATE_180, the width will
be 50, and if the orientation is ROTATE_90 or ROTATE_270, the width will be 122.
- **useOffsetStep** - If using a PARTIAL_BUFFER, and you still want the drawing to be rendered
as if there is a full buffer, then set this value to true (the default is false). You will need
to draw the full image multiple times, and this is discussed further in the
[Demo Code section](#demo-code) below.
- **setOffsetStep** - Do not use this method. It is called by the parent EParentDisplay to
coordinate with the PaintBuffer when refreshing with a partial buffer.

#### Image Basics
If you are creating your own images to be displayed, please keep in mind some of these basic
facts. A single image byte represents 8 pixels, one bit for each pixel, with the most significant
bit being the leftmost pixel. As an example, an image that is 24 pixels wide and 24 pixels in
height, each row of the image will be 3 bytes wide, and there will be 24 rows. The total number
of bytes will be 3 x 24 = 72. But what if the width of the image is not a multiple of 8? If we
have an image that is 21 pixels wide and 21 pixels in height, how many bytes should each row be?
2 bytes is not enough bits (16 bits) and 3 bytes is more than is needed (24 bits).

In cases where the pixel width is not a multiple of 8, we round up the number of bytes. In this
case: 3 bytes. The extra bits are just ignored. But they are still accounted for in the memory
allocated for the image. An image 24 x 24 pixels and an image that is 21 x 24 pixels require the
same amount of memory, 72 bytes.

The formula to use when determining the 'byte' width of an image is:

```
if (pixelWidth % 8 == 0) { 
  byteWidth = pixelWidth / 8;       // pixel width is a multiple of 8
} else {
  byteWidth = (pixelWidth / 8) + 1; // pixel width is not a multiple of 8, add a byte for padding
}
```

## Demo Code
The demo code in the Arduino_EPaperDisplay.ino sketch demonstrates usage for almost all of the
methods outlined.

### fullBufferDemo
This method renders a number of screens that are shown in the display when
the image buffer matches the full size of the display. It demonstrates use of all the shape,
string, and bitmap methods, using different colors and different display orientations. Normal
usage will probably not require multiple calls to the start and stop methods, but changing
orientation in the demo requires it.

### partialBufferDemo
This method demonstrates the usage of the EPaperDisplay and PaintBuffer
objects when only a partial buffer is allocated. When PARTIAL_BUFFER is given as a parameter to
the start method, a buffer that is only 1/5th the size of the full display is allocated. The
size of the buffer in pixels is only 122 x 50. Using a partial buffer changes the behavior of the
refresh method. Where the display was updated with just one call to refresh, the display will not
update until there have been 5 calls to refresh, each call updating a segment of the display. All
of the updates won't be displayed until the fifth call.

The first two examples show how this works when the display orientation is the default ROTATE_0.
Each segment is rendered independent of each other, working progressively down the 250 pixel height
of the display. This might be useful, but not likely. The situation gets even worse when the
display orientation is changed, as the third example shows. The text is rotation and displayed
in each segment. The issue is that drawing in the PaintBuffer is being done in the buffer coordinates,
not the display coordinates.

But in order for the display coordinates to make sense across all 5 buffer segments, you will need
to call the useOffsetStep method. This configures the PaintBuffer to work with the parent
EPaperDisplay object to count the number of refresh calls and to offset the coordinates accordingly.
But in order for this to work you will need to draw the same buffer contents each of the 5 calls to
refresh. It is a lot of duplication, but it works perfectly. All of the following examples demonstrate
this by drawing the same buffer 5 times using display coordinates instead of buffer coordinates. This
technique works with all display orientations and all of the paint buffer drawing methods. Pixels that
would be rendered outside of the mapped buffer are ignored so that memory is not corrupted.

## Questions, Concerns, Bugs
If you have any questions or encounter bugs, [please report a new issue](https://github.com/markwomack/Arduino_EPaperDisplay/issues).
Please provide as much detail and sample code that you can.
