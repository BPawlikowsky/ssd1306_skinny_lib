#include "ssd1306-esp32-lib.hpp"
#include "stringToChar.hpp"
#include "fonts/charSet.h"

#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define CLK_SPEED 400000

double currentTime, lastTime;

constexpr size_t I2C_BUFFER_SIZE = 1024;

uint8_t *buffer = (uint8_t *)malloc(BUFFER_LENGTH);

char *testText = "HELLO WORLD\nabcdefghijklmnoprstuwvxyz\tABCDEFGHIJKLMNOPRSTUWVXYZ\t1234567890!@#$%^&*()_+-,./?;:[]{}=<>\"\'`~|";
char text[100] = "0";
int x = 0; int y = 0;
int xIncDec = 2;
int yIncDec = 1;
int fps = 0;
int frames = 0;

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
  stringToCharArray(testText, buffer, (uint8_t *)font8, 9, 7, (uint16_t *)lut8, 0, 0);
  stringToCharArray(text, buffer, (uint8_t *)font8, 9, 7, (uint16_t *)lut8, x, y);
  stringToCharArray(text, buffer, (uint8_t *)font12, 13, 8, (uint16_t *)lut12, x, y + 12);
  stringToCharArray(text, buffer, (uint8_t *)font16, 16, 9, (uint16_t *)lut16, x, y + 25);
  stringToCharArray(text, buffer, (uint8_t *)font22, 22, 12, (uint16_t *)lut22, x, y + 40);
  sendBuffer(buffer);
  x+=xIncDec;
  y+=yIncDec;
  if(x >= 128 || x < 0) xIncDec = -xIncDec;
  if(y >= 64 || y < 0) yIncDec = -yIncDec;
  end = micros();
  
  sprintf(text, "%0.2ffps", (1000000 / (end - start)));
}
