#include "ssd1306-esp32-lib.hpp"
void singleCommand(uint8_t command) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)COMMAND_CODE);
  Wire.write((uint8_t)command);
  Wire.endTransmission();
}

void commandWithParam(uint8_t command, uint8_t param) {
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

void putPixel(uint8_t x, uint8_t y) {
  setPageAddress(y / 8);
  setLowerColAddress(x & 0x0f);
  setHigherColAddress(x >> 4);
}

void sendBuffer( uint8_t* buffer) {
  putPixel(0, 0);
  commandWithParam(0x20, 0x00);
  bufferCommand((uint8_t *)buffer, BUFFER_LENGTH);
  commandWithParam(0x20, 0x02);
}

void setPageAddress(uint8_t p) {
  singleCommand(0xb0 + p);
}

void setLowerColAddress(uint8_t l) {
  singleCommand(0x00 + l);
}

void setHigherColAddress(uint8_t h) {
  singleCommand(0x10 + h);
}

void putToBuffer(uint8_t x, uint8_t y, uint8_t data, uint8_t* buffer) {
  int div = y / 8;
  int rest = y - (8 * div);
  uint8_t firstLine = data << rest;
  uint8_t secondLine = data >> (8 - rest);
  buffer[x + (128 * ((y / 8)))] = firstLine;
  buffer[x + (128 * ((y / 8) + 1))] = secondLine;
}