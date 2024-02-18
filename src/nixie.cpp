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
    switch(num) {
        case 1:
            digitalWrite(pinD, LOW);
            digitalWrite(pinC, LOW);
            digitalWrite(pinB, LOW);
            digitalWrite(pinA, HIGH);
            break;
        case 2:
            digitalWrite(pinD, LOW);
            digitalWrite(pinC, LOW);
            digitalWrite(pinB, HIGH);
            digitalWrite(pinA, LOW);
            break;
        default:
            break;
    }
}

