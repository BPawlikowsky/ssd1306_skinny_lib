#include <stdint.h>
#include <Arduino.h>
#include "fonts/charSet.h"

char *binString(uint8_t n);
void printRotatedString(char* text, uint8_t* buffer, int bufferWidth, uint8_t* font, int fontHeight, int fontWidth, uint16_t* lut, int x, int y);
void printString(char* string, uint8_t* buffer, int bufferWidth, int bx, int by, char* font, int width, int height);
