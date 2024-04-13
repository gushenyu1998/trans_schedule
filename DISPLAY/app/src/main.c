// Main program to build the application
// Has main(); does initialization and cleanup and perhaps some basic logic.

#include <stdio.h>
#include <stdbool.h>
#include "displaylogic.h"
#include "alphabet.h"
#include "badmath.h"
#include "hal/button.h"

// Assume pins already configured for SPI
// E.g. for SPI1, CS0:
// (bbg)$ config-pin P9_28 spi_cs
// (bbg)$ config-pin P9_31 spi_sclk
// (bbg)$ config-pin P9_29 spi
// (bbg)$ config-pin P9_30 spi
// P9_48 == D/C
// P9_60 == RST
#define SPI_DEV_BUS0_CS0 "/dev/spidev0.0"
#define SPI_DEV_BUS1_CS0 "/dev/spidev1.0"
#define SPI_DEV_BUS1_CS1 "/dev/spidev1.1"
#define SPI_MODE_DEFAULT 0
#define SPEED_HZ_DEFAULT 10000000 // Arbitrary, but 500000 is reasonable

#define MAX_BYTE_VALUE 255

#define Y_DISPLAY_MAX_LOW 0x00
#define Y_DISPLAY_MAX_HIGH 0xEF

#define X_DISPLAY_MAX_HIGH 0x01
#define X_DISPLAY_MAX_LOW 0x3F

#define Y_DISPLAY_DIMENSION 360
#define X_DISPLAY_DIMENSION 480

#define MAX_VAL_16_BITS 65535

#define FONT_SIZE_3_START_Y 3
#define FONT_SIZE_3_START_X 3
#define FONT_SIZE_3_INCREMENTOR 18

#define FONT_SIZE_2 2
#define FONT_SIZE_2_START_Y 25
#define FONT_SIZE_2_START_X 2
#define FONT_SIZE_2_Y_INCREMENTOR 20

#define SECOND_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR* 1))  
#define THIRD_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 2))
#define FOURTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 3))
#define FIFTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 4))
#define SIXTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 5))
#define SEVENTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 6))
#define EIGHTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 7))

int main()
{
    // For random
    srand(time(NULL));

    displaylogic_init();

    // Cleanup
    displaylogic_cleanup();
    shutdown_all_pins();
    // close(spiFileDesc);

    return 0;


}
