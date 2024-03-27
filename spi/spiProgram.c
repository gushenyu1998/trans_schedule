#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <time.h>

// SPI settings
static const char *spi_device = "/dev/spidev1.0";
static uint8_t spi_mode = SPI_MODE_0;
static uint8_t spi_bits = 8;
static uint32_t spi_speed = 500000; // 500 kHz

// Initialize SPI
int spi_init(const char *device) {
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the SPI device");
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_MODE, &spi_mode) == -1) {
        perror("Failed to set SPI mode");
        close(fd);
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits) == -1) {
        perror("Failed to set SPI bits per word");
        close(fd);
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) == -1) {
        perror("Failed to set SPI speed");
        close(fd);
        return -1;
    }

    return fd;
}

// Close SPI
void spi_close(int fd) {
    close(fd);
}

// Send a command to the display
void send_spi_command(int fd, uint8_t cmd) {
    // Your implementation to send a command via SPI
    write(fd, &cmd, 1);
}

// Send data to the display
void send_spi_data(int fd, const uint8_t *data, size_t length) {
    // Your implementation to send data via SPI
    write(fd, data, length);
}

// Set a pixel at a specific location with a specific color
void set_pixel(int fd, uint16_t x, uint16_t y, uint16_t color) {
    uint8_t command;
    uint8_t data[4];

    // Set column address (X)
    command = 0x2A; // Column address set
    data[0] = x >> 8; data[1] = x & 0xFF; // Start column high & low
    data[2] = x >> 8; data[3] = x & 0xFF; // End column high & low
    send_spi_command(fd, command);
    send_spi_data(fd, data, 4);

    // Set row address (Y)
    command = 0x2B; // Page address set (row address)
    data[0] = y >> 8; data[1] = y & 0xFF; // Start row high & low
    data[2] = y >> 8; data[3] = y & 0xFF; // End row high & low
    send_spi_command(fd, command);
    send_spi_data(fd, data, 4);

    // Write to memory
    command = 0x2C; // Memory write
    send_spi_command(fd, command);
    data[0] = color >> 8; data[1] = color & 0xFF; // color high & low
    send_spi_data(fd, data, 2);
}

// Main function
int main() {
    // Initialize SPI
    int fd = spi_init(spi_device);
    if (fd < 0) return 1;

    // Set a specific pixel to a specific color
    // Example: Set pixel at (10, 10) to red (color code 0xF800 in RGB565)
    set_pixel(fd, 10, 10, 0xF800);

    printf("SPI device opened and pixel set successfully\n");

    // Close SPI
    spi_close(fd);

    return 0;
}
