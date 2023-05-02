#include "strings.hpp"

char *binString(uint8_t n)
{
    static char bin[9];
    int x;

    for(x=0;x<8;x++)
    {
        bin[x] = n & 0x80 ? '1' : '0';
        n <<= 1;
    }
    bin[8] = '\0';

    return(bin);
}

void getGlyph(uint8_t* font, uint8_t* arr,int width, int height) {
  if(width == 8 || width == 12) {
    for (int i = 0; i < width*2; i+=2) {
      *arr = font[i];
      arr[width] = font[i+1];
      arr++;
    }
  } else if(height == 16) {
    for (int i = 0; i < height*2; i+=2) {
      *arr = font[i];
      arr[height] = font[i+1];
      arr++;
    }
  } else if(height == 22) {
    for (int i = 0; i <= height*2; i+=2) {
      *arr = font[i];
      arr[height] = font[i+1];
      arr++;
    }
  } else{
    for (int i = 0; i < height; i++) {
      arr[i] = font[i];
    }
  }
}

void copyPixel(uint8_t* src, uint8_t* dest, int srcX, int srcY, int destX, int destY, int height) {
  int srcOffset = srcY - ((srcY / 8) * 8);
  int destOffset = destY - ((destY / 8) * 8);
  uint8_t mask = (0b00000001 << (7 - srcOffset));
  uint8_t srcByte = src[srcX + (height * (srcY / 8))] & mask;
  srcByte = srcByte >> (7 - srcOffset);
  uint8_t destByte = dest[destX + (height * (destY / 8))] | (((srcByte == 1) ? 0b00000001 : 0x00) << (destOffset));
  dest[destX + (height * (destY / 8))] = destByte;
}

void copyPixelToBuffer(uint8_t* src, uint8_t* dest, int destWidth, int srcX, int srcY, int destX, int destY, int height) {
  int srcOffset = srcY - ((srcY / 8) * 8);
  int destOffset = destY - ((destY / 8) * 8);
  uint8_t mask = (0b00000001 << (7 - srcOffset));
  uint8_t srcByte = src[srcX + (height * (srcY / 8))] & mask;
  srcByte = srcByte >> (7 - srcOffset);
  uint8_t destByte = dest[destX + (destWidth * (destY / 8))] | (((srcByte == 1) ? 0b00000001 : 0x00) << (destOffset));
  dest[destX + (destWidth * (destY / 8))] = destByte;
}

void rotateGlyph90CW(uint8_t* arr, int height, int width) {
  int size = height * ((height/8)+2) * sizeof(uint8_t);
  uint8_t* arrTemp = (uint8_t*)malloc(size);
  uint8_t* arrRot = (uint8_t*)malloc(size);
  memset(arrTemp, 0, size);
  memset(arrRot, 0, size);

  for (int x = 0; x < height; x++) {
    for (int y = 0; y < height; y++) {
      copyPixel(arr, arrTemp, x, y, y, x, height);
    }
  }
  
  for (int x = 0; x < height; x++) {
    for (int y = 0; y < height; y++) {
      copyPixel(arrTemp, arrRot, x, y, height - x, height - y, height);
    }
  }
  
  for (int i = 0; i < size; i++) {
    arr[i] = arrTemp[i];
  }
  free(arrTemp);
  free(arrRot);
}

void printRotatedString(char* text, uint8_t* buffer, int bufferWidth, uint8_t* font, int fontHeight, int fontWidth, uint16_t* lut, int x, int y) {
  // Map text to glyphs
  char* p = text;
  char character;
  uint8_t* bp = buffer;
  int size = fontHeight * ((fontHeight/8)+2);
  uint8_t* arr = (uint8_t*)malloc(size * sizeof(uint8_t));
  memset(arr, 0, size * sizeof(uint8_t));

  while ((character = *p) != '\0') {
    int pageOffset = bp - buffer;
    int endOfPageDist = bufferWidth - pageOffset%bufferWidth;

    if(bp - buffer > bufferWidth * 8) {
      break;
    }

    if((endOfPageDist) <= fontHeight + x) {
      if(fontHeight >= 13) bp+=(bufferWidth + endOfPageDist);
      else bp+=(endOfPageDist);
    }

    if(character == '\n') {
      if(fontHeight >= 13) bp+=(bufferWidth + endOfPageDist - x);
      else bp+=(endOfPageDist - x);
    } else if(character == '\t') {
      bp+=(fontWidth * 3);
    } else {
      int lutOffset = ((uint8_t)character) - ((uint8_t)' ');
      uint8_t* fp = &font[lut[lutOffset]];
      getGlyph(fp, arr, 0, fontHeight);
      rotateGlyph90CW(arr, fontHeight, fontWidth);
      if(fontHeight == 22) {
        for (int i = 0; i < fontWidth; i++) {
          bp[i + x + (bufferWidth * (y / 8))] = arr[i];
          bp[i + bufferWidth + x + (bufferWidth * (y / 8))] = arr[i + fontHeight];
          bp[i + (bufferWidth * 2) + x + (bufferWidth * (y / 8))] = arr[i + (fontHeight*2)];
        }
      } else {
        for (int i = 0; i < fontWidth; i++) {
          bp[i + x + (bufferWidth * (y / 8))] = arr[i];
          bp[i + bufferWidth + x + (bufferWidth * (y / 8))] = arr[i + fontHeight];
        }
      }

      bp += fontWidth;
    }
    p++;
  }

  free(arr);
}

void printString(char* string, uint8_t* buffer, int bufferWidth, int bx, int by, char* font, int width, int height) {
  char character;
  int offset = 0;
  uint8_t* bp = buffer;
  while((character = *string++) != '\0') {
    if(offset > bufferWidth * 8) {
      break;
    }

    int endOfPageDist = bufferWidth - ((offset + bx) - (bufferWidth * (offset/bufferWidth)));

    if(endOfPageDist < height + 2) {
      if(width >= 8) offset+=(bufferWidth + endOfPageDist + bx);
      else offset+=(endOfPageDist + bx);
    }

    if(character == '\n') {
      if(width >= 8) offset+=(bufferWidth + endOfPageDist + bx - height);
      else offset+=(endOfPageDist + bx - height);
    } else if(character == '\t') {
      offset+=(width * 3);
    } else if(width < 8) {
      char* fp = &font[(character - ' ') * width];
      for (int x = 0; x < width; x++) {
        for (int y = 0; y < 8; y++) {
          copyPixelToBuffer((uint8_t*)fp, bp, bufferWidth, x, y, x + offset + bx, y + by, height);
        }
      }
    } else if(width < 12) {
      char* fp = &font[(character - ' ') * (width*2)];
      uint8_t arr[width*2];
      memset(arr, 0, width*2);
      getGlyph((uint8_t*)fp, arr, width, height);
      for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height + 3; y++) {
          copyPixelToBuffer(arr, bp, bufferWidth, x, y, x + offset + bx, y + by, width);
        }
      }
    } else {
      char* fp = &font[(character - ' ') * (width*2)];
      uint8_t arr[width*2];
      memset(arr, 0, width*2);
      getGlyph((uint8_t*)fp, arr, width, height);
      for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
          copyPixelToBuffer(arr, bp, bufferWidth, x, y, x + offset + bx, y + by, width);
        }
      }
    }
    offset+=height;
  }
}
