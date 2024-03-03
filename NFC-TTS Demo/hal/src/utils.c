// Implementation Source file for utils module.
// Common functions that are used by HAL and APP files
// Also contains the pin configuration to be changed

#include "utils.h"

#include <stdio.h>
#include <time.h>
#include <errno.h>		// Errors

void writeToFile(const char* fileName, const char* value)
{
    FILE *pFile = fopen(fileName, "w");
    fprintf(pFile, "%s", value);
    fclose(pFile);
}

int readFromFile(const char* fileName, char* buff, unsigned int maxLength)
{
    FILE *file = fopen(fileName, "r");
    int bytes_read = getline(&buff, (size_t *) &maxLength, file);
    fclose(file);
    return bytes_read;
}

void nano_sleep(long long seconds, long long nano_seconds)
{
    struct timespec reqDelay = {seconds, nano_seconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void sleep_ms(int milliseconds)
{
    struct timespec ts;
    int res;

    if (milliseconds < 0)
    {
        return;
    }

    ts.tv_sec = milliseconds / 1000;          // Convert milliseconds to seconds
    ts.tv_nsec = (milliseconds % 1000) * 1000000; // Convert remainder to nanoseconds

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR); // Retry if interrupted by signal
}

