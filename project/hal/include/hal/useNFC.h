//
// Created by Jacky Lim on 2024-04-09.
//

#ifndef PROJECT_USENFC_H
#define PROJECT_USENFC_H

#endif //PROJECT_USENFC_H

#include <nfc/nfc.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char* readNFC(uint8_t sector);
void writeToNFC(char* input, uint8_t sector);
void init_nfc(void);
