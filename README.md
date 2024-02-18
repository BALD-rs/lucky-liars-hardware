Cornhacks 2024 - Hardware interface
=====================================

![Cornhacks 24](https://newsroom.unl.edu/announce/files/file194465.png)

## Introduction
This program was written at the cornhacks 2024 competition to interface between our game and the custom peripheral designed for it. The peripheral
is built on the Raspberry Pi Pico and serves two main functions - to display the result of a D20 roll, and
to output a polygraph-style image used to enhance the game. This code was mainly written by Dawson McGahan, with 
crucial assistance from Blaine Traudt.

The polygraph-style image is displayed on a 2.8" TFT display that can be driven with the TFT_eSPI library. The D20
roll is displayed on two IN-14 nixie tubes. The pico controls these tubes using the  [ K155IN1](https://tubehobby.com/datasheets/k155id1.pdf) BCD/Decimal driver.

## The Game
The peripheral is required to enable TTS, making the game more realistic, as if the user was pressing an intercom button.
After the user releases the button, a D20 is rolled and the result is displayed using the nixie tubes with a special
animation. This value is similar to a "perception check," and the result impacts the quality of the polygraph shown. A higher number rolled means a clearer, higher quality polygraph is shown on the display, and can be used to determine the true killer.

## Layout
![Overall Layout](/img/IMG_5011.jpg)
![Front View](/img/IMG_5012.JPG)
![Nixie](/img/IMG_5013.jpg)