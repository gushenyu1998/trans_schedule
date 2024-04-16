// Header file for useNFC.c
// Used to read and write to NFC

#include "../include/hal/useNFC.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nfc/nfc.h>
#include <nfc/nfc-types.h>
#include "hal/readNWrite.h"

uint8_t keyA1[6] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7}; // CARD
uint8_t keyA2[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // FOB
uint8_t keyB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static bool authenticate_sector(nfc_device* pnd, uint8_t block, uint8_t* key, bool useKeyA, nfc_target nt) {
    uint8_t cmd[12];
    cmd[0] = useKeyA ? 0x60 : 0x61;
    cmd[1] = block;
    memcpy(cmd + 2, key, 6);
    memcpy(cmd + 8, nt.nti.nai.abtUid + (nt.nti.nai.szUidLen - 4), 4);
    int res = nfc_initiator_transceive_bytes(pnd, cmd, sizeof(cmd), NULL, 0, 0);
    return res >= 0;
}

static bool write_block(nfc_device* pnd, uint8_t block, uint8_t* data) {
    uint8_t cmd[18];
    cmd[0] = 0xA0;
    cmd[1] = block;
    memcpy(cmd + 2, data, 16);
    int res = nfc_initiator_transceive_bytes(pnd, cmd, sizeof(cmd), NULL, 0, 0);
    return res >= 0;
}

static bool read_block(nfc_device* pnd, uint8_t block, uint8_t* out_data) {
    uint8_t cmd[2] = {0x30, block};
    uint8_t res[18];
    int res_len = nfc_initiator_transceive_bytes(pnd, cmd, sizeof(cmd), res, sizeof(res), 0);
    if (res_len >= 0) {
        memcpy(out_data, res, 16);
        return true;
    } else {
        return false;
    }
}


static void stringToUint8Array(const char *input, uint8_t *output, size_t outputSize) {
    size_t inputLength = strlen(input);
    for (size_t i = 0; i < inputLength && i < outputSize; i++) {
        output[i] = (uint8_t)input[i];
    }
}

static void hexToAlphabeticalString(uint8_t *hexData, char *output, size_t dataSize) {
    for (size_t i = 0; i < dataSize; i++) {
        output[i] = (char)hexData[i];
    }
    output[dataSize] = '\0'; // Null-terminate the string
}

void * writeToNFC(char* inputString, uint8_t sector){
    nfc_device* pnd;
    nfc_target nt;
    nfc_context* context;
    nfc_init(&context);
    pnd = nfc_open(context, NULL);
    const nfc_modulation nmMifare = {
            .nmt = NMT_ISO14443A,
            .nbr = NBR_106,
    };
    while (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) <= 0);
    uint8_t block = sector * 4;
    if (!authenticate_sector(pnd, block, keyA1, true, nt)) {
        usleep(50000);
        nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt); // Reselect the target
        if (!authenticate_sector(pnd, block, keyA2, false, nt)) {
            fprintf(stderr, "Authentication with both keys failed.\n");
            nfc_close(pnd);
            nfc_exit(context);
            return NULL;
        }
    }
    uint8_t data[16] = {0};
    stringToUint8Array(inputString, data, sizeof(data));
    write_block(pnd, block, data);
    nfc_close(pnd);
    nfc_exit(context);
    return NULL;
}

char* readNFC(uint8_t sector) {
    nfc_device* pnd;
    nfc_target nt;
    nfc_context* context;
    nfc_init(&context);
    pnd = nfc_open(context, NULL);
    const nfc_modulation nmMifare = {
            .nmt = NMT_ISO14443A,
            .nbr = NBR_106,
    };
    while (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) <= 0);
    char* output = (char*)malloc(17 * sizeof(char));
    if (!output) {
        fprintf(stderr, "Memory allocation failed.\n");
        nfc_close(pnd);
        nfc_exit(context);
        return NULL;
    }
    uint8_t block = sector * 4;
    if (!authenticate_sector(pnd, block, keyA1, true, nt)) {
        usleep(50000);
        nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt); // Reselect the target
        if (!authenticate_sector(pnd, block, keyA2, false, nt)) {
            fprintf(stderr, "Authentication with both keys failed.\n");
            free(output);
            nfc_close(pnd);
            nfc_exit(context);
            return NULL;
        }
    }
    printf("Authentication successful.\n");
    uint8_t blockData[16];
    read_block(pnd, block, blockData);
    hexToAlphabeticalString(blockData, output, sizeof(blockData));
    nfc_close(pnd);
    nfc_exit(context);
    return output;
}

void init_nfc(void){
    writeToFile("/sys/devices/platform/ocp/ocp:P9_19_pinmux/state", "i2c");
    writeToFile("/sys/devices/platform/ocp/ocp:P9_20_pinmux/state", "i2c");
}
