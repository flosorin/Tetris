/************************************************************************
* Classic Tetris game displayed on LED matrices by an Arduino UNO
* https://github.com/flosorin/Tetris.git
* 
* Using Eberhard Fahle's LedControl library  
* http://wayoda.github.io/LedControl/
* 
* Using Peter Knight's TrueRandom library
* http://code.google.com/p/tinkerit
* 
* Multi7Seg library by Florian Sorin (2018) 
* https://github.com/flosorin/Multi7Seg.git
* 
* ButtonsController library by Florian Sorin (2020)
* https://github.com/flosorin/ButtonsController.git
* 
************************************************************************/

#include <Multi7Seg.h>
#include <LedControl.h>
#include <ButtonsController.h>

#include "TetrisPiece.h"
#include "TetrisMusic.h"

/* MAX7219 managing game matrices pins */
#define PIN_DATA_IN_GAME 13
#define PIN_CLK_MAX7219_GAME 12
#define PIN_LOAD_GAME 11

/* MAX7219 managing next piece displaying pins */
#define PIN_DATA_IN_SHAPE 10
#define PIN_CLK_MAX7219_SHAPE 9
#define PIN_LOAD_SHAPE 8

/* 7 segment displays and 74HC595 pins */
#define NB_7_SEG 5
#define PIN_DIGIT0 3
#define PIN_DIGIT1 4
#define PIN_DIGIT2 5
#define PIN_DIGIT3 6
#define PIN_DIGIT4 7
#define PIN_CLK_74HC595 A4
#define PIN_LATCH A3
#define PIN_DATA A2

/* Buttons const */
#define PIN_BUTTONS A1

/* Music buzzer */
#define PIN_BUZZER 2

