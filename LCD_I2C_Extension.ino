#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LCD_Extension.h"

char annyong[40] = {
  // '안녕하세요', 40x8px
  0x22, 0x02, 0x32, 0x06, 0x3c, 0x5a, 0x4e, 0x02,
  0x26, 0x66, 0xcb, 0x4e, 0xfe, 0x26, 0x42, 0xda,
  0x4a, 0x73, 0x7e, 0x66, 0x72, 0x7a, 0x5a, 0x76,
  0x3c, 0x42, 0x3e, 0x7a, 0xde, 0x24, 0x60, 0x62,
  0x02, 0x06, 0xff, 0x7e, 0x3e, 0x02, 0x06, 0x00
}, annyongMap[64], katakana[64] {
  // 'アイウエオ  ', 25x8px
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x48, 0x01,
  0x00, 0x00, 0x08, 0xff, 0xff, 0x80, 0x00, 0x29,
  0xa2, 0x43, 0x00, 0x00, 0x22, 0xa2, 0x45, 0x00,
  0x00, 0x40, 0x84, 0x45, 0x00, 0x00, 0x80, 0x99,
  0xf9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t byteRange[320] = {0};
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
  uint8_t comparators[8] = {
    0b10000000, 0b1000000, 0b100000, 0b10000,
    0b1000, 0b100, 0b10, 0b1
  };
  for (uint8_t i = 0; i < 40; i++) {
    uint8_t byteX = katakana[i];
    for (uint8_t y = 0; y < 8; y++) {
      byteRange[i * 8 + y] = (byteX >> 7 - y) & 1;
    }
  }
  hexDump(byteRange, 320);
  uint8_t ix = 0;
  uint16_t pos = 0;
  for (uint8_t i = 0; i < 64; i += 8) {
    uint8_t iy = ix;
    for (uint8_t j = 0; j < 8; j++) {
      annyongMap[ix] = byteRange[pos] << 4 | byteRange[pos + 1] << 3 | byteRange[pos + 2] << 2 | byteRange[pos + 3] << 1 | byteRange[pos + 4];
      ix += 8;
      pos += 5;
    }
    ix = iy + 1;
  }
  hexDump(annyongMap, 64);
  Serial.println("Good to go!");
}

void loop() {
  cls();
  memcpy(charmap, annyongMap, 64);
  hexDump(charmap, 64);
  refreshMap();
  displayMap(0, 0);
  delay(3500);
  uint8_t startX = 0;
  for (uint8_t y = 1; y < 7; y++) {
    cls();
    drawBox(0, 0, 40, 8);
    drawHLine((startX + 5 * y) % 35, y, 5);
    refreshMap();
    displayMap(0, 0);
    delay(500);
  }
  delay(1000);
  cls();
  drawBox(0, 0, 40, 8);
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
