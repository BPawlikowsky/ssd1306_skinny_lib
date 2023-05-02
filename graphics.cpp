#include "graphics.hpp"

void printStringToBuffer(char* text, uint8_t* buffer, int bufferWidth, int x, int y, int fontSize) {
	switch (fontSize)
	{
	case 1: {
		printString(text, buffer, bufferWidth, x, y, (char*)font4x6, 4, 6);
	} break;
	case 2: {
  		printString(text, buffer, bufferWidth, x, y, (char*)font6x8, 6, 8);
	} break;
	case 3: {
  		printRotatedString(text, buffer, bufferWidth, (uint8_t *)font8, 9, 7, (uint16_t *)lut8, x, y);
	} break;
	case 4: {
  		printRotatedString(text, buffer, bufferWidth, (uint8_t *)font12, 13, 8, (uint16_t *)lut12, x, y);
	} break;
	case 5: {
  		printString(text, buffer, bufferWidth, x, y, (char*)font8x12, 8, 12);
	} break;
	case 6: {
  		printRotatedString(text, buffer, bufferWidth, (uint8_t *)font16, 16, 9, (uint16_t *)lut16, x, y);
	} break;
	case 7: {
  		printString(text, buffer, bufferWidth, x, y, (char*)font12x16, 12, 16);
	} break;
	case 8: {
  		printRotatedString(text, buffer, bufferWidth, (uint8_t *)font22, 22, 12, (uint16_t *)lut22, x, y);
	} break;
	
	default: printString(text, buffer, bufferWidth, x, y, (char*)font6x8, 6, 8);
		break;
	}
}

void putPixelInBuffer(uint8_t* buffer, int bufferWidth, int x, int y, int color) {
	int bufferOffset = x + (bufferWidth * (y / 8));
  int destOffset = y - (((y / 8) * 8));
  uint8_t destByte = buffer[bufferOffset];
  if(color > 0) {
  	destByte = buffer[bufferOffset] | (0b00000001 << destOffset);
  } else {
	if(readPixel(buffer, bufferWidth, x, y)) {
		destByte = buffer[bufferOffset] ^ (0b00000001 << (destOffset));
  	}
  }
  buffer[x + (bufferWidth * (y / 8))] = destByte;
}

int readPixel(uint8_t* buffer, int bufferWidth, int x, int y) {
  	int destOffset = y - ((y / 8) * 8);
	uint8_t destByte = buffer[x + (bufferWidth * (y / 8))];
	uint8_t pixel = 0b00000001 & (destByte >> (destOffset));
	return pixel;
	
}

void putHorizontalLine(uint8_t* buffer, int bufferWidth, int x, int y, int lineLength, int color) {
	for(int i = 0; i < lineLength; i++) {
 	   putPixelInBuffer(buffer, bufferWidth, x + i, y, color);
  	}
}

void putVerticalLine(uint8_t* buffer, int bufferWidth, int x, int y, int lineLength, int color) {
	for(int i = 0; i < lineLength; i++) {
 	   putPixelInBuffer(buffer, bufferWidth, x, y + i, color);
  	}
}

void putBox(uint8_t* buffer, int bufferWidth, int x, int y, int boxWidth, int boxHeight, int color) {
		for(int i = 0; i < boxHeight; i++) {
	 	   putHorizontalLine(buffer, bufferWidth, x, y + i, boxWidth, color);
	  	}
}