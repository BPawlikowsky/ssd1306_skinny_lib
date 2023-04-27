// #include <inttypes.h>
#include <pgmspace.h>
#include <stdint.h>
void getGlyph(uint8_t* font, uint8_t* arr, int height);
void rotateGlyph90CW(uint8_t* arr, int height, int width);
void copyPixel(uint8_t* src, uint8_t* dest, int srcX, int srcY, int destX, int destY, int size);
void stringToCharArray(char* text, uint8_t* buffer, uint8_t* font, int fontHeight, int fontWidth, uint16_t* lut, int x, int y);
