//
// Created by Jacky Lim on 2024-04-09.
//

#include "../include/hal/useNFC.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <nfc/nfc.h>
#include <nfc/nfc-types.h>
#include "hal/readNWrite.h"

uint8_t keyA[6] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};
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

void writeToNFC(char* inputString){
    nfc_device *pnd;
    nfc_target nt;
    nfc_context *context;
    nfc_init(&context);
    pnd = nfc_open(context, NULL);
    const nfc_modulation nmMifare = {
            .nmt = NMT_ISO14443A,
            .nbr = NBR_106,
    };
    int res;
    uint8_t blockData[16];
    uint8_t sector = 1;
    uint8_t block = sector * 4;
    if ((res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt)) <= 0) {
        printf("Waiting for card...\n");
    }
    while ((res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt)) <= 0) {
    }
    printf("\033[A");
    printf("\033[K");
    if (authenticate_sector(pnd, block, keyA, true, nt)) {
        read_block(pnd, block, blockData);
//        char inputString[] = "John";
        uint8_t data[16] = {0};
        stringToUint8Array(inputString, data, sizeof(data));
        write_block(pnd, block, data);
    } else if (authenticate_sector(pnd, block, keyB, false, nt)) {
        printf("Authenticated with Key B.\n");
    }
    nfc_close(pnd);
    nfc_exit(context);
}

char* readNFC(void){
    nfc_device *pnd;
    nfc_target nt;
    nfc_context *context;
    nfc_init(&context);
    pnd = nfc_open(context, NULL);
    const nfc_modulation nmMifare = {
            .nmt = NMT_ISO14443A,
            .nbr = NBR_106,
    };

    int res;
    uint8_t blockData[16];
    uint8_t sector = 1;
    uint8_t block = sector * 4;
    if ((res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt)) <= 0) {
        printf("Waiting for card...\n");
    }
    while ((res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt)) <= 0) {
    }
    printf("\033[A");
    printf("\033[K");
    char* output = (char*)malloc(17 * sizeof(char));
    if (authenticate_sector(pnd, block, keyA, true, nt)) {
        read_block(pnd, block, blockData);
        hexToAlphabeticalString(blockData, output, sizeof(blockData));
    } else if (authenticate_sector(pnd, block, keyB, false, nt)) {
        printf("Authenticated with Key B.\n");
    }
    nfc_close(pnd);
    nfc_exit(context);
    return output;
}

void init_nfc(void){
    writeToFile("/sys/devices/platform/ocp/ocp:P9_17_pinmux/state", "i2c");
    writeToFile("/sys/devices/platform/ocp/ocp:P9_18_pinmux/state", "i2c");
}
