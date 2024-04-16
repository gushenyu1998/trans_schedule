// Header file for displaylogic.c
// Thread used to write/refresh text on the LED Display For Different Lines

#ifndef _DISPLAYLOGIC_H_
#define _DISPLAYLOGIC_H_

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
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include "alphabet.h"
#include "hal/readNWrite.h"

int randomIntFrom0To2(void);

void firstLineNewText(const char* text, int spiFileDesc, uint8_t cmd);

void secondLineNewText(const char* text, int spiFileDesc, uint8_t cmd);

void fourthLineNewText(const char* text, int spiFileDesc, uint8_t cmd);

void fifthLineNewText(const char* text, int spiFileDesc, uint8_t cmd);

void seventhLineNewText(const char* text, int spiFileDesc, uint8_t cmd);

void eighthLineNewText(const char* text, int spiFileDesc, uint8_t cmd);

// Get the status of the thread (true means continue thread, false means stop thread)
bool getThreadStatus(void);

// Client requests to stop the thread
void stopThread(void);

void sleepForMs(long long delayInMs);

void init_all_pins(void);

void shutdown_all_pins(void);

int SPI_initPort(char* spiDevice);

void SPI_transfer(int spiFileDesc, uint8_t *sendBuf, uint8_t *receiveBuf, int length);

// According to 4-line serial interface timing diagram on page 35 of the datasheet
void Display_writeCommand(int spiFileDesc, uint8_t command, uint8_t *params, size_t param_count);

// According to the 4-line serial interface "8-bit read" timing diagram on page 38 of the datasheet
uint8_t Display_read8Command(int spiFileDesc, uint8_t command);

void hardwareReset(void);

// Helper function. Need client code to free the return value
char *decimalToHexadecimal(int decimal_value);

/* Returns the low byte and the high byte
   Client code needs to free the return value */
uint8_t *highAndLowBytesFromDecimal(int decimal_value);

void displayRectangle(int spiFileDesc, uint8_t cmd, uint8_t color, int xStart, int xEnd, int yStart, int yEnd, bool cond);

void displayString(const char *text, int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

void charToDisplay(char currentChar, int spiFileDesc, uint8_t cmd, uint8_t color, int fontSize, int xStart, int yStart, bool cond);

void displaylogic_init(void);

void displaylogic_cleanup(void);

char ** getLocationBuffer(void);

char ** getTimeBuffer(void);

// get the flag of the screen display, true means able to flash the screen, false means screen is in flashing
bool getFlagScreenDisplay(void);

// set the flag of the screen display, true means able to flash the screen, false means screen is in flashing
void setFlagScreenDisplay(bool flag);

// get the flag of the screen update, true means Update hold the screen, false means NFC hold the screen
bool getNFCOrUpdate(void);

// set the flag of the screen update, true means Update hold the screen, false means NFC hold the screen
void setNFCOrUpdate(bool flag);

#endif