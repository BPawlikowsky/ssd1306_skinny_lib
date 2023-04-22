#include <Wire.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define COMMAND_CODE 0b00100000
#define DATA_CODE 0x40
#define BUFFER_LENGTH SCREEN_WIDTH * ((SCREEN_HEIGHT) / 8)

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3d  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
double currentTime, lastTime;

constexpr size_t I2C_BUFFER_SIZE = 1024;


const unsigned char PROGMEM numOne[] = {
  0b00000000,
  0b00000000,
  0b10000010,
  0b11111111,
  0b10000000,
  0b00000000,
  0b00000000,
  0b00000000
};

const unsigned char PROGMEM numTwo[] = {
  0b00000000,
  0b11100010,
  0b10010001,
  0b10001001,
  0b11000110,
  0b00000000,
  0b00000000,
  0b00000000
};

const unsigned char PROGMEM numThree[] = {
  0b00000000,
  0b01000010,
  0b10000001,
  0b10001001,
  0b01110110,
  0b00000000,
  0b00000000,
  0b00000000
};

const unsigned char PROGMEM numFour[] = {
  0b00000000,
  0b00001111,
  0b00001000,
  0b10001000,
  0b11111111,
  0b10000000,
  0b00000000,
  0b00000000
};

const unsigned char PROGMEM numFive[] = {
  0b01000000,
  0b10000111,
  0b10000101,
  0b10001001,
  0b01110011,
  0b00000000,
  0b00000000,
  0b00000000
};

const unsigned char PROGMEM numSix[] = {
  0b00000000,
  0b01111110,
  0b10001001,
  0b10001001,
  0b01110010,
  0b00000000,
  0b00000000,
  0b00000000
};

const unsigned char PROGMEM numSeven[] = {
  0b00000000,
  0b11100011,
  0b00011001,
  0b00000101,
  0b00000011,
  0b00000000,
  0b00000000,
  0b00000000
};

const unsigned char PROGMEM numEight[] = {
  0b00000000,
  0b01110110,
  0b10001001,
  0b10001001,
  0b01110110,
  0b00000000,
  0b00000000,
  0b00000000
};

const unsigned char PROGMEM numNine[] = {
  0b00000000,
  0b01000110,
  0b10001001,
  0b10001001,
  0b01111110,
  0b00000000,
  0b00000000,
  0b00000000
};


const unsigned char PROGMEM numZero[] = {
  0b00000000,
  0b01111110,
  0b10000001,
  0b10000001,
  0b01111110,
  0b00000000,
  0b00000000,
  0b00000000
};

const uint8_t PROGMEM *buffer = (uint8_t *)malloc(BUFFER_LENGTH);

void setup() {
  int rstPin = -1;
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(3400000);
  size_t res = Wire.setBufferSize(I2C_BUFFER_SIZE);
  Serial.println(res);
  startupSequence();
  memset((uint8_t *)buffer, 0, BUFFER_LENGTH);
  clearDisplay();
}

void loop() {
  clearDisplay();
  byte offset = 24;
  putPixel(offset, 16);
  lastTime = millis();
  for (int i = 0; i < 8; i++) {
    int numOffset = 0;
    putToBuffer(offset + i, 16);
    dataCommand((byte)numOne[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numTwo[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numThree[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numFour[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numFive[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numSix[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numSeven[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numEight[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numNine[i]);

    numOffset += 8;
    putToBuffer(offset + i + numOffset, 16);
    dataCommand(numZero[i]);
  }
  currentTime = millis();
  Serial.println(currentTime - lastTime);
}

void singleCommand(byte command) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)COMMAND_CODE);
  Wire.write((uint8_t)command);
  Wire.endTransmission();
}

void commandWithParam(byte command, byte param) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)COMMAND_CODE);
  Wire.write((uint8_t)command);
  Wire.write((uint8_t)param);
  Wire.endTransmission();
}

void dataCommand(uint8_t data) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)DATA_CODE);
  Wire.write((uint8_t)data);
  Wire.endTransmission();
}


void bufferCommand(uint8_t *data, int len) {
  Wire.beginTransmission(SCREEN_ADDRESS);
  Wire.write((uint8_t)DATA_CODE);
  Wire.write(data, len);
  Wire.endTransmission();
}

void startupSequence() {
  singleCommand(0xae);           // Display Off
  commandWithParam(0xA8, 0x3F);  // Set MUX Ratio( set to 63)
  commandWithParam(0xD3, 0x00);  // Set Display Offset
  singleCommand(0x40);           // Set Display Start Line
  singleCommand(0xA0);           // Set Segment re-map A0h/A1h
  singleCommand(0xC0);           // Set COM Output Scan Direction C0h/C8h
  commandWithParam(0xDA, 0x12);  // Set COM Pins hardware configuration(0x02 for 32 lines, 0x12 for 64 lines)
  commandWithParam(0x81, 0x7F);  // Set Contrast Control
  singleCommand(0xa4);           // Disable Entire Display On
  singleCommand(0xa6);           // Set Normal Display
  commandWithParam(0x20, 0x02);  // Set Memory addressing mode to Page addressing mode
  commandWithParam(0xD5, 0x80);  // Set Osc Frequency
  commandWithParam(0x8D, 0x15);  // Enable charge pump regulator
  singleCommand(0xaf);           // Display On
}

void clearDisplay() {
  lastTime = millis();
  putPixel(0, 0);
  commandWithParam(0x20, 0x00);
  bufferCommand((uint8_t*)buffer, BUFFER_LENGTH);
  commandWithParam(0x20, 0x02);
  currentTime = millis();
  Serial.println(currentTime - lastTime);
}

void setPageAddress(byte p) {
  singleCommand(0xb0 + p);
}

void setLowerColAddress(byte l) {
  singleCommand(0x00 + l);
}

void setHigherColAddress(byte h) {
  singleCommand(0x10 + h);
}

void putPixel(byte x, byte y) {
  setPageAddress(y / 8);
  setLowerColAddress(x & 0x0f);
  setHigherColAddress(x >> 4);
}

void putToBuffer(byte x, byte y, uint8_t data) {
  buffer[x + (128 * y)] = data;
}