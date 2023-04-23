#include <Arduino.h>
#line 1 "/Users/bartsky/Documents/Arduino/ssd1306_skinny_lib/ssd1306_skinny_lib.ino"
#include "charSet.h"
#include "ssd1306-esp32-lib.hpp"

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define CLK_SPEED 3400000

double currentTime, lastTime;

constexpr size_t I2C_BUFFER_SIZE = 1024;

uint8_t PROGMEM *buffer = (uint8_t *)malloc(BUFFER_LENGTH);

byte yOffset = 16;
byte xOffset = 16;
byte incDec = 1;
byte incDec2 = 1;

#line 18 "/Users/bartsky/Documents/Arduino/ssd1306_skinny_lib/ssd1306_skinny_lib.ino"
void setup();
#line 30 "/Users/bartsky/Documents/Arduino/ssd1306_skinny_lib/ssd1306_skinny_lib.ino"
void loop();
#line 18 "/Users/bartsky/Documents/Arduino/ssd1306_skinny_lib/ssd1306_skinny_lib.ino"
void setup() {
  int rstPin = -1;
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(CLK_SPEED);
  size_t res = Wire.setBufferSize(I2C_BUFFER_SIZE);
  Serial.println(res);
  startupSequence();
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  sendBuffer(buffer);
}

void loop() {
  sendBuffer(buffer);
  byte offset = xOffset;
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  for (byte i = 0; i < 8; i++) {
    int numOffset = 0;
    putToBuffer(offset + i, yOffset, numOne[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numTwo[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numThree[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numFour[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numFive[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numSix[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numSeven[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numEight[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numNine[i], buffer);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numZero[i], buffer);
  }
  byte yyy = yOffset;
  if (yyy >= 55 || yyy <= 0) incDec = -incDec;
  if (offset >= (128 - 80) || offset <= 0) incDec2 = -incDec2;
  yOffset += incDec;
  xOffset += incDec2;
}

