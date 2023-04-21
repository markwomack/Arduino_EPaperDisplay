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
and bitmaps into the buffer for the display. More information on that class is below.
By default the display is created with an underlying buffer that is the full size of the
screen (FULL_BUFFER). For a black and white image at 122 x 250 pixels this is 4000 bytes.
If that is too much for your available runtime memory, you can pass the value
PARTIAL_BUFFER as a parameter to the start method and a smaller buffer will be created.
However, this has implications for rendering, which is discussed further in the Demo Code
section below. Typically you only need to call this method once, at the start of the program,
using a full size buffer.

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
discussed more in the Demo Code section below.

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
2.13" device.
![](https://github.com/markwomack/Arduino_EPaperDisplay/blob/main/docs/RPiPinsForDisplay.jpg)

I have used these pins in my own usage of the device.
