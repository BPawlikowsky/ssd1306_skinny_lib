#include "pgmspace.h"
#include "stringToChar.hpp"
#include <Arduino.h>

void getGlyph(uint8_t* font, uint8_t* arr) {
  for (int i = 0; i < 9; i++) {
    arr[i] = font[i];
  }

  // int height = 9;
  // int width = 8;
  // for (int x = 0; x < height; x++) {
  //   for (int y = width - 1; y >= 0; y--) {
  //     copyPixel(font, arr, x, y, width - y - 1, x);
  //   }
  // }
}

void rotateGlyph90CW(uint8_t* arr, int height) {
  int size = height * 2;
  uint8_t* arrTemp = (uint8_t*)malloc(size * sizeof(uint8_t));
  uint8_t* arrRot = (uint8_t*)malloc(size * sizeof(uint8_t));
  memset(arrTemp, 0, size);
  memset(arrRot, 0, size);
  for (int x = 0; x < height; x++) {
    for (int y = 0; y < height; y++) {
      copyPixel(arr, arrTemp, x, y, height - y - 1, x);
    }
  }
  for (int x = 0; x < height; x++) {
    for (int y = 0; y < height; y++) {
      copyPixel(arrTemp, arrRot, x, y, height - x - 1, (height - y) - 3);
    }
  }
  for (int i = 0; i < size; i++) {
    *arr++ = arrRot[i];
  }
  free(arrTemp);
  free(arrRot);
}

void copyPixel(uint8_t* src, uint8_t* dest, int srcX, int srcY, int destX, int destY) {
  int srcOffset = srcY - ((srcY / 8) * 8);
  int destOffset = destY - ((destY / 8) * 8);
  uint8_t mask = (0b00000001 << (7 - srcOffset));
  uint8_t srcByte = src[srcX + (128 * (srcY / 8))] & mask;
  srcByte = srcByte >> (7 - srcOffset);
  uint8_t destByte = dest[destX + (128 * (destY / 8))] | (((srcByte == 1) ? 0b00000001 : 0x00) << (7 - destOffset));
  dest[destX + (128 * (destY / 8))] = destByte;
}

void stringToCharArray(char* text, uint8_t* buffer, uint8_t* font, int fontHeight, int fontWidth, uint16_t* lut, int x, int y) {
  // Map text to glyphs
  char* p = text;
  char character;
  uint8_t* bp = buffer;
  uint8_t* arr = (uint8_t*)malloc(16 * 2 * sizeof(uint8_t));
  memset(arr, 0, fontHeight * 2 * sizeof(uint8_t));

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
      
      getGlyph(fp, arr);
      rotateGlyph90CW(arr, fontHeight);
      for (int i = 0; i < fontWidth; i++) {
        bp[i + x + (128 * (y / 8))] = arr[i];
        bp[i + 128 + x + (128 * (y / 8))] = arr[i + fontHeight];
      }

      bp += fontWidth;
    }
    p++;
  }

  free(arr);
}