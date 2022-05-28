#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LCD_Extension.h"

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Start");
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.setCursor(9, 0); // set the cursor to column 3, line 0
  lcd.print("LCD1602");  // Print a message to the LCD
  lcd.setCursor(6, 1); // set the cursor to column 2, line 1
  lcd.print("Animation");  // Print a message to the LCD.
  drawHLine(0, 0, 40);
  drawHLine(0, 7, 40);
  drawVLine(0, 0, 7);
  drawVLine(39, 0, 7);
  hexDump(charmap, 64);
  refreshMap();
  displayMap(0, 0);
  delay(1000);
}

void loop() {
  uint8_t startX = 0;
  for (uint8_t y = 1; y < 7; y++) {
    cls();
    drawHLine(0, 0, 40);
    drawHLine(0, 7, 40);
    drawVLine(0, 0, 7);
    drawVLine(39, 0, 7);
    drawHLine((startX + 5 * y) % 35, y, 5);
    refreshMap();
    displayMap(0, 0);
    delay(500);
  }
  delay(1000);
  cls();
  drawHLine(0, 0, 40);
  drawHLine(0, 7, 40);
  drawVLine(0, 0, 7);
  drawVLine(39, 0, 7);
  for (uint8_t y = 2; y < 7; y += 2) {
    setPixel(2, y);
    refreshMap();
    displayMap(0, 0);
    delay(500);
  }
  for (uint8_t y = 1; y < 6; y += 2) {
    setPixel(4, y);
    refreshMap();
    displayMap(0, 0);
    delay(500);
  }
  for (uint8_t y = 2; y < 7; y += 2) {
    clearPixel(2, y);
    refreshMap();
    displayMap(0, 0);
    delay(500);
  }
  for (uint8_t y = 1; y < 6; y += 2) {
    clearPixel(4, y);
    refreshMap();
    displayMap(0, 0);
    delay(500);
  }

  for (uint8_t y = 2; y < 7; y += 2) {
    for (uint8_t x = 2; x < 39; x += 2) {
      setPixel(x, y);
    }
  }
  refreshMap();
  displayMap(0, 0);
  delay(1000);
  for (uint8_t y = 2; y < 7; y += 2) {
    for (uint8_t x = 2; x < 39; x += 2) {
      clearPixel(x, y);
    }
  }
  refreshMap();
  displayMap(0, 0);
  delay(1000);
}
