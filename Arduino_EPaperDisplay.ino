//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

#include "pin_assignments.h"
#include "EPaperDisplay.h"
#include "PaintBuffer.h"

#include "src/image_data.h"

// This method demonstrates the use of the display with a full
// buffer that matches the size of the display. This is the
// typical usage unless you have very limited memory available.
void fullBufferDemo() {
  EPaperDisplay display;

  display.setPinsAndSPI(RESET_PIN, BUSY_PIN, DC_PIN, SPI_CS0_PIN, &SPI);
  display.setDisplayRotation(ROTATE_90);
  PaintBuffer* paintBuffer = display.start();
  display.setMode(FULL);
  
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawLine(0,10,paintBuffer->getWidth()-1,10);
  paintBuffer->drawLine(0,20,paintBuffer->getWidth()-1,20);
  paintBuffer->drawLine(0,30,paintBuffer->getWidth()-1,30);
  paintBuffer->drawLine(10,0,10,paintBuffer->getHeight()-1);
  paintBuffer->drawLine(20,0,20,paintBuffer->getHeight()-1);
  paintBuffer->drawLine(30,0,30,paintBuffer->getHeight()-1);
  paintBuffer->drawRectangle(50, 50, 100, 90);
  paintBuffer->drawFilledRectangle(100, 100, 200, paintBuffer->getHeight()-1);
  paintBuffer->drawCircle(150, 50, 15);
  paintBuffer->drawFilledCircle(200, 75, 25);
  display.refresh();
  display.sleep();
  delay(2000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);
  
  paintBuffer->eraseBuffer(BLACK);
  paintBuffer->setColor(WHITE);
  paintBuffer->drawLine(0,10,paintBuffer->getWidth()-1,10);
  paintBuffer->drawLine(0,20,paintBuffer->getWidth()-1,20);
  paintBuffer->drawLine(0,30,paintBuffer->getWidth()-1,30);
  paintBuffer->drawLine(10,0,10,paintBuffer->getHeight()-1);
  paintBuffer->drawLine(20,0,20,paintBuffer->getHeight()-1);
  paintBuffer->drawLine(30,0,30,paintBuffer->getHeight()-1);
  paintBuffer->drawRectangle(50, 50, 100, 90);
  paintBuffer->drawFilledRectangle(100, 100, 200, paintBuffer->getHeight()-1);
  paintBuffer->drawCircle(150, 50, 15);
  paintBuffer->drawFilledCircle(200, 75, 25);
  display.refresh();
  display.sleep();
  delay(2000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  // Display rotation can't be changed once the display is started
  // so, stop the display, set rotation, restart.
  // This is typically not something you would need to do, you
  // would set a rotation and use it forever after. But the
  // demo is showing different usages.
  display.stop();
  display.setDisplayRotation(ROTATE_0);
  paintBuffer = display.start();
  
  paintBuffer->eraseBuffer();
  paintBuffer->setColor(BLACK);
  paintBuffer->drawBitmap(sample_image2, 0, 0, 21, 21);
  paintBuffer->drawBitmap(sample_image2, 21, 21, 21, 21);
  paintBuffer->drawBitmap(sample_image1, paintBuffer->getWidth() - 16, 0, 16, 16);
  paintBuffer->drawBitmap(sample_image1, paintBuffer->getWidth() - 32, 16, 16, 16);
  display.refresh();
  display.sleep();
  delay(1000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);
  
  display.stop();
  display.setDisplayRotation(ROTATE_90);
  paintBuffer = display.start();
  paintBuffer->eraseBuffer();
  paintBuffer->drawBitmap(sample_image2, 0, 0, 21, 21);
  paintBuffer->drawBitmap(sample_image2, 21, 21, 21, 21);
  paintBuffer->drawBitmap(sample_image1, paintBuffer->getWidth() - 16, 0, 16, 16);
  paintBuffer->drawBitmap(sample_image1, paintBuffer->getWidth() - 32, 16, 16, 16);
  display.refresh();
  display.sleep();
  delay(1000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  display.stop();
  display.setDisplayRotation(ROTATE_180);
  paintBuffer = display.start();
  paintBuffer->eraseBuffer();
  paintBuffer->drawBitmap(sample_image2, 0, 0, 21, 21);
  paintBuffer->drawBitmap(sample_image2, 21, 21, 21, 21);
  paintBuffer->drawBitmap(sample_image1, paintBuffer->getWidth() - 16, 0, 16, 16);
  paintBuffer->drawBitmap(sample_image1, paintBuffer->getWidth() - 32, 16, 16, 16);
  display.refresh();
  display.sleep();
  delay(1000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  display.stop();
  display.setDisplayRotation(ROTATE_270);
  paintBuffer = display.start();
  paintBuffer->eraseBuffer();
  paintBuffer->drawBitmap(sample_image2, 0, 0, 21, 21);
  paintBuffer->drawBitmap(sample_image2, 21, 21, 21, 21);
  paintBuffer->drawBitmap(sample_image1, paintBuffer->getWidth() - 16, 0, 16, 16);
  paintBuffer->drawBitmap(sample_image1, paintBuffer->getWidth() - 32, 16, 16, 16);
  display.refresh();
  display.sleep();
  delay(1000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  display.stop();
  display.setDisplayRotation(ROTATE_90);
  paintBuffer = display.start();
  paintBuffer->eraseBuffer();
  paintBuffer->drawStringAt(20, 2, "e-Paper Demo", &Font16, LEFT_JUST);
  paintBuffer->drawStringAt(124, 5 + Font16.height, "e-Paper Demo", &Font16, CENTER_JUST);
  paintBuffer->drawStringAt(230, 8 + (2 * Font16.height), "e-Paper Demo", &Font16, RIGHT_JUST);
  display.refresh();
  display.sleep();
  delay(2000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  display.stop();
  display.setDisplayRotation(ROTATE_0);
  paintBuffer = display.start();
  paintBuffer->eraseBuffer();
  paintBuffer->drawBitmapFromProgMem(sample_image3, 0, 0, 121, 249);
  display.refresh();
  display.sleep();

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  paintBuffer->eraseBuffer(BLACK);
  paintBuffer->setColor(WHITE);
  paintBuffer->drawBitmapFromProgMem(sample_image3, 0, 0, 121, 249);
  display.refresh();
  display.sleep();
  display.stop();
}

// This method demonstrates the usage of the display with
// only a partial buffer. Only use this if your available
// memory is small.
void partialBufferDemo() {

  EPaperDisplay display;

  display.setPinsAndSPI(RESET_PIN, BUSY_PIN, DC_PIN, SPI_CS0_PIN, &SPI);
  display.setDisplayRotation(ROTATE_0);
  PaintBuffer* paintBuffer = display.start(PARTIAL_BUFFER);
  display.setMode(FULL);

  paintBuffer->eraseBuffer();
  paintBuffer->drawRectangle(10,10,paintBuffer->getWidth() - 11, paintBuffer->getHeight() - 11);
  // It now takes 5 refresh calls to refresh the entire display.
  // refresh() is smart enough to perform the display refresh on the fifth
  // call. You can have different buffer contents each time. This just uses
  // the same buffer over and over, so you will see 5 versions down the length
  // of the display.
  display.refresh();
  display.refresh();
  display.refresh();
  display.refresh();
  display.refresh();
  display.sleep();
  delay(2000);
  
  // Wake up! Let's draw some more!
  display.setMode(FULL);

  // This will demonstrate it even more clearly
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh 1", &Font12, CENTER_JUST);
  display.refresh();
  
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh 2", &Font12, CENTER_JUST);
  display.refresh();
  
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh 3", &Font12, CENTER_JUST);
  display.refresh();
  
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh 4", &Font12, CENTER_JUST);
  display.refresh();
  
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh 5", &Font12, CENTER_JUST);
  display.refresh();
  display.sleep();
  delay(2000);
  
  // Wake up! Let's draw some more!
  display.setMode(FULL);

  // If you rotate the buffer while using a partial
  // buffer, then it doesn't work like you might expect
  display.stop();
  display.setDisplayRotation(ROTATE_90);
  paintBuffer = display.start(PARTIAL_BUFFER);
  display.setMode(FULL);

  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh Rotated 90", &Font12, CENTER_JUST);
  display.refresh();
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh Rotated 90", &Font12, CENTER_JUST);
  display.refresh();
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh Rotated 90", &Font12, CENTER_JUST);
  display.refresh();
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh Rotated 90", &Font12, CENTER_JUST);
  display.refresh();
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawStringAt(
    paintBuffer->getWidth()/2,
    paintBuffer->getHeight()/2 - Font12.height/2,
    "Buffer Refresh Rotated 90", &Font12, CENTER_JUST);
  display.refresh();
  display.sleep();
  delay(2000);
  
  // Wake up! Let's draw some more!
  display.setMode(FULL);

  // You have to do a lot of duplication,
  // but using the offsetStep, the display and
  // paint buffer coordinate to make sure the
  // pixels end up in the right places across
  // all of the buffer segments. Just make sure
  // you draw everything in display coordinates
  // instead of the buffer coordinates.
  display.stop();
  display.setDisplayRotation(ROTATE_0);
  paintBuffer = display.start(PARTIAL_BUFFER);
  display.setMode(FULL);
  paintBuffer->useOffsetStep(true);
  for (int x = 0; x < 5; x++) {
    paintBuffer->eraseBuffer(WHITE);
    paintBuffer->drawRectangle(10,10,display.getDisplayWidth()-11,display.getDisplayHeight()-11);
    paintBuffer->drawStringAt(
      display.getDisplayWidth()/2,
      display.getDisplayHeight()/2 - Font12.height/2,
      "Buffer Rotated 0", &Font12, CENTER_JUST);
    display.refresh();
  }
  display.sleep();
  delay(2000);
  
  display.stop();
  display.setDisplayRotation(ROTATE_90);
  paintBuffer = display.start(PARTIAL_BUFFER);
  display.setMode(FULL);
  paintBuffer->useOffsetStep(true);
  for (int x = 0; x < 5; x++) {
    paintBuffer->eraseBuffer(WHITE);
    paintBuffer->drawRectangle(10,10,display.getDisplayWidth()-11,display.getDisplayHeight()-11);
    paintBuffer->drawStringAt(
      display.getDisplayWidth()/2,
      display.getDisplayHeight()/2 - Font12.height/2,
      "Buffer Rotated 90", &Font12, CENTER_JUST);
    display.refresh();
  }
  display.sleep();
  delay(2000);
  
  display.stop();
  display.setDisplayRotation(ROTATE_180);
  paintBuffer = display.start(PARTIAL_BUFFER);
  display.setMode(FULL);
  paintBuffer->useOffsetStep(true);
  for (int x = 0; x < 5; x++) {
    paintBuffer->eraseBuffer(WHITE);
    paintBuffer->drawRectangle(10,10,display.getDisplayWidth()-11,display.getDisplayHeight()-11);
    paintBuffer->drawStringAt(
      display.getDisplayWidth()/2,
      display.getDisplayHeight()/2 - Font12.height/2,
      "Buffer Rotated 180", &Font12, CENTER_JUST);
    display.refresh();
  }
  display.sleep();
  delay(2000);
  
  display.stop();
  display.setDisplayRotation(ROTATE_270);
  paintBuffer = display.start(PARTIAL_BUFFER);
  display.setMode(FULL);
  paintBuffer->useOffsetStep(true);
  for (int x = 0; x < 5; x++) {
    paintBuffer->eraseBuffer(WHITE);
    paintBuffer->drawRectangle(10,10,display.getDisplayWidth()-11,display.getDisplayHeight()-11);
    paintBuffer->drawStringAt(
      display.getDisplayWidth()/2,
      display.getDisplayHeight()/2 - Font12.height/2,
      "Buffer Rotated 270", &Font12, CENTER_JUST);
    display.refresh();
  }
  display.sleep();
  delay(2000);

  // Even full size bitmaps can be rendered across the
  // buffer refreshes to get the whole image.
  display.stop();
  display.setDisplayRotation(ROTATE_0);
  paintBuffer = display.start(PARTIAL_BUFFER);
  display.setMode(FULL);
  paintBuffer->useOffsetStep(true);
  for (int x = 0; x < 5; x++) {
    paintBuffer->eraseBuffer();
    paintBuffer->drawBitmapFromProgMem(sample_image3, 0, 0, 121, 249);
    display.refresh();
  }
  display.sleep();
  
  display.stop();
}

void setup() {
  Serial.begin(9600);

  fullBufferDemo();
  
  delay(5000);
  
  partialBufferDemo();
}

void loop() {
  // These aren't the droids we are looking for, move along.
}
