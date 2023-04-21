//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

#include <DebugMsgs.h>

#include "pin_assignments.h"
#include "EPaperDisplay.h"
#include "PaintBuffer.h"

#include "src/image_data.h"
                                 
EPaperDisplay display;

void setup() {
  Serial.begin(9600);

  DebugMsgs.enableLevel(DEBUG);

  // Make sure the right pins are set for SPI
  SPI.setMOSI(SPI_MOSI0_PIN);
  SPI.setMISO(SPI_MISO0_PIN);
  SPI.setSCK(SPI_SCK0_PIN);

  display.start(RESET_PIN, BUSY_PIN, DC_PIN, SPI_CS0_PIN, &SPI);
  display.setMode(FULL);

  PaintBuffer* paintBuffer = display.getPaintBuffer();
  
  paintBuffer->setBufferRotation(ROTATE_90);
  paintBuffer->eraseBuffer(WHITE);
  paintBuffer->drawLine(0,10,249,10);
  paintBuffer->drawLine(0,20,249,20);
  paintBuffer->drawLine(0,30,249,30);
  paintBuffer->drawLine(10,0,10,121);
  paintBuffer->drawLine(20,0,20,121);
  paintBuffer->drawLine(30,0,30,121);
  paintBuffer->drawRectangle(50, 50, 100, 90);
  paintBuffer->drawFilledRectangle(100, 100, 200, 121);
  paintBuffer->drawCircle(150, 50, 15);
  paintBuffer->drawFilledCircle(200, 75, 25);
  display.refresh();
  display.sleep();
  delay(2000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);
  
  paintBuffer->setBufferRotation(ROTATE_90);
  paintBuffer->eraseBuffer(BLACK);
  paintBuffer->setColor(WHITE);
  paintBuffer->drawLine(0,10,249,10);
  paintBuffer->drawLine(0,20,249,20);
  paintBuffer->drawLine(0,30,249,30);
  paintBuffer->drawLine(10,0,10,121);
  paintBuffer->drawLine(20,0,20,121);
  paintBuffer->drawLine(30,0,30,121);
  paintBuffer->drawRectangle(50, 50, 100, 90);
  paintBuffer->drawFilledRectangle(100, 100, 200, 121);
  paintBuffer->drawCircle(150, 50, 15);
  paintBuffer->drawFilledCircle(200, 75, 25);
  display.refresh();
  display.sleep();
  delay(2000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  paintBuffer->setBufferRotation(ROTATE_0);
  paintBuffer->eraseBuffer();
  paintBuffer->setColor(BLACK);
  paintBuffer->drawBitmap(sample_image2, 0, 0, 21, 21);
  paintBuffer->drawBitmap(sample_image2, 21, 21, 21, 21);
  paintBuffer->drawBitmap(sample_image1, 122 - 16, 0, 16, 16);
  paintBuffer->drawBitmap(sample_image1, 122 - 32, 16, 16, 16);
  display.refresh();
  display.sleep();
  delay(1000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);
  
  paintBuffer->setBufferRotation(ROTATE_90);
  paintBuffer->eraseBuffer();
  paintBuffer->drawBitmap(sample_image2, 0, 0, 21, 21);
  paintBuffer->drawBitmap(sample_image2, 21, 21, 21, 21);
  paintBuffer->drawBitmap(sample_image1, 122 - 16, 0, 16, 16);
  paintBuffer->drawBitmap(sample_image1, 122 - 32, 16, 16, 16);
  display.refresh();
  display.sleep();
  delay(1000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  paintBuffer->setBufferRotation(ROTATE_180);
  paintBuffer->eraseBuffer();
  paintBuffer->drawBitmap(sample_image2, 0, 0, 21, 21);
  paintBuffer->drawBitmap(sample_image2, 21, 21, 21, 21);
  paintBuffer->drawBitmap(sample_image1, 122 - 16, 0, 16, 16);
  paintBuffer->drawBitmap(sample_image1, 122 - 32, 16, 16, 16);
  display.refresh();
  display.sleep();
  delay(1000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  paintBuffer->setBufferRotation(ROTATE_270);
  paintBuffer->eraseBuffer();
  paintBuffer->drawBitmap(sample_image2, 0, 0, 21, 21);
  paintBuffer->drawBitmap(sample_image2, 21, 21, 21, 21);
  paintBuffer->drawBitmap(sample_image1, 122 - 16, 0, 16, 16);
  paintBuffer->drawBitmap(sample_image1, 122 - 32, 16, 16, 16);
  display.refresh();
  display.sleep();
  delay(1000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  paintBuffer->setBufferRotation(ROTATE_90);
  paintBuffer->eraseBuffer();
  paintBuffer->drawStringAt(20, 2, "e-Paper Demo", &Font16, LEFT_JUST);
  paintBuffer->drawStringAt(124, 5 + Font16.height, "e-Paper Demo", &Font16, CENTER_JUST);
  paintBuffer->drawStringAt(230, 8 + (2 * Font16.height), "e-Paper Demo", &Font16, RIGHT_JUST);
  display.refresh();
  display.sleep();
  delay(2000);

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  paintBuffer->setBufferRotation(ROTATE_0);
  paintBuffer->eraseBuffer();
  paintBuffer->drawBitmapFromProgMem(sample_image3, 0, 0, 121, 249);
  display.refresh();
  display.sleep();

  // Wake up! Let's draw some more!
  display.setMode(FULL);

  paintBuffer->setBufferRotation(ROTATE_0);
  paintBuffer->eraseBuffer(BLACK);
  paintBuffer->setColor(WHITE);
  paintBuffer->drawBitmapFromProgMem(sample_image3, 0, 0, 121, 249);
  display.refresh();
  display.sleep();
}

void loop() {
  // These aren't the droids we are looking for, move along.
}
