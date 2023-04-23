//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef PIN_ASSIGNMENTS_H
#define PIN_ASSIGNMENTS_H

// I used a Teensy 4.0 for my testing, and these were
// the pins I used. Change the values to match your
// pin setup.

const uint8_t DC_PIN            (7);
const uint8_t BUSY_PIN          (8);
const uint8_t RESET_PIN         (9);
const uint8_t SPI_CS_PIN       (10);
const uint8_t SPI_MOSI_PIN     (11);
const uint8_t SPI_MISO_PIN     (12);
const uint8_t SPI_SCK_PIN      (13);
const uint8_t LED_STATUS_PIN   (14);
const uint8_t I2C_SDA_PIN      (18);
const uint8_t I2C_SCL_PIN      (19);
const uint8_t TS_RESET_PIN     (20);
const uint8_t TS_INTERRUPT_PIN (21);

#endif // PIN_ASSIGNMENTS_H
