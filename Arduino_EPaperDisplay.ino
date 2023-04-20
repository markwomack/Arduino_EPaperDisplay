//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

#include <DebugMsgs.h>

#include "pin_assignments.h"
#include "EPaperDisplay.h"
#include "PaintBuffer.h"

#include "image_data.h"

EPaperDisplay display;

void setup() {
  Serial.begin(9600);

  DebugMsgs.enableLevel(DEBUG);

  // Make sure the right pins are set for SPI
  SPI.setMOSI(SPI_MOSI0_PIN);
  SPI.setMISO(SPI_MISO0_PIN);
  SPI.setSCK(SPI_SCK0_PIN);

  DebugMsgs.debug().println("Starting display");
  display.start(RESET_PIN, BUSY_PIN, DC_PIN, SPI_CS0_PIN, &SPI);
  DebugMsgs.debug().println("Setting FULL mode");
  display.setDisplayMode(FULL);
  display.setDisplayRotation(ROTATE_90);

  PaintBuffer* paintBuffer = display.getPaintBuffer();
  DebugMsgs.debug().println("Erasing buffer with all WHITE");
  paintBuffer->eraseBuffer(WHITE);
  DebugMsgs.debug().println("Drawing lines");
  paintBuffer->drawLine(0,0,249,121);
  paintBuffer->drawLine(0,10,249,10);
  paintBuffer->drawLine(0,20,249,20);
  paintBuffer->drawLine(0,30,249,30);
  paintBuffer->drawLine(10,0,10,121);
  paintBuffer->drawLine(20,0,20,121);
  paintBuffer->drawLine(30,0,30,121);
  DebugMsgs.debug().println("Drawing rectangles");
  paintBuffer->drawRectangle(50, 50, 100, 90);
  paintBuffer->drawFilledRectangle(100, 100, 200, 121);
  DebugMsgs.debug().println("Drawing circle");
  paintBuffer->drawCircle(150, 50, 15);
  DebugMsgs.debug().println("Drawing filled circle");
  paintBuffer->drawFilledCircle(200, 75, 25);
  DebugMsgs.debug().println("refreshing display");
  display.refresh();

  delay(3000);

  DebugMsgs.debug().println("Drawing full image");
  paintBuffer->drawFullImage(IMAGE_DATA);
  DebugMsgs.debug().println("refreshing display");
  display.refresh();

  DebugMsgs.debug().println("putting display to sleep");
  display.sleep();
}

void loop() {
  // put your main code here, to run repeatedly:

}
