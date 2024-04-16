// Source code for displaylogic.c
// This file contains the implementation of the displaylogic thread
// to control the display.

#include "displaylogic.h"
#include "hal/threadsControl.h"
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
// #define SPEED_HZ_DEFAULT 500000 // Arbitrary, but 500000 is reasonable
#define SPEED_HZ_DEFAULT 10000000 // Arbitrary, but 500000 is reasonable

#define MAX_BYTE_VALUE 255

#define Y_DISPLAY_MAX_LOW 0x00
#define Y_DISPLAY_MAX_HIGH 0xEF

#define X_DISPLAY_MAX_HIGH 0x01
#define X_DISPLAY_MAX_LOW 0x3F

// #define Y_DISPLAY_DIMENSION 360
// #define X_DISPLAY_DIMENSION 480
#define Y_DISPLAY_DIMENSION 240
#define X_DISPLAY_DIMENSION 320

#define MAX_VAL_16_BITS 65535

#define FONT_SIZE_3_START_Y 3
#define FONT_SIZE_3_START_X 3
#define FONT_SIZE_3_INCREMENTOR 18

#define FONT_SIZE_2 2
#define FONT_SIZE_2_START_Y 25
#define FONT_SIZE_2_START_X 2
#define FONT_SIZE_2_INCREMENTOR 12
#define FONT_SIZE_2_Y_INCREMENTOR 20

#define SECOND_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR* 1))  
#define THIRD_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 2))
#define FOURTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 3))
#define FIFTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 4))
#define SIXTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 5))
#define SEVENTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 6))
#define EIGHTH_LINE_Y (FONT_SIZE_2_START_Y + (FONT_SIZE_2_Y_INCREMENTOR * 7))

#define BLACK_COLOR 0x00
#define WHITE_COLOR 0xFF

// Global variables
static pthread_t tid_displaylogic;
static bool continueThread = true; // Can be modified by other modules

static bool flagScreenDisplay = false; // Can be modified by other modules
static bool flagNFCOrUpdate =true; // false = NFC, true = Update

// Strings for Bus info
char firstLine[50];
char secondsLine[50];
char thirdLine[50];

// strings for time
char firstTime[6];
char secondTime[6];
char thirdTime[6];

static char *locations[3] = {firstLine, secondsLine, thirdLine};
static char *times[3] = {firstTime, secondTime, thirdTime};

void *thread_displaylogic()
{
    printf("Beginning of displaylogic thread\n");
    int spiFileDesc; // File descriptor for SPI device
    uint8_t cmd; // Variable to hold command byte
    // uint8_t params[4]; // Array to hold command parameters if any

    // Initialize all GPIO pins
    init_all_pins();

    printf("Suspect\n");
    // Initialize SPI port for Display
    spiFileDesc = SPI_initPort(SPI_DEV_BUS1_CS0); // Change SPI_DEV_BUS1_CS0 as needed
    printf("Suspect passed\n");

    // 1) Hardware reset
    hardwareReset();

    // 2) Turn off sleep mode by sending the "sleep OUT" command and then waiting 60 ms for the display to wake up
    cmd = 0x11; // Sleep OUT command 
    Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command
    sleepForMs(60); // Wait for the display to wake up

    // 3) Sending the "display ON" command.
    cmd = 0x29; // Display ON command
    Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command

    // 4) Make sure that everything is working as expected
    cmd = 0x0A; // Read Display Power Mode command
    // uint8_t powerMode = Display_read8Command(spiFileDesc, cmd);
    // printf("Display power mode: 0x%02X\n", powerMode); // Should be 0x9C if the display is active

    // Initialize the display by setting up a white background
    displayRectangle(spiFileDesc, cmd, 0xFF, 0, 319, 0, 239, true);

    bool statusUpdateScreen = false;
    while(!ifShutdown()) {
        // Start of critical section (READ)
        lockDisplayMutex();
        statusUpdateScreen = getFlagScreenDisplay();

        // Check if we need to update the screen
        if(statusUpdateScreen == true) {
            printf("%s\n",locations[0]);
            printf("%s\n",locations[1]);
            printf("%s\n",locations[2]);

            // Clear the screen by setting a white background
            displayRectangle(spiFileDesc, cmd, 0xFF, 0, X_DISPLAY_DIMENSION - 1, 0, Y_DISPLAY_DIMENSION - 1, true);
            // 1) Update the first line
            firstLineNewText(locations[0], spiFileDesc, cmd);

            // 2) Update the second line
            secondLineNewText(times[0], spiFileDesc, cmd);

            // 3) Update the third line
            space(spiFileDesc, cmd, WHITE_COLOR, FONT_SIZE_2, FONT_SIZE_2_START_X, THIRD_LINE_Y, false);

            // 4) Update the fourth line
            fourthLineNewText(locations[1], spiFileDesc, cmd);

            // 5) Update the fifth line
            fifthLineNewText(times[1], spiFileDesc, cmd);

            // 6) Update the sixth line
            space(spiFileDesc, cmd, WHITE_COLOR, FONT_SIZE_2, FONT_SIZE_2_START_X, SIXTH_LINE_Y, false);

            // 7) Update the seventh line
            seventhLineNewText(locations[2], spiFileDesc, cmd);

            // 8) Update the eighth line
            eighthLineNewText(times[2], spiFileDesc, cmd);

            // Once we have finished updating the screen, set updateScreenRequest back to false
            // Start of critical section (WRITE)
            // pthread_mutex_lock(&mutex_displaylogic);
            // {
            //     updateScreenRequest = false;
            // }
            // // End of critical section
            // pthread_mutex_unlock(&mutex_displaylogic);

            setFlagScreenDisplay(false);
        }
        unlockDisplayMutex();
    }

    // Set the display to a white background
    displayRectangle(spiFileDesc, cmd, 0xFF, 0, 319, 0, 239, true);

    close(spiFileDesc);
    writeToFile("/sys/class/gpio/gpio113/value", "1");

    return NULL;
}

int randomIntFrom0To2(void)
{
    int res = rand() % 3;
    return res;
}

void firstLineNewText(const char* text, int spiFileDesc, uint8_t cmd)
{
    displayString(text, spiFileDesc, cmd, BLACK_COLOR, FONT_SIZE_2, FONT_SIZE_2_START_X, FONT_SIZE_2_START_Y, false);
}

void secondLineNewText(const char* text, int spiFileDesc, uint8_t cmd)
{
    displayString(text, spiFileDesc, cmd, BLACK_COLOR, FONT_SIZE_2, FONT_SIZE_2_START_X, SECOND_LINE_Y, false);
}

void fourthLineNewText(const char* text, int spiFileDesc, uint8_t cmd)
{
    displayString(text, spiFileDesc, cmd, BLACK_COLOR, FONT_SIZE_2, FONT_SIZE_2_START_X, FOURTH_LINE_Y, false);
}

void fifthLineNewText(const char* text, int spiFileDesc, uint8_t cmd)
{
    displayString(text, spiFileDesc, cmd, BLACK_COLOR, FONT_SIZE_2, FONT_SIZE_2_START_X, FIFTH_LINE_Y, false);
}

void seventhLineNewText(const char* text, int spiFileDesc, uint8_t cmd)
{
    displayString(text, spiFileDesc, cmd, BLACK_COLOR, FONT_SIZE_2, FONT_SIZE_2_START_X, SEVENTH_LINE_Y, false);
}

void eighthLineNewText(const char* text, int spiFileDesc, uint8_t cmd)
{
    displayString(text, spiFileDesc, cmd, BLACK_COLOR, FONT_SIZE_2, FONT_SIZE_2_START_X, EIGHTH_LINE_Y, false);
}


// Get the status of the thread (true means continue thread, false means stop thread)
bool getThreadStatus(void) {
    return continueThread;
}

// Client requests to stop the thread
void stopThread(void) {
    continueThread = false;
}

void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void init_all_pins(void)
{
    // CS
    // runCommand("cd && cd /sys/class/gpio/gpio113 && echo out > direction");
    writeToFile("/sys/class/gpio/gpio113/direction", "out");

    // SCLK
    // runCommand("cd && cd /sys/class/gpio/gpio110 && echo out > direction");
    writeToFile("/sys/class/gpio/gpio110/direction", "out");

    // MOSI
    // runCommand("cd && cd /sys/class/gpio/gpio112 && echo out > direction");
    writeToFile("/sys/class/gpio/gpio112/direction", "out");

    // MISO
    // runCommand("cd && cd /sys/class/gpio/gpio111 && echo out > direction");
    writeToFile("/sys/class/gpio/gpio111/direction", "out");

    // D/C
    runCommand("cd && cd /sys/class/gpio/gpio48 && config-pin p9.15 gpio && echo out > direction");

    // RST
    runCommand("cd && cd /sys/class/gpio/gpio60 && config-pin p9.12 gpio && echo out > direction");
}

void shutdown_all_pins(void)
{
    // CS
    // runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value && echo in > direction");
    writeToFile("/sys/class/gpio/gpio113/value", "0");
    writeToFile("/sys/class/gpio/gpio113/direction", "in");

    // SCLK
    // runCommand("cd && cd /sys/class/gpio/gpio110 && echo 0 > value && echo in > direction");
    writeToFile("/sys/class/gpio/gpio110/value", "0");
    writeToFile("/sys/class/gpio/gpio110/direction", "in");

    // MOSI
    // runCommand("cd && cd /sys/class/gpio/gpio112 && echo 0 > value && echo in > direction");
    writeToFile("/sys/class/gpio/gpio112/value", "0");
    writeToFile("/sys/class/gpio/gpio112/direction", "in");

    // MISO
    // runCommand("cd && cd /sys/class/gpio/gpio111 && echo 0 > value && echo in > direction");
    writeToFile("/sys/class/gpio/gpio111/value", "0");
    writeToFile("/sys/class/gpio/gpio111/direction", "in");

    // D/C
    // writeToFile("/sys/class/gpio/gpio48/ocp:Pk9_15_pinmux/state", "default");
    writeToFile("/sys/class/gpio/gpio48/state", "default");
    writeToFile("/sys/class/gpio/gpio48/value", "1");
    writeToFile("/sys/class/gpio/gpio48/direction", "in");

    // runCommand("cd && cd /sys/class/gpio/gpio48 && config-pin p9.15 default && echo 1 > value && echo in > direction");

    // RST
    // runCommand("cd && cd /sys/class/gpio/gpio60 && config-pin p9.12 default && echo 1 > value && echo in > direction");
    writeToFile("/sys/class/gpio/gpio60/state", "default");
    writeToFile("/sys/class/gpio/gpio60/value", "1");
    writeToFile("/sys/class/gpio/gpio60/direction", "in");
}

int SPI_initPort(char* spiDevice)
{
    // Open Device
    int spiFileDesc = open(spiDevice, O_RDWR);
    if (spiFileDesc < 0) {
        printf("Error: Can't open device %s\n", spiDevice);
        exit(1);
    }

    // Set port parameters

    // Set SPI mode: Necessary
    int spiMode = SPI_MODE_DEFAULT;
    int errorCheck = ioctl(spiFileDesc, SPI_IOC_WR_MODE, &spiMode);
    if (errorCheck < 0) {
        printf("Error: Set SPI mode failed\n");
        exit(1);
    }

    int speedHz = SPEED_HZ_DEFAULT;
    errorCheck = ioctl(spiFileDesc, SPI_IOC_WR_MAX_SPEED_HZ, &speedHz);
    if (errorCheck < 0) {
        printf("Error: Set max speed failed\n");
        exit(1);
    }

    return spiFileDesc;
}

void SPI_transfer(int spiFileDesc, uint8_t *sendBuf, uint8_t *receiveBuf, int length)
{
    // Setting transfer this way ensures all other fields are 0
    struct spi_ioc_transfer transfer = {
        .tx_buf = (unsigned long) sendBuf,
        .rx_buf = (unsigned long) receiveBuf,
        .len = length
    };

    const int NUM_TRANSFERS = 1;
    int status = ioctl(spiFileDesc, SPI_IOC_MESSAGE(NUM_TRANSFERS), &transfer);
    if (status < 0) {
        printf("Error: SPI Transfer failed\n");
    }
}

// according to 4-line serial interface timing diagram on page 35 of the datasheet
void Display_writeCommand(int spiFileDesc, uint8_t command, uint8_t *params, size_t param_count) {
    // ! set the CS to low through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value");
    writeToFile("/sys/class/gpio/gpio113/value", "0");

    // ! set the D/C to low through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio48 && echo 0 > value");
    writeToFile("/sys/class/gpio/gpio48/value", "0");

    SPI_transfer(spiFileDesc, &command, NULL, 1);

    // ! set the D/C to high through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio48 && echo 1 > value");
    writeToFile("/sys/class/gpio/gpio48/value", "1");

    SPI_transfer(spiFileDesc, params, NULL, param_count);

    // ! set the CS to high through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio113 && echo 1 > value");
    writeToFile("/sys/class/gpio/gpio113/value", "1");
}

// according to the 4-line serial interface "8-bit read" timing diagram on page 38 of the datasheet
uint8_t Display_read8Command(int spiFileDesc, uint8_t command) {
    // ! set the CS to low through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value");
    writeToFile("/sys/class/gpio/gpio113/value", "0");

    // ! set the D/C to low through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio48 && echo 0 > value");
    writeToFile("/sys/class/gpio/gpio48/value", "0");

    SPI_transfer(spiFileDesc, &command, NULL, 1);

    // ! set the D/C to high through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio48 && echo 1 > value");
    writeToFile("/sys/class/gpio/gpio48/value", "1");

    uint8_t readVal;
    SPI_transfer(spiFileDesc, NULL, &readVal, 1);

    // ! set the CS to high through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio113 && echo 1 > value");
    writeToFile("/sys/class/gpio/gpio113/value", "1");

    return readVal;
}

void hardwareReset(void) {
    // ! wait for 100 ms
    sleepForMs(100);

    // ! set RST to low
    // runCommand("cd && cd /sys/class/gpio/gpio60 && echo 0 > value");
    writeToFile("/sys/class/gpio/gpio60/value", "0");

    // ! wait for 10 ms
    sleepForMs(10);

    // ! set RST to high
    // runCommand("cd && cd /sys/class/gpio/gpio60 && echo 1 > value");
    writeToFile("/sys/class/gpio/gpio60/value", "1");

    // ! wait for 20 ms
    sleepForMs(20);
}

// Helper function. Need client code to free the return value
char *decimalToHexadecimal(int decimal_value)
{
    char *buff = (char *)malloc(20 * sizeof(char));
    snprintf(buff, 20, "%X", decimal_value);
    return buff;
}

// Helper function. Convert a hexadecimal character to uint8_t
uint8_t hexCharToUint8(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return 10 + c - 'A';
    } else if (c >= 'a' && c <= 'f')  {
        return 10 + c - 'a';
    }

    return 0; // Default case if not a valid hex character
}

// Returns the low byte and the high byte of a decimal_value [high byte, low byte]
// Client code needs to free the return value
uint8_t *highAndLowBytesFromDecimal(int decimal_value)
{
    if (decimal_value > MAX_VAL_16_BITS) {
        printf("ERROR: decimal_value entered is greater than %d\n", MAX_VAL_16_BITS);
        // return;
        exit(EXIT_FAILURE);
    }

    char *hex_value = decimalToHexadecimal(decimal_value);
    // e.g., hex_value = 0xFBA5

    int length = strlen(hex_value);

    uint8_t *res = (uint8_t *)malloc(2 * sizeof(uint8_t));

    // e.g. length: 0xF, 0xFF, 0xFFF, 0xFFFF

    uint8_t temp0 = 0;
    uint8_t temp1 = 0;
    uint8_t temp2 = 0;

    switch(length)
    {
        // e.g. F
        case 1:
            // High byte (0)
            res[0] = 0x00;

            // Low byte (F)
            res[1] = hexCharToUint8(hex_value[0]);
            break;
        // e.g. FF
        case 2:
            // High byte (0)
            res[0] = 0x00;

            // Low byte (FF)
            temp0 = hexCharToUint8(hex_value[0]);
            res[1] = 0;
            res[1] += (temp0 * 16);
            temp1 = hexCharToUint8(hex_value[1]);
            res[1] += temp1;
            break;
        // e.g. FFF
        case 3:
            // High byte (F)
            res[0] = hexCharToUint8(hex_value[0]);

            // Low byte (FF)
            res[1] = 0;
            temp0 = hexCharToUint8(hex_value[1]);
            res[1] += (temp0 * 16);
            temp1 = hexCharToUint8(hex_value[2]);
            res[1] += temp1;
            break;
        // e.g. FFFF
        case 4:
            // High byte (FF)
            res[0] = 0;
            temp0 = hexCharToUint8(hex_value[0]);
            res[0] += (temp0 * 16);
            temp1 = hexCharToUint8(hex_value[1]);
            res[0] += temp1;

            // Low byte (FF)
            res[1] = 0;
            temp2 = hexCharToUint8(hex_value[2]);
            res[1] += (temp2 * 16);
            temp2 = hexCharToUint8(hex_value[3]);
            res[1] += temp2;
            break;
        default:
            assert(false);
    }

    // Must free values called from decimalToHexadecimal()
    free(hex_value);

    return res;
}

void displayRectangle(int spiFileDesc, uint8_t cmd, uint8_t color, int xStart, int xEnd, int yStart, int yEnd, bool cond)
{
    // 5. Specify where you want to draw on the display using "column address set" and "page address set" commands
    // For example, setting column range (Refer to datasheet for the correct parameters)

    // xStart, yStart, xEnd, yEnd
    // Column
    uint8_t *yStartBytes = highAndLowBytesFromDecimal(yStart);
    uint8_t *yEndBytes = highAndLowBytesFromDecimal(yEnd);

    // Page
    uint8_t *xStartBytes = highAndLowBytesFromDecimal(xStart);
    uint8_t *xEndBytes = highAndLowBytesFromDecimal(xEnd);

    uint8_t params[4];

    params[0] = yStartBytes[0]; // Start column high byte
    params[1] = yStartBytes[1]; // Start column low byte 

    params[2] = yEndBytes[0]; // End column high byte
    params[3] = yEndBytes[1]; // End column and low byte (239)

    cmd = 0x2A; // Column address set command
    Display_writeCommand(spiFileDesc, cmd, params, 4);

    // Setting page range (Refer to datasheet for the correct parameters)
    params[0] = xStartBytes[0]; // Start page high byte
    params[1] = xStartBytes[1]; // Start page low byte

    params[2] = xEndBytes[0]; // End page high byte
    params[3] = xEndBytes[1]; // End page low byte (319)

    cmd = 0x2B; // Page address set command
    Display_writeCommand(spiFileDesc, cmd, params, 4);

    // 6. Send the "memory write" command to start writing pixels
    cmd = 0x2C; // Memory write command
    Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command

    free(yStartBytes);
    free(yEndBytes);
    free(xStartBytes);
    free(xEndBytes);

    // 7 & 8. Now, send pixel data. Implement your function to send pixel colors as needed.
    // This is highly dependent on your specific requirements and the display's capabilities.

    // ! set the CS to low through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value");
    writeToFile("/sys/class/gpio/gpio113/value", "0");

    uint8_t colorData[4096];

    for (int i = 0; i < 4096; i++) {
        colorData[i] = color;
    }

    // Send pixel data
    if(cond == true) {
        for (int i = 0; i < 60; i++) {
            SPI_transfer(spiFileDesc, colorData, NULL, sizeof(colorData));
        }
    } else {
        SPI_transfer(spiFileDesc, colorData, NULL, sizeof(colorData));
    }

    // ! set the CS to high through GPIO
    // runCommand("cd && cd /sys/class/gpio/gpio113 && echo 1 > value");
}

void displayString(const char *text, int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    int stringLength = strlen(text);

    int xIndex = xStart;

    for(int i = 0; i < stringLength; i++) {
        charToDisplay(text[i], spiFileDesc, cmd, color, fontSize, xIndex, yStart, cond);
        xIndex += FONT_SIZE_2_INCREMENTOR;
    }
}

void charToDisplay(char currentChar, int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond)
{
    switch(currentChar) {
        // 'A' or 'a'
        case 'A':
            A(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'a':
            A(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'B' or 'a'
        case 'B':
            B(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'b':
            B(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'C' or 'c'
        case 'C':
            C(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'c':
            C(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'D' or 'd'
        case 'D':
            D(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'd':
            D(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'E' or 'e'
        case 'E':
            E(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);   
            break;
        case 'e':
            E(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'F' or 'f'
        case 'F':
            F(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'f':
            F(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'G' or 'g'
        case 'G':
            G(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'g':
            G(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'H' or 'h'
        case 'H':
            H(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'h':
            H(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'I' or 'i'
        case 'I':
            I(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'i':
            I(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'J' or 'j'
        case 'J':
            J(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'j':
            J(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'K' or 'k'
        case 'K':
            K(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'k':
            K(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'L' or 'l'
        case 'L':
            L(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'l':
            L(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'M' or 'm'
        case 'M':
            M(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'm':
            M(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'N' or 'n'
        case 'N':
            N(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'n':
            N(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'O' or 'o'
        case 'O':
            O(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'o':
            O(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'P' or 'p'
        case 'P':
            P(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'p':
            P(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'Q' or 'q'
        case 'Q':
            Q(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'q':
            Q(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'R' or 'r'
        case 'R':
            R(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'r':
            R(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'S' or 's'
        case 'S':
            S(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 's':
            S(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'T' or 't'
        case 'T':
            T(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 't':
            T(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'U' or 'u'
        case 'U':
            U(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'u':
            U(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'V' or 'v'
        case 'V':
            V(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'v':
            V(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'W' or 'w'
        case 'W':
            W(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'w':
            W(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'X' or 'x'
        case 'X':
            X(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'x':
            X(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'Y' or 'y'
        case 'Y':
            Y(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'y':
            Y(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // 'Z' or 'z'
        case 'Z':
            Z(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        case 'z':
            Z(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '0' 
        case '0':
            zero(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '1' 
        case '1':
            one(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '2'
        case '2':
            two(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '3' 
        case '3':
            three(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '4' 
        case '4':
            four(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '5' 
        case '5':
            five(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '6'
        case '6':
            six(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '7' 
        case '7':
            seven(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '8' 
        case '8':
            eight(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // '9' 
        case '9':
            nine(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // ':' 
        case ':':
            semicolon(spiFileDesc, cmd, color, fontSize, xStart, yStart, cond);
            break;
        // ' ' 
        case ' ':
            space(spiFileDesc, cmd, 0xFF, fontSize, xStart, yStart, cond); // Assumes that the background color is white
            break;
        default:
            space(spiFileDesc, cmd, 0xFF, fontSize, xStart, yStart, cond);
    }
}

void displaylogic_init(void)
{
    // int spiFileDesc; // File descriptor for SPI device
    // uint8_t cmd; // Variable to hold command byte
    // // uint8_t params[4]; // Array to hold command parameters if any

    // // Initialize all GPIO pins
    // init_all_pins();

    // // Initialize SPI port for Display
    // spiFileDesc = SPI_initPort(SPI_DEV_BUS1_CS0); // Change SPI_DEV_BUS1_CS0 as needed

    // // 1. Hardware reset
    // hardwareReset();

    // // 2. Turn off sleep mode by sending the "sleep OUT" command and then waiting 60 ms for the display to wake up
    // cmd = 0x11; // Sleep OUT command 
    // Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command
    // sleepForMs(60); // Wait for the display to wake up

    // // 3. Sending the "display ON" command.
    // cmd = 0x29; // Display ON command
    // Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command

    // // 4. Make sure that everything is working as expected
    // cmd = 0x0A; // Read Display Power Mode command
    // // uint8_t powerMode = Display_read8Command(spiFileDesc, cmd);
    // // printf("Display power mode: 0x%02X\n", powerMode); // Should be 0x9C if the display is active

    // // Initialize the display by setting up a white background
    // displayRectangle(spiFileDesc, cmd, 0xFF, 0, 319, 0, 239, true);

    // Initialize thread

    // Create attributes
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Create the thread
    pthread_create(&tid_displaylogic, &attr, thread_displaylogic, NULL);
}

void displaylogic_cleanup(void)
{
    // Join the thread
    pthread_join(tid_displaylogic, NULL);
}

char ** getLocationBuffer(void)
{
    return  locations;
}

char ** getTimeBuffer(void)
{
    return times;
}

bool getFlagScreenDisplay(void)
{
    return flagScreenDisplay;
}

void setFlagScreenDisplay(bool flag)
{
    flagScreenDisplay = flag;
}

bool getNFCOrUpdate(void)
{
    return flagNFCOrUpdate;
}

void setNFCOrUpdate(bool flag)
{
    flagNFCOrUpdate = flag;
}