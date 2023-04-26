#include "ssd1306-esp32-lib.hpp"
#include "stringToChar.hpp"
#include "fonts/charSet.h"

#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define CLK_SPEED 400000

double currentTime, lastTime;

constexpr size_t I2C_BUFFER_SIZE = 1024;

uint8_t *buffer = (uint8_t *)malloc(BUFFER_LENGTH);

char *text2 = "HELLO WORLD\nabcdefghijklmnoprstuwvxyz\tABCDEFGHIJKLMNOPRSTUWVXYZ\t1234567890!@#$%^&*()_+-,./?;:[]{}=<>\"\'`~|";
char text[100];
int x = 0; int y = 0;
int xIncDec = 1;
int yIncDec = 1;

void setup() {
  int rstPin = -1;
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(CLK_SPEED);
  size_t res = Wire.setBufferSize(I2C_BUFFER_SIZE);
  Serial.println(res);
  startupSequence();
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  lastTime = currentTime = millis();
}

void loop() {
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  sprintf(text, "%d", (int)currentTime);
  stringToCharArray(text2, buffer, (uint8_t *)font8, 9, 7, (uint16_t *)lut, x, y);
  stringToCharArray(text, buffer, (uint8_t *)font8, 9, 7, (uint16_t *)lut, 0, 55);
  sendBuffer(buffer);
  currentTime = millis();
  // if(currentTime - lastTime > 10) {
    x+=xIncDec;
    y+=yIncDec;
    if(x > 100 || x < 0) xIncDec = -xIncDec;
    if(y > 50 || y < 0) yIncDec = -yIncDec;
  //   lastTime = millis();
  // }
}
