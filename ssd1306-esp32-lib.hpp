#include <Wire.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define COMMAND_CODE 0b00100000
#define DATA_CODE 0x40
#define SCREEN_ADDRESS 0x3d  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define BUFFER_LENGTH SCREEN_WIDTH *((SCREEN_HEIGHT) / 8)
void singleCommand(uint8_t);
void commandWithParam(uint8_t, uint8_t);
void dataCommand(uint8_t data);
void bufferCommand(uint8_t *data, int len);
void startupSequence(void);
void sendBuffer(uint8_t* buffer);
void setPageAddress(uint8_t p);
void setLowerColAddress(uint8_t l);
void setHigherColAddress(uint8_t h);
void putPixel(uint8_t x, uint8_t y);
void putToBuffer(uint8_t x, uint8_t y, uint8_t data, uint8_t* buffer);