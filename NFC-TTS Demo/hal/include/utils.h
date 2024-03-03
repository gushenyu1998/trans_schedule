// Utility Functions - Header File
// Common functions that are used by HAL and APP files

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

void writeToFile(const char* fileName, const char* value);
int readFromFile(const char* fileName, char* buff, unsigned int maxLength);
void nano_sleep(long long seconds, long long nano_seconds);
void sleep_ms(int milliseconds);

#endif //UTILS_H
