/**
 * BALD-rs
 * University of Nebraska-Lincoln
 * Cornhacks, February 17th, 2024.
 * 
 * Dawson McGahan
 * Blaine Traudt
 * 
 * File for interfacing with the peripherals of 'Project Escobar'
*/

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB
#define BUTTON 4

int needle = 0;

#include <TFT_eSPI.h> // Hardware specific library. Use Setup61 in User_Setup_Select.h
#include <SPI.h>
#include "nixie.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

TFT_eSPI tft = TFT_eSPI();
Nixie tube1 = Nixie(0, 2, 3, 1);
Nixie tube2 = Nixie(7, 13, 14, 12);
uint16_t screen[320][240];
std::mt19937 eng(random()); // Seed the generator

/**
 * Initializes each cell in the array to 0xFFFF. Keeping track of the array in memory 
 * is quicker than reading it from the screen directly.
*/
void initArray();

/**
 * Initialize buttons
*/
void initButton();

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
 * Generates the line for the polygraph. Input number is confidence. Higher values mean 
 * more likely to lie
*/
void polyGraph(uint8_t confidence);

/**
 * Checks if button pressed
*/
void checkButton();

/**
 * Intercepts serial data for interfacing with game
*/
void readGameDataFromSerial();

/**
 * Displays a goofy neat little animation that simulates rolling a die
 * Ends on the desired value;
*/
void rollDie(uint8_t d1, uint8_t d2);

void setup(void) {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(1);
  initButton();
  initArray();
  tft.fillScreen(WHITE);
}

void loop() {
  checkButton();
  readGameDataFromSerial();
  delay(500);
  readGameDataFromSerial();
}

void initArray() {
    for (int i = 0; i < tft.width(); i++) {
        for (int j = 0; j < tft.height(); j++) {
            screen[i][j] = 0xFFFF;
        }
    }
}

void initButton() {
  pinMode(BUTTON, INPUT_PULLUP);
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

void polyGraph(uint8_t confidence) {
  uint8_t change;

  std::uniform_int_distribution<> prob(0, 250);
  
  if (confidence >= 20) {
    std::uniform_int_distribution<> prob(0, 200);
  }

  
  while(Serial.read() != 'S') {
    if (prob(eng) == 0) {
      change = confidence;
    } else {
      change = 1;
    }

    std::uniform_int_distribution<> distr(-change, change); // Define the range for delta change
    std::uniform_int_distribution<> prob(0, 99);

    int delta = distr(eng); // Generate a small change to simulate wave

    if ((needle > 4 && delta > 0) || (needle < -4 && delta < 0)) {
        if (prob(eng) < 75) { // 75% chance to nudge towards center when further away
            delta = -delta / 2;
        }
    }

    needle += delta * 1.5; // Adjust needle position

    if (needle < -120) needle = -119;
    if (needle > 119) needle = 118;

    tft.drawPixel(tft.width() - 1, needle + 120, BLACK);
    tft.drawPixel(tft.width() - 1, needle + 119, BLACK);
    screen[tft.width() - 1][needle + 120] = BLACK;
    screen[tft.width() - 1][needle + 119] = BLACK;
    shiftLeft();
    needle += delta * 3;
  }

  tube1.setTube(10);
  tube2.setTube(10);
}

void checkButton() {
  while (digitalRead(BUTTON) == HIGH) {
    Serial.println("A");
    delay(100);
  }
  Serial.println("1");
  delay(20);
  while (digitalRead(BUTTON) == LOW) {
    Serial.println("B");
    delay(100);
  }
  Serial.println("0");
  delay(20);
}

void readGameDataFromSerial() {
  while (Serial.available() < 3) {
    asm volatile("nop");
  }

  while (Serial.available() > 0) {
    uint8_t readByte = Serial.read();
    char buffer[2];
    Serial.readBytes(buffer, 2);
    if(readByte == 'D') {
      rollDie(buffer[0] - 0x30, buffer[1] - 0x30);
    } else if (readByte == 'P') {
      polyGraph(((buffer[0] - 0x30) * 10) + (buffer[1] - 0x30));
    }
  }
}

void rollDie(uint8_t d1, uint8_t d2) {
  for (int i = 10; i <= 40; i += 10) {
    for(int j = 1; j < 10; j++) {
      tube1.setTube(j);
      delay(i);
      tube2.setTube(9 - j);
      delay(i);
    }
    for(int j = 1; j < 10; j++) {
      tube1.setTube(9 - j);
      delay(i);
      tube2.setTube(j);
      delay(i);
    }
  }

  tube1.setTube(d1);
  tube2.setTube(d2);
  delay(400);
  tube1.setTube(10);
  tube2.setTube(10);
  delay(400);
  tube1.setTube(d1);
  tube2.setTube(d2);
  delay(400);
  tube1.setTube(10);
  tube2.setTube(10);
  delay(400);
  tube1.setTube(d1);
  tube2.setTube(d2);
  
}