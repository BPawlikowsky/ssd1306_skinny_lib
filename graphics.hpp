#include "strings.hpp"

void printStringToBuffer(char* text, uint8_t* buffer,int bufferWidth, int x, int y, int fontSize);
void putPixelInBuffer(uint8_t* buffer, int bufferWidth, int x, int y, int color);
int readPixel(uint8_t* buffer, int bufferWidth, int x, int y);
void putHorizontalLine(uint8_t* buffer, int bufferWidth, int x, int y, int lineLength, int color);
void putVerticalLine(uint8_t* buffer, int bufferWidth, int x, int y, int lineLength, int color);
void putBox(uint8_t* buffer, int bufferWidth, int x, int y, int boxWidth, int boxHeight, int color);