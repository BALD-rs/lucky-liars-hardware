#include "nixie.h"
#include <Arduino.h>

Nixie::Nixie(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD) {
    this->pinA = pinA;
    this->pinB = pinB;
    this->pinC = pinC;
    this->pinD = pinD;

    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(pinC, OUTPUT);
    pinMode(pinD, OUTPUT);
}

void Nixie::setTube(uint8_t num) {
    if (num > 9) {
        num = 0;
    }

    uint8_t binaryRep[4];
    // Initialize all elements to 0
    for (int i = 0; i < 4; ++i) {
        binaryRep[i] = 0;
    }

    int i = 3;
    while(num > 0 && i >= 0) {
        binaryRep[i] = num % 2;
        num = num / 2; 
        i--; 
    }

    // d, c, b, a notation
    digitalWrite(pinD, binaryRep[0] ? HIGH : LOW);
    digitalWrite(pinC, binaryRep[1] ? HIGH : LOW);
    digitalWrite(pinB, binaryRep[2] ? HIGH : LOW);
    digitalWrite(pinA, binaryRep[3] ? HIGH : LOW);
}

