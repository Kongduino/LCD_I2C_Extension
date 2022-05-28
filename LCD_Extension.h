void displayMap(uint8_t, uint8_t);
void displayMap(uint8_t, uint8_t, uint8_t, uint8_t);

void hexDump(uint8_t* buf, uint16_t len) {
  // Something similar to the Unix/Linux hexdump -C command
  // Pretty-prints the contents of a buffer, 16 bytes a row
  // I use to debug :-) Like a super printf.
  char alphabet[17] = "0123456789abcdef";
  uint16_t i, index;
  Serial.print(F("   +------------------------------------------------+ +----------------+\n"));
  Serial.print(F("   |.0 .1 .2 .3 .4 .5 .6 .7 .8 .9 .a .b .c .d .e .f | |      ASCII     |\n"));
  for (i = 0; i < len; i += 16) {
    if (i % 128 == 0) Serial.print(F("   +------------------------------------------------+ +----------------+\n"));
    char s[] = "|                                                | |                |\n";
    // pre-formated line. We will replace the spaces with text when appropriate.
    uint8_t ix = 1, iy = 52, j;
    for (j = 0; j < 16; j++) {
      if (i + j < len) {
        uint8_t c = buf[i + j];
        // fastest way to convert a byte to its 2-digit hex equivalent
        s[ix++] = alphabet[(c >> 4) & 0x0F];
        s[ix++] = alphabet[c & 0x0F];
        ix++;
        if (c > 31 && c < 127) s[iy++] = c;
        else s[iy++] = '.'; // display ASCII code 0x20-0x7F or a dot.
      }
    }
    index = i / 16;
    // display line number then the text
    if (i < 256) Serial.write(' ');
    Serial.print(index, HEX); Serial.write('.');
    Serial.print(s);
  }
  Serial.print(F("   +------------------------------------------------+ +----------------+\n"));
}


LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
uint8_t charmap[64] = { 0};
#define printByte(args) write(args);

void refreshMap() {
  Wire.beginTransmission(0x27);
  for (uint8_t i = 0; i < 8; i++) {
    lcd.createChar(i, charmap + (i * 8));
  }
}
void displayMap(uint8_t x, uint8_t y) {
  displayMap(x, y, 0, 7);
}
void displayMap(uint8_t x, uint8_t y, uint8_t startx, uint8_t endx) {
  if (x > 15) return;
  if (y > 1) return;
  if (endx < startx) {
    uint8_t temp = startx;
    startx = endx;
    endx = temp;
  }
  if (startx > 7) return;
  if (endx > 7) return;
  lcd.setCursor(x, y);
  Wire.beginTransmission(0x27);
  for (uint8_t i = startx; i <= endx; i++) {
    lcd.printByte(i);
  }
  Wire.endTransmission();
}

void cls(uint8_t x, uint8_t y) {
  memset(charmap, 0, 64);
  displayMap(x, y, 0, 7);
}

void cls() {
  memset(charmap, 0, 64);
}

void drawVLine(uint8_t x, uint8_t y, uint8_t h) {
  if (x > 39) return;
  if (y > 6) return;
  if (y + h > 7) return;
  uint8_t xByte = x / 5;
  uint8_t bitsets[5] = { 0b10000, 0b01000, 0b00100, 0b00010, 0b00001 };
  uint8_t bitset = bitsets[x % 5];
  for (uint8_t j = y; j < y + h; j++) {
    uint8_t px = j + 8 * xByte;
    charmap[px] |= bitset;
  }
}

void drawHLine(uint8_t x, uint8_t y, uint8_t w) {
  if (x > 38) return;
  if (y > 7) return;
  uint8_t lastPixel = x + w - 1;
  if (lastPixel > 39) lastPixel = 39;
  uint8_t mw = (lastPixel / 5);
  // Serial.print("mw = "); Serial.println(mw);
  // if (mw * 5 < lastPixel) mw += 1;
  // Serial.print("; mw = ");Serial.println(mw);
  for (uint8_t i = x / 5; i <= mw; i++) {
    uint8_t pix = charmap[i * 8 + y];
    uint8_t startP = i * 5;
    uint8_t endP = startP + 4;
    if (startP >= x && endP <= lastPixel) {
      pix = 0x1f;
    }
    else {
      if (endP > lastPixel) endP = lastPixel;
      uint8_t bitset = 0b10000;
      while (startP < x) {
        startP += 1;
        bitset = bitset >> 1;
      }
      while (startP <= endP) {
        pix |= bitset;
        startP += 1;
        bitset = bitset >> 1;
      }
    }
    charmap[i * 8 + y] = pix;
  }
}

void setPixel(uint8_t x, uint8_t y) {
  if (x > 39) return;
  if (y > 7) return;
  uint8_t xByte = x / 5;
  uint8_t bitsets[5] = { 0b10000, 0b01000, 0b00100, 0b00010, 0b00001 };
  uint8_t bitset = bitsets[x % 5];
  uint8_t px = y + 8 * xByte;
  charmap[px] |= bitset;
}

void clearPixel(uint8_t x, uint8_t y) {
  if (x > 39) return;
  if (y > 7) return;
  uint8_t xByte = x / 5;
  uint8_t bitsets[5] = { 0b01111, 0b10111, 0b11011, 0b11101, 0b11110 };
  uint8_t bitset = bitsets[x % 5];
  uint8_t px = y + 8 * xByte;
  charmap[px] &= bitset;
}

void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  drawHLine(x, y, w);
  drawHLine(x, y + h - 1, w);
  drawVLine(x, y, h - 1);
  drawVLine(x + w - 1, y, h - 1);
}
