#include <Wire.h>
#include "charSet.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define COMMAND_CODE 0b00100000
#define DATA_CODE 0x40
#define BUFFER_LENGTH SCREEN_WIDTH *((SCREEN_HEIGHT) / 8)

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3d  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define CLK_SPEED 3400000

double currentTime, lastTime;

constexpr size_t I2C_BUFFER_SIZE = 1024;

uint8_t PROGMEM *buffer = (uint8_t *)malloc(BUFFER_LENGTH);

byte yOffset = 16;
byte xOffset = 16;
byte incDec = 1;
byte incDec2 = 1;

void setup() {
  int rstPin = -1;
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(CLK_SPEED);
  size_t res = Wire.setBufferSize(I2C_BUFFER_SIZE);
  Serial.println(res);
  startupSequence();
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  sendBuffer();
}

void loop() {
  sendBuffer();
  byte offset = xOffset;
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  for (byte i = 0; i < 8; i++) {
    int numOffset = 0;
    putToBuffer(offset + i, yOffset, numOne[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numTwo[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numThree[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numFour[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numFive[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numSix[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numSeven[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numEight[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numNine[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, yOffset, numZero[i]);
  }
  byte yyy = yOffset;
  if (yyy >= 55 || yyy <= 0) incDec = -incDec;
  if (offset >= (128 - 80) || offset <= 0) incDec2 = -incDec2;
  yOffset += incDec;
  xOffset += incDec2;
}

void singleCommand(byte command) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)COMMAND_CODE);
  Wire.write((uint8_t)command);
  Wire.endTransmission();
}

void commandWithParam(byte command, byte param) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)COMMAND_CODE);
  Wire.write((uint8_t)command);
  Wire.write((uint8_t)param);
  Wire.endTransmission();
}

void dataCommand(uint8_t data) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)DATA_CODE);
  Wire.write((uint8_t)data);
  Wire.endTransmission();
}


void bufferCommand(uint8_t *data, int len) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)DATA_CODE);
  Wire.write(data, len);
  Wire.endTransmission();
}

void startupSequence() {
  singleCommand(0xae);           // Display Off
  commandWithParam(0xA8, 0x3F);  // Set MUX Ratio( set to 63)
  commandWithParam(0xD3, 0x00);  // Set Display Offset
  singleCommand(0x40);           // Set Display Start Line
  singleCommand(0xA0);           // Set Segment re-map A0h/A1h
  singleCommand(0xC0);           // Set COM Output Scan Direction C0h/C8h
  commandWithParam(0xDA, 0x12);  // Set COM Pins hardware configuration(0x02 for 32 lines, 0x12 for 64 lines)
  commandWithParam(0x81, 0x7F);  // Set Contrast Control
  singleCommand(0xa4);           // Disable Entire Display On
  singleCommand(0xa6);           // Set Normal Display
  commandWithParam(0x20, 0x02);  // Set Memory addressing mode to Page addressing mode
  commandWithParam(0xD5, 0x80);  // Set Osc Frequency
  commandWithParam(0x8D, 0x15);  // Enable charge pump regulator
  singleCommand(0xaf);           // Display On
}

void sendBuffer() {
  putPixel(0, 0);
  commandWithParam(0x20, 0x00);
  bufferCommand((uint8_t *)buffer, BUFFER_LENGTH);
  commandWithParam(0x20, 0x02);
}

void setPageAddress(byte p) {
  singleCommand(0xb0 + p);
}

void setLowerColAddress(byte l) {
  singleCommand(0x00 + l);
}

void setHigherColAddress(byte h) {
  singleCommand(0x10 + h);
}

void putPixel(byte x, byte y) {
  setPageAddress(y / 8);
  setLowerColAddress(x & 0x0f);
  setHigherColAddress(x >> 4);
}

void putToBuffer(byte x, byte y, uint8_t data) {
  int div = y / 8;
  int rest = y - (8 * div);
  uint8_t firstLine = data << rest;
  uint8_t secondLine = data >> (8 - rest);
  buffer[x + (128 * ((y / 8)))] = firstLine;
  buffer[x + (128 * ((y / 8) + 1))] = secondLine;
}
