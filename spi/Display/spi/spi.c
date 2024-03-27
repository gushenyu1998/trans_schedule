#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

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
#define SPEED_HZ_DEFAULT 500000 // Arbitrary, but 500000 is reasonable

#define MAX_BYTE_VALUE 255

#define Y_DISPLAY_MAX_LOW 0x00
#define Y_DISPLAY_MAX_HIGH 0xEF

#define X_DISPLAY_MAX_HIGH 0x01
#define X_DISPLAY_MAX_LOW 0x3F

#define Y_DISPLAY_DIMENSION 360
#define X_DISPLAY_DIMENSION 480

#define MAX_VAL_16_BITS 65535

void runCommand(char* command)
{
    // Execute the shell command (output into pipe)static pthread_t tid_analyzer; // Thread ID
    FILE *pipe = popen(command, "r");

    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }

    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
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
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo out > direction");

    // SCLK
    runCommand("cd && cd /sys/class/gpio/gpio110 && echo out > direction");

    // MOSI
    runCommand("cd && cd /sys/class/gpio/gpio112 && echo out > direction");

    // MISO
    runCommand("cd && cd /sys/class/gpio/gpio111 && echo out > direction");

    // D/C
    runCommand("cd && cd /sys/class/gpio/gpio48 && config-pin p9.15 gpio && echo out > direction");

    // RST
    runCommand("cd && cd /sys/class/gpio/gpio60 && config-pin p9.12 gpio && echo out > direction");
}

void shutdown_all_pins(void)
{
    // CS
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value && echo in > direction");

    // SCLK
    runCommand("cd && cd /sys/class/gpio/gpio110 && echo 0 > value && echo in > direction");

    // MOSI
    runCommand("cd && cd /sys/class/gpio/gpio112 && echo 0 > value && echo in > direction");

    // MISO
    runCommand("cd && cd /sys/class/gpio/gpio111 && echo 0 > value && echo in > direction");

    // D/C
    runCommand("cd && cd /sys/class/gpio/gpio48 && config-pin p9.15 default && echo 1 > value && echo in > direction");

    // RST
    runCommand("cd && cd /sys/class/gpio/gpio60 && config-pin p9.12 default && echo 1 > value && echo in > direction");
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
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value");

    // ! set the D/C to low through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio48 && echo 0 > value");

    SPI_transfer(spiFileDesc, &command, NULL, 1);

    // ! set the D/C to high through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio48 && echo 1 > value");

    SPI_transfer(spiFileDesc, params, NULL, param_count);

    // ! set the CS to high through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 1 > value");
}

// according to the 4-line serial interface "8-bit read" timing diagram on page 38 of the datasheet
uint8_t Display_read8Command(int spiFileDesc, uint8_t command) {
    // ! set the CS to low through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value");

    // ! set the D/C to low through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio48 && echo 0 > value");

    SPI_transfer(spiFileDesc, &command, NULL, 1);

    // ! set the D/C to high through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio48 && echo 1 > value");

    uint8_t readVal;
    SPI_transfer(spiFileDesc, NULL, &readVal, 1);

    // ! set the CS to high through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 1 > value");

    return readVal;
}

static void hardwareReset() {
    // ! wait for 100 ms
    sleepForMs(100);

    // ! set RST to low
    runCommand("cd && cd /sys/class/gpio/gpio60 && echo 0 > value");

    // ! wait for 10 ms
    sleepForMs(10);

    // ! set RST to high
    runCommand("cd && cd /sys/class/gpio/gpio60 && echo 1 > value");

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

// Returns the low byte and the high byte
// Client code needs to free the return value
uint8_t *lowAndHighBytes(int decimal_value)
{
    if (decimal_value > MAX_VAL_16_BITS) {
        printf("ERROR: decimal_value entered is greater than %d\n", MAX_VAL_16_BITS);
        return;
    }

    char *hex_value = decimalToHexadecimal(decimal_value);

    int length = strlen(hex_value);

    uint8_t *res = (uint8_t *)malloc(2 * sizeof(uint8_t));

    switch(length)
    {
        case 1:
            res[0] = 0x00;
            res[1] = (uint8_t)strtol(hex_value, NULL, 16);
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
    }

    free(hex_value);

    return res;
}

void displayVerticalLine(int spiFileDesc, int height, uint8_t startColLow, uint8_t startPageLow, uint8_t startColHigh, uint8_t startPageHigh)
{
    // Error handling
    if (startColLow > MAX_BYTE_VALUE) {
        printf("ERROR: Entered a start column low byte value greater than %d\n", MAX_BYTE_VALUE);
    }

    if (startPageLow > MAX_BYTE_VALUE) {
        printf("ERROR: Entered a start page low byte value greater than %d\n", MAX_BYTE_VALUE);
    }

    if (startColHigh > MAX_BYTE_VALUE) {
        printf("ERROR: Entered a start column high byte value greater than %d\n", MAX_BYTE_VALUE);
    }

    if (startPageHigh > MAX_BYTE_VALUE) {
        printf("ERROR: Entered a start page high byte value greater than %d\n", MAX_BYTE_VALUE);
    }

    if (height > Y_DISPLAY_DIMENSION) {
        printf("ERROR: Entered a height value greater than %d\n", Y_DISPLAY_DIMENSION);
    }

    uint8_t cmd; // Variable to hold command byte0x3F
    uint8_t params[4]; // Array to hold command parameters if any

    // Specify where you want to draw on the display using "column address set" and "page address set" commands
    params[0] = startColHigh; // Start column high 
    params[1] = startColLow; // Start column low byte (0)

    params[2] = Y_DISPLAY_MAX_HIGH; // End column high byte
    params[3] = Y_DISPLAY_MAX_LOW; // End column and low byte (239) 0xEF

    cmd = 0x2A; // Column address set command
    Display_writeCommand(spiFileDesc, cmd, params, 4);

    // Setting page range (Refer to datasheet for the correct parameters)
    params[0] = startColHigh; // Start page high byte
    params[1] = startPageLow; // Start page low byte

    params[2] = X_DISPLAY_MAX_HIGH; // End page high byte
    params[3] = X_DISPLAY_MAX_LOW; // End page low byte (319)

    cmd = 0x2B; // Page address set command
    Display_writeCommand(spiFileDesc, cmd, params, 4);

    // Send the "memory write" command to start writing pixels
    cmd = 0x2C; // Memory write command
    Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command

    // Now, send pixel data. 

    // Prepare for SPI transfer: convert to byte array
    uint8_t colorData[2] = {0x00, 0x00}; // High byte, Low byte

    // ! set the CS to low through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value");

    // Display line
    for (int y = 0; y < height; y++) {
        SPI_transfer(spiFileDesc, colorData, NULL, sizeof(colorData));
    }

    // ! set the CS to high through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 1 > value");
}

void displayHorizontalLine(int spiFileDesc)
{
    uint8_t cmd; // Variable to hold command byte
    uint8_t params[4]; // Array to hold command parameters if any

    // Define the color of the line in RGB565 format
    uint8_t colorData[2] = {0x00, 0x00};

    // Define the y-coordinate for the line (middle of the screen)

    // Set column range for the entire width of the display
    // Here we will have a constant y-value
    params[0] = 0x00; // Start page high byte
    params[1] = 0x50; // Start page low byte

    params[2] = 0x00; // End page high byte (same as start)
    params[3] = 0x50; // End page low byte (same as start)

    // Send the "column address set" command
    cmd = 0x2A; 
    Display_writeCommand(spiFileDesc, cmd, params, 4);

    // Set page range
    params[0] = 0x00; // Start column high byte
    params[1] = 0x32; // Start column low byte (0)

    params[2] = 0x00; // End column high byte (for 320, use 0x013F)
    params[3] = 0x64; // End column low byte (319)

    // Send the "page address set" command
    cmd = 0x2B; 
    Display_writeCommand(spiFileDesc, cmd, params, 4);

    // Send the "memory write" command
    cmd = 0x2C; 
    Display_writeCommand(spiFileDesc, cmd, NULL, 0);

    // ! set the CS to low through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value");

    // Draw the line by writing the color data to each column in the defined row
    for (int i = 0; i < X_DISPLAY_DIMENSION; i++) {
        SPI_transfer(spiFileDesc, colorData, NULL, sizeof(colorData));
    }

    // ! set the CS to high through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 1 > value");
}

int main(void) {

    int spiFileDesc; // File descriptor for SPI device
    uint8_t cmd; // Variable to hold command byte
    uint8_t params[4]; // Array to hold command parameters if any

    // Initialize all GPIO pins
    init_all_pins();

    // Initialize SPI port for Display
    spiFileDesc = SPI_initPort(SPI_DEV_BUS1_CS0); // Change SPI_DEV_BUS1_CS0 as needed

    // 1. Hardware reset
    hardwareReset();

    // 2. Turn off sleep mode by sending the "sleep OUT" command and then waiting 60 ms for the display to wake up
    cmd = 0x11; // Sleep OUT command (Refer to ILI9340C datasheet for the correct command byte)
    Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command
    sleepForMs(60); // Wait for the display to wake up

    // 3. Sending the "display ON" command.
    cmd = 0x29; // Display ON command
    Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command

    // 4. Make sure that everything is working as expected
    cmd = 0x0A; // Read Display Power Mode command
    uint8_t powerMode = Display_read8Command(spiFileDesc, cmd);
    printf("Display power mode: 0x%02X\n", powerMode); // Should be 0x9C if the display is active

    // 5. Specify where you want to draw on the display using "column address set" and "page address set" commands
    // For example, setting column range (Refer to datasheet for the correct parameters)
    params[0] = 0x00; // Start column high byte
    params[1] = 0x00; // Start column low byte 

    params[2] = 0x00; // End column high byte
    params[3] = 0xEF; // End column and low byte 

    cmd = 0x2A; // Column address set command
    Display_writeCommand(spiFileDesc, cmd, params, 4);

    // Setting page range (Refer to datasheet for the correct parameters)
    params[0] = 0x00; // Start page high byte
    params[1] = 0x00; // Start page low byte

    params[2] = 0x01; // End page high byte
    params[3] = 0x3F; // End page low byte (319)

    cmd = 0x2B; // Page address set command
    Display_writeCommand(spiFileDesc, cmd, params, 4);

    // 6. Send the "memory write" command to start writing pixels
    cmd = 0x2C; // Memory write command
    Display_writeCommand(spiFileDesc, cmd, NULL, 0); // No additional parameters for this command

    // 7 & 8. Now, send pixel data. Implement your function to send pixel colors as needed.
    // This is highly dependent on your specific requirements and the display's capabilities.

    // ! set the CS to low through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 0 > value");

    // Define color white in RGB565 format for simplicity

    // Prepare for SPI transfer: convert to byte array
    // uint8_t colorData[2] = {0xFF, 0xFF}; // High byte, Low byte
    
    uint8_t colorData[4096];

    for (int i = 0; i < 4096; i++) {
        colorData[i] = 0xFF;
    }

    // Assume 320x240 display, sending white to each pixel
    for (int i = 0; i < 60; i++) {
        SPI_transfer(spiFileDesc, colorData, NULL, sizeof(colorData));
    }

    // SPI_transfer(spiFileDesc, colorData, NULL, sizeof(colorData));

    // ! set the CS to high through GPIO
    runCommand("cd && cd /sys/class/gpio/gpio113 && echo 1 > value");

    // Print HELLO

    // Print H
    // displayVerticalLine(spiFileDesc, 100, 0x32, 0x32);
    displayVerticalLine(spiFileDesc, 100, 50, 50, 0, 0);

    displayHorizontalLine(spiFileDesc);

    // displayVerticalLine(spiFileDesc, 100, 0x32, 0x64);
    displayVerticalLine(spiFileDesc, 100, 50, 100, 0, 0);

    // Start at (50, 50)

    char *res = decimalToHexadecimal(65535);
    printf("res = %s\n", res);

    free(res);

    // printf("(x, y) = (%d, %d)\n", x, y);
    
    // Cleanup
    shutdown_all_pins();
    close(spiFileDesc);

    return 0;
}