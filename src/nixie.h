/**
 * nixie.h
 * 
 * Contains methods for doing business with nixie tubes using a K155ID1 driver
*/

#ifndef NIXIE_H
#define NIXIE_H

#include <string>

class Nixie {
public:

    /**
     * Default constructor takes in pin numbers
    */
    Nixie(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD);

    /**
     * Sets the tube from 0-9
    */
    void setTube(uint8_t num);

private:
    uint8_t pinA;
    uint8_t pinB;
    uint8_t pinC;
    uint8_t pinD;
};

#endif
