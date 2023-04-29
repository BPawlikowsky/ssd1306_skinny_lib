#include "ssd1306-esp32-lib.hpp"
#include "stringToChar.hpp"
#include "fonts/charSet.h"

#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define CLK_SPEED 3400000

double currentTime, lastTime;

constexpr size_t I2C_BUFFER_SIZE = 1024;

uint8_t *buffer = (uint8_t *)malloc(BUFFER_LENGTH);

char *testText = "HELLO WORLD\nabcdefghijklmnoprstuwvxyz\tABCDEFGHIJKLMNOPRSTUWVXYZ\t1234567890!@#$%^&*()_+-,./?;:[]{}=<>\"\'`~|";
char text[100] = "0";
float x = 0.0f; float y = 0.0f;
float xIncDec = 0.5f;
float yIncDec = 1.0f;
int fps = 0;
int frames = 0;
char* font = font12x16;

void setup() {
  int rstPin = -1;
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(CLK_SPEED);
  size_t res = Wire.setBufferSize(I2C_BUFFER_SIZE);
  Serial.println(res);
  startupSequence();
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  lastTime = currentTime = micros();
}

void loop() {
  double start, end;
  start = micros();
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  // Basic text block tests
  // stringToCharArray(testText, buffer, (uint8_t *)font8, 9, 7, (uint16_t *)lut8, x, 0);
  // stringToCharArray(testText, buffer, (uint8_t *)font12, 13, 8, (uint16_t *)lut12, x, 0);
  // stringToCharArray(testText, buffer, (uint8_t *)font16, 16, 9, (uint16_t *)lut16, x, 0);
  // stringToCharArray(testText, buffer, (uint8_t *)font22, 22, 12, (uint16_t *)lut22, x, 0);
  // printString(testText, buffer, x, 0, font4x6, 4, 6);
  // printString(testText, buffer, x, 0, font6x8, 6, 8);
  // printString(testText, buffer, x, 0, font8x12, 8, 12);
  printString(testText, buffer, x, 0, font12x16, 12, 16);
  // FPS counter
  printString(text, buffer, 0, 0, font4x6, 4, 6);
  sendBuffer(buffer); // send buffer to ssd1306
  x+=xIncDec;
  y+=yIncDec;
  if(x >= 128.0f || x <= 0.0f) xIncDec = -xIncDec;
  if(y >= 64.0f || y <= 0.0f) yIncDec = -yIncDec;
  end = micros();
  // Serial.printf("%s", binString(font[0]));
  // Serial.printf("%s\n", binString(font[1]));
  // font+=2;
  
  sprintf(text, "%0.2ffps", (1000000 / (end - start)));
}
