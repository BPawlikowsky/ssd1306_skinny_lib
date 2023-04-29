#include "graphics.hpp"

void printStringToBuffer(char* text, uint8_t* buffer, int x, int y, int fontSize) {
	switch (fontSize)
	{
	case 1: {
		printString(text, buffer, x, y, (char*)font4x6, 4, 6);
	} break;
	case 2: {
  		printString(text, buffer, x, y, (char*)font6x8, 6, 8);
	} break;
	case 3: {
  		printRotatedString(text, buffer, (uint8_t *)font8, 9, 7, (uint16_t *)lut8, x, y);
	} break;
	case 4: {
  		printString(text, buffer, x, y, (char*)font8x12, 8, 12);
	} break;
	case 5: {
  		printString(text, buffer, x, y, (char*)font12x16, 12, 16);
	} break;
	case 6: {
  		printRotatedString(text, buffer, (uint8_t *)font12, 13, 8, (uint16_t *)lut12, x, y);
	} break;
	case 7: {
  		printRotatedString(text, buffer, (uint8_t *)font16, 16, 9, (uint16_t *)lut16, x, y);
	} break;
	case 8: {
  		printRotatedString(text, buffer, (uint8_t *)font22, 22, 12, (uint16_t *)lut22, x, y);
	} break;
	
	default:
		break;
	}
}