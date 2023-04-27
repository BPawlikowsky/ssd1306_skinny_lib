#include "pgmspace.h"
#include "stringToChar.hpp"
#include <Arduino.h>

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

void getGlyph(uint8_t* font, uint8_t* arr, int height) {
  if(height == 16) {
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

void printBin(uint8_t* arr, char* message, int size, int height) {
    
  Serial.println(message);
  for(int i = 0; i < (height * (size/height)); i++) {
    Serial.print(binString(arr[i+height]));
    Serial.println(binString(arr[i]));
  }
  Serial.println("");
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

void copyPixel(uint8_t* src, uint8_t* dest, int srcX, int srcY, int destX, int destY, int height) {
  int srcOffset = srcY - ((srcY / 8) * 8);
  int destOffset = destY - ((destY / 8) * 8);
  uint8_t mask = (0b00000001 << (7 - srcOffset));
  uint8_t srcByte = src[srcX + (height * (srcY / 8))] & mask;
  srcByte = srcByte >> (7 - srcOffset);
  uint8_t destByte = dest[destX + (height * (destY / 8))] | (((srcByte == 1) ? 0b00000001 : 0x00) << (destOffset));
  dest[destX + (height * (destY / 8))] = destByte;
}

void stringToCharArray(char* text, uint8_t* buffer, uint8_t* font, int fontHeight, int fontWidth, uint16_t* lut, int x, int y) {
  // Map text to glyphs
  char* p = text;
  char character;
  uint8_t* bp = buffer;
  int size = fontHeight * ((fontHeight/8)+2);
  uint8_t* arr = (uint8_t*)malloc(size * sizeof(uint8_t));
  memset(arr, 0, size * sizeof(uint8_t));

  while ((character = *p) != '\0') {
    int pageOffset = bp - buffer;
    int endOfPageDist = 128 - pageOffset%128;

    if(bp - buffer > 128 * 8) {
      break;
    }

    if((128 - pageOffset%128) <= fontWidth + x) {
        bp+=(endOfPageDist);
    }

    if(character == '\n') {
      bp+=(endOfPageDist - x);
    } else if(character == '\t') {
      bp+=(fontWidth * 3);
    } else {
      int lutOffset = ((uint8_t)character) - ((uint8_t)' ');
      uint8_t* fp = &font[lut[lutOffset]];
      getGlyph(fp, arr, fontHeight);
      rotateGlyph90CW(arr, fontHeight, fontWidth);
      if(fontHeight == 22) {
        for (int i = 0; i < fontWidth; i++) {
          bp[i + x + (128 * (y / 8))] = arr[i];
          bp[i + 128 + x + (128 * (y / 8))] = arr[i + fontHeight];
          bp[i + 256 + x + (128 * (y / 8))] = arr[i + (fontHeight*2)];
        }
      } else {
        for (int i = 0; i < fontWidth; i++) {
          bp[i + x + (128 * (y / 8))] = arr[i];
          bp[i + 128 + x + (128 * (y / 8))] = arr[i + fontHeight];
        }
      }

      bp += fontWidth;
    }
    p++;
  }

  free(arr);
}