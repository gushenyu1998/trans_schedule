// Header file for useNFC.c
// Used to read and write to NFC

#ifndef PROJECT_USENFC_H
#define PROJECT_USENFC_H

#endif //PROJECT_USENFC_H

#include <nfc/nfc.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Return the string in the sector (alphabetical)
char* readNFC(uint8_t sector);

// Write the string to the sector (alphabetical)
void * writeToNFC(char* input, uint8_t sector);


void init_nfc(void);

