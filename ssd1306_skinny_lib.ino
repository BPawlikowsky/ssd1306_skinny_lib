#include "ssd1306-esp32-lib.hpp"
#include "stringToChar.hpp"
#include "fonts/charSet.h"

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define CLK_SPEED 400000

double currentTime, lastTime;

constexpr size_t I2C_BUFFER_SIZE = 1024;

uint8_t *buffer = (uint8_t *)malloc(BUFFER_LENGTH);

void setup() {
  int rstPin = -1;
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(CLK_SPEED);
  size_t res = Wire.setBufferSize(I2C_BUFFER_SIZE);
  Serial.println(res);
  startupSequence();
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
}

void loop() {
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);  // clear buffer
  // char* text = "HELLO WORLD \nabcdefghijklmnoprstuwvxyz\t1234567890!@#$%^&*()_+-,./?;:[]{}=";
  char text[100];
  sprintf(text, "%d", millis());
  stringToCharArray(text, buffer, (char*)font8, 9, 7, (uint16_t*)lut, 10, 0);
  sendBuffer(buffer);
}
