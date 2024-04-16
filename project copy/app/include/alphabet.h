// Header file for alphabet.c
// Used to write individual characters on the LED Display
// Note: All characters display a 5:7 aspect ratio (width:height). All characters are uppercase

#ifndef _ALPHABET_H_
#define _ALPHABET_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "displaylogic.h"

// A
void A(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// B
void B(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// C
void C(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// // D
void D(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// // E
void E(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// F
void F(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// G
void G(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// H
void H(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// I
void I(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// J
void J(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// K
void K(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// L
void L(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// M
void M(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// N
void N(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// O
void O(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// P
void P(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// Q
void Q(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// R
void R(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// S
void S(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// T
void T(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// U
void U(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// V
void V(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// W
void W(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// X
void X(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// Y
void Y(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// Z
void Z(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// Numbers

// 0
void zero(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 1
void one(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 2
void two(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 3
void three(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 4
void four(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 5
void five(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 6
void six(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 7
void seven(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 8
void eight(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// 9
void nine(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// Special characters

// Semicolon (:)
void semicolon(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// Space
void space(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

// Display all characters
void displayAllCharacters(int spiFileDesc, uint8_t cmd, uint8_t color, uint8_t spaceColor, int fontSize, int xStart, int yStart, int charDelay, int spaceDelay, bool cond);

#endif