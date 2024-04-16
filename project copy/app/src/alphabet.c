// Source file for alphabet.c
// Used to write individual characters on the LED Display
// Note: All characters display a 5:7 aspect ratio (width:height). All characters are uppercase

#include "alphabet.h"

// displayRectangle(spiFileDesc, cmd, color, xStart, xEnd, yStart, yEnd); 

// A
void A(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display left vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 7), cond);

    // Display the top horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond); 

    // Display the middle horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the right vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 7), cond);
}

// B
void B(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 7), cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the upper-right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the bottom-right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 4), yStart + (fontSize * 6), cond);
}

// C
void C(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the upper-right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 2), cond);

    // Display the bottom-right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);
}

// // D
void D(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the rright-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 6), cond);
}

// // E
void E(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

}

// // F
void F(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 7), cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);   
}

// G
void G(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the right-middle block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 5), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the bottom-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

    // Display the top-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 2), cond);
}

// H
void H(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 7), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 7), cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);
}

// I
void I(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the middle-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + fontSize, yStart + (fontSize * 6), cond);
}

// J
void J(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 3), yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 6), cond);
}

// K
void K(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 7), cond);

    // Display the middle block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the middle-top block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle-bottom block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the top-penultimate block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 1), yStart + (fontSize * 2), cond);

    // Display the bottom-penultimate block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

    // Display the top block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + fontSize, cond);

    // Display the bottom block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);
}

// L
void L(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 7), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 5), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);
}

// M
void M(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 7), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 7), cond);

    // Display the middle-left block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the middle-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);
}

// N
void N(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 7), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 7), cond);

    // Display the middle-left block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the middle-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);
}

// O
void O(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + (fontSize * 1), cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);
}

// P
void P(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 7), cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 5), cond);
}

// Q
void Q(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + (fontSize * 1), cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 5), cond);

    // Display the bottom-horizontal block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 3), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the bottom-right-1 block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

    // Display the bottom-right-2 block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);
}

// R
void R(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 7), cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the right-upper block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 4), cond);

    // Display the right-bottom-1 block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

    // Display the right-bottom-2 block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);
}

// S
void S(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the upper-left-vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the bottom-right-vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 4), yStart + (fontSize * 6), cond);

    // Display the bottom-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

    // Display the top-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 2), cond);
}

// T
void T(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
   // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart, yStart + fontSize, cond);

    // Display the middle-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + fontSize, yStart + (fontSize * 7), cond);
}

// U
void U(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 6), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);
}

// V
void V(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 5), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 5), cond);

    // Display the left-bottom block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

    // Display the middle-bottom block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the right-bottom block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);
}

// W
void W(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the vertical-left line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 7), cond);

    // Display the vertical-right line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 7), cond);

    // Display the left-bottom block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the middle block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the right-bottom block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);
}

// X
void X(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the vertical-top-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 2), cond);

    // Display the vertical-bottom-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 5), yStart + (fontSize * 7), cond);

    // Display the vertical-top-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 2), cond);

    // Display the vertical-bottom-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 5), yStart + (fontSize * 7), cond);

    // Display the middle block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the middle-top-left block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle-bottom-left block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the middle-top-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle-bottom-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);
}

// Y
void Y(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart, yStart + (fontSize * 2), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 2), cond);

    // Display the middle-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 7), cond);

    // Display the middle-left block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);
}

// Z
void Z(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart, yStart + fontSize, cond);

    // Display the top-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 2), cond);

    // Display the middle-right-1 block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle-right-2 block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the middle-left block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the bottom-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

    // Display the horizontal-bottom line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

}

// Numbers

// 0
void zero(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the right-block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle-block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the left-block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 1), xStart + (fontSize * 2), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);
}

// 1
void one(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the middle-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart, yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the top-left block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + fontSize, yStart + (fontSize * 2), cond);
}

// 2
void two(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the horizontal-bottom line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the vertical-top-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the vertical-top-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the middle block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 4), yStart + (fontSize * 5), cond);

    // Display the middle-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the middle-left block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);
}

// 3
void three(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 4), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the right-top-vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the right-bottom-vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 4), yStart + (fontSize * 6), cond);

    // Display the top-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 2), cond);

    // Display the bottom-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 5), yStart + (fontSize * 6), cond);
}

// 4
void four(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
   // Display the vertical-right line
   displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart, yStart + (fontSize * 7), cond);

   // Display the horizontal-bottom line
   displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 4), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

   // Display the top-right block
   displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + fontSize, yStart + (fontSize * 2), cond);

   // Display the middle-right block
   displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

   // Display the middle-left block
   displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 2), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

   // Display the bottom-left block
   displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 4), yStart + (fontSize * 5), cond);
}

// 5
void five(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart, yStart + fontSize, cond);

    // Display the horizontal-middle line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the upper-vertical-left line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the bottom-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 5), yStart + (fontSize * 6), cond);

    // Display the vertical-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 3), yStart + (fontSize * 6), cond);

    // Display the bottom-horizontal block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);
}

// 6
void six(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the middle-horizontal block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the bottom-horizontal block
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the left-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the bottom-right-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 4), yStart + (fontSize * 6), cond);

    // Display the top-right block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 2), cond);
}

// 7
void seven(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart, yStart + fontSize, cond);

    // Display the vertical-right line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 2), cond);

    // Display the diagonal-block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 3), xStart + (fontSize * 4), yStart + (fontSize * 2), yStart + (fontSize * 3), cond);

    // Display the middle-vertical line
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 3), yStart + (fontSize * 7), cond);
}

// 8
void eight(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the top-left vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the bottom-left-vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 4), yStart + (fontSize * 6), cond);

    // Display the top-right-vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the bottom-right-vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + (fontSize * 4), yStart + (fontSize * 6), cond);
}

// 9
void nine(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart, yStart + fontSize, cond);

    // Display the middle-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 3), yStart + (fontSize * 4), cond);

    // Display the bottom-horizontal line
    displayRectangle(spiFileDesc, cmd, color, xStart + fontSize, xStart + (fontSize * 4), yStart + (fontSize * 6), yStart + (fontSize * 7), cond);

    // Display the right-vertical block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 4), xStart + (fontSize * 5), yStart + fontSize, yStart + (fontSize * 6), cond);

    // Display the top-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + fontSize, yStart + (fontSize * 3), cond);

    // Display the bottom-left block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + fontSize, yStart + (fontSize * 5), yStart + (fontSize * 6), cond);
}

// Special characters

// Semicolon (:)
void semicolon(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display the top block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + fontSize, yStart + (fontSize * 2), cond);

    // Display the bottom block
    displayRectangle(spiFileDesc, cmd, color, xStart + (fontSize * 2), xStart + (fontSize * 3), yStart + (fontSize * 5), yStart + (fontSize * 6), cond);
}

// Space
void space(int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    // Display block
    displayRectangle(spiFileDesc, cmd, color, xStart, xStart + (fontSize * 5), yStart, yStart + (fontSize * 7), cond);
}

// Display all characters
void displayAllCharacters(int spiFileDesc, uint8_t cmd, uint8_t color, uint8_t spaceColor, int fontSize, int xStart, int yStart, int charDelay, int spaceDelay, bool cond)
{
    A(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    B(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
    
    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    C(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    D(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    E(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    F(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    G(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    H(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    I(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    J(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    K(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    L(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    M(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    N(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    O(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    P(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    Q(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    R(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    S(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    T(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    U(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    V(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    W(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    X(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    Y(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    Z(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    zero(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    one(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    two(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    three(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    four(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    five(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    six(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    seven(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    eight(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    nine(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    sleepForMs(charDelay);

    // Erase the previous character
    space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart, cond);

    sleepForMs(spaceDelay);

    semicolon(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);

    // sleepForMs(500);

    // // Erase the previous character
    // space(spiFileDesc, cmd, spaceColor, fontSize, xStart, yStart);

    // sleepForMs(50);


}