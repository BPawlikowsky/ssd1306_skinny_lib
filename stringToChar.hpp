// #include <inttypes.h>
#include <pgmspace.h>
#include <stdint.h>
void stringToCharArray(char* text, uint8_t* buffer, uint8_t* font, int fontHeight, int fontWidth, uint16_t* lut, int x, int y);
void printString(char* string, uint8_t* buffer, int bx, int by, char* font, int width, int height);
char *binString(uint8_t n);
