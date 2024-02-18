/**
 * BALD-rs
 * University of Nebraska-Lincoln
 * Cornhacks, February 17th, 2024.
 * 
 * Dawson McGahan
 * 
 * File for interfacing with the peripherals of 'Project Escobar'
*/

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

#include <TFT_eSPI.h> // Hardware specific library. Use Setup61 in User_Setup_Select.h
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

// Our specific screen is 320x240.
uint16_t screen[320][240];

/**
 * Initializes each cell in the array to 0xFFFF. Keeping track of the array in memory 
 * is quicker than reading it from the screen directly.
*/
void initArray();

/**
 * Writes a simple sine wave across the width of the screen
*/
void sineWave();

/**
 * Shifts every pixel in the screen left, rotating the first column into the last position
*/
void rotLeft();

/**
 * Shifts every pixel in the screen left, deleting the first column and setting the last
 * column to white.
*/
void shiftLeft();

/**
 * Used to test serial input and rotation on the display. Toggles the position of the "cursor"
 * based on the current mode
*/
void toggleHigh();

void setup(void) {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(3);
  initArray();
  tft.fillScreen(WHITE);
  tft.setTextColor(WHITE, BLACK);
  sineWave();
}

void loop() {
  toggleHigh();
}

void initArray() {
    for (int i = 0; i < tft.width(); i++) {
        for (int j = 0; j < tft.height(); j++) {
            screen[i][j] = 0xFFFF;
        }
    }
}

void sineWave() {
  int x = 0;
  int y = 0;
  for (x = 0; x < tft.width(); x += 1) {
    y = round(50 * sin(( ((double) x) / tft.width()) * 2 * PI)) + 120;
    tft.drawPixel(x, 240 - y, BLACK);
    screen[x][240 - y] = BLACK;
  } 
}

void rotLeft() {
  uint16_t x = tft.width();
  uint16_t y = tft.height();
  uint16_t color;
  uint16_t nextColor;

  uint16_t firstRowColors[y];

  for (int i = 0; i < y; i++) {
    firstRowColors[i] = screen[0][i];
  }

  for (int i = 0; i < x - 1; i++) {
    for (int j = 0; j < y; j++) {
      color = screen[i][j];
      nextColor = screen[i + 1][j];
      if (nextColor == BLACK) {
        tft.drawPixel(i, j, BLACK);
        screen[i][j] = BLACK;
        tft.drawPixel(i + 1, j, WHITE);
        screen[i + 1][j] = WHITE;
      } else if (color == BLACK) {
        tft.drawPixel(i, j, nextColor);
        screen[i][j] = nextColor;
      }
    }
  }
  for (int j = 0; j < y; j++) {
    tft.drawPixel(tft.width() - 1, j, firstRowColors[j]);
    screen[tft.width() - 1][j] = firstRowColors[j];
  }
}

void shiftLeft() {
  uint16_t x = tft.width();
  uint16_t y = tft.height();
  uint16_t color;
  uint16_t nextColor;

  for (int i = 0; i < x - 1; i++) {
    for (int j = 0; j < y; j++) {
      color = screen[i][j];
      nextColor = screen[i + 1][j];
      if (nextColor == BLACK) {
        tft.drawPixel(i, j, BLACK);
        screen[i][j] = BLACK;
        tft.drawPixel(i + 1, j, WHITE);
        screen[i + 1][j] = WHITE;
      } else if (color == BLACK) {
        tft.drawPixel(i, j, nextColor);
        screen[i][j] = nextColor;
      }
    }
  }
  for (int j = 0; j < y; j++) {
    tft.drawPixel(tft.width() - 1, j, WHITE);
    screen[tft.width() - 1][j] = WHITE;
  }
}

void toggleHigh() {
  uint8_t mode = -1; // -1 for low, 1 for high

  while(true) {
    uint8_t input = Serial.read();

    // if 1 is pressed, toggle
    if (input == 0x31) {
      mode *= -1;
    }

    if (mode == 1) {
      tft.drawPixel(tft.width() - 1, tft.height() / 2 - 20, BLACK);
      screen[tft.width() - 1][tft.height() / 2 - 20] = BLACK;
    } else {
      tft.drawPixel(tft.width() - 1, tft.height() / 2, BLACK);
      screen[tft.width() - 1][tft.height() / 2] = BLACK;
    }

    shiftLeft();
  }
}