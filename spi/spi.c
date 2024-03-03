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
#define SPI_DEV_BUS0_CS0 "/dev/spidev0.0"
#define SPI_DEV_BUS1_CS0 "/dev/spidev1.0"
#define SPI_DEV_BUS1_CS1 "/dev/spidev1.1"
#define SPI_MODE_DEFAULT 0
#define SPEED_HZ_DEFAULT 500000 // Arbitrary, but 500000 is reasonable


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

    // Set Max Speed (Hz): Optional
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
    // ! set the D/C to low through GPIO
    SPI_transfer(spiFileDesc, &command, NULL, 1);
    // ! set the D/C to high through GPIO
    SPI_transfer(spiFileDesc, params, NULL, param_count);
    // ! set the CS to high through GPIO
}

// according to the 4-line serial interface "8-bit read" timing diagram on page 38 of the datasheet
uint8_t Display_read8Command(int spiFileDesc, uint8_t command) {
    // ! set the CS to low through GPIO
    // ! set the D/C to low through GPIO
    SPI_transfer(spiFileDesc, &command, NULL, 1);
    // ! set the D/C to high through GPIO
    uint8_t readVal;
    SPI_transfer(spiFileDesc, NULL, &readVal, 1);
    // ! set the CS to high through GPIO
    return readVal;
}

static void hardwareReset() {
    // ! wait for 100 ms
    // ! set RST to low
    // ! wait for 10 ms
    // ! set RST to high
    // ! wait for 20 ms
}


int main(void) {

    // p9_28 spi_cs
    int spiFileDesc = open("/dev/spidev1.0", O_RDWR);

    // int spiMode = 0;
    // ioctl(spiFileDesc, SPIO_IOC_WR_MODE, &spiMode);

    uint8_t *sendBuf = malloc(3);
    uint8_t *receiveBuf = malloc(3);

    // sendBuf[0] = READ REGISTER 0 COMMAND;
    // sendBuf[1] = READ REGISTER 1 COMMAND;
    // sendBuf[2] = 0x00;

    struct spi_ioc_transfer transfer;
    memset(&transfer, 0, sizeof(transfer));

    transfer.tx_buf = (unsigned long) sendBuf;
    transfer.rx_buf = (unsigned long) receiveBuf;
    transfer.len = 3;

    // 5. Make an ioctl call to perform the actual transfer
    const int NUM_TRANSFERS = 1;
    int status = ioctl(spiFileDesc, SPI_IOC_MESSAGE(NUM_TRANSFERS), &transfer);

    uint8_t reg0Data = receiveBuf[1];
    uint8_t reg1Data = receiveBuf[2];

    return 0;
}

