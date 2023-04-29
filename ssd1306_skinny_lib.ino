#include "ssd1306-esp32-lib.hpp"
#include "graphics.hpp"

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
  printStringToBuffer(testText, buffer, x, 0, 1);
  // printStringToBuffer(testText, buffer, x, 0, 2);
  // printStringToBuffer(testText, buffer, x, 0, 3);
  // printStringToBuffer(testText, buffer, x, 0, 4);
  // printStringToBuffer(testText, buffer, x, 0, 5);
  // printStringToBuffer(testText, buffer, x, 0, 6);
  // printStringToBuffer(testText, buffer, x, 0, 7);
  // printStringToBuffer(testText, buffer, x, 0, 8);

  // FPS counter
  printStringToBuffer(text, buffer, 0, 0, 1);
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
