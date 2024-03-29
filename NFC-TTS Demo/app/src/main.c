/*-
 * Free/Libre Near Field Communication (NFC) library
 *
 * Libnfc historical contributors:
 * Copyright (C) 2009      Roel Verdult
 * Copyright (C) 2009-2013 Romuald Conty
 * Copyright (C) 2010-2012 Romain Tartière
 * Copyright (C) 2010-2013 Philippe Teuwen
 * Copyright (C) 2012-2013 Ludovic Rousseau
 * See AUTHORS file for a more comprehensive list of contributors.
 * Additional contributors of this file:
 * Copyright (C) 2020      Adam Laurie
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  1) Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  2 )Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Note that this license only applies on the examples, NFC library itself is under LGPL
 *
 */

/**
 * @file nfc-poll.c
 * @brief Polling example
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <err.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <nfc/nfc.h>
#include <nfc/nfc-types.h>

#define MAX_DEVICE_COUNT 16


// To compile this simple example:
// $ gcc -o quick_start_example1 quick_start_example1.c -lnfc
// ./quick_start_example1

#include "utils.h"
#include <flite/flite.h>
//#include <espeak/speak_lib.h>
// Callback function to handle speech synthesis events
//int SynthCallback(short *wav, int num_samples, espeak_EVENT *events) {
//    for (espeak_EVENT *event = events; event->type != espeakEVENT_LIST_TERMINATED; event++) {
//        switch (event->type) {
//            case espeakEVENT_WORD:
//                break;
//            case espeakEVENT_SENTENCE:
//                break;
//            case espeakEVENT_END:
//                printf("espeakEVENT_END\n");
//                break;
//
//            default:
//                break;
//        }
//    }
//    return 0;
//}

// Updated key definitions
uint8_t keyA[6] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};
uint8_t keyB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t data[16] = {0x12, 0x34, 0xAB, 0xCD, 0xEF, 0x39, 0x1e, 0x48, 0x05, 0x73, 0x90, 0x38, 0xf3, 0xd6, 0xa8, 0x41}; // Data to write

bool authenticate_sector(nfc_device* pnd, uint8_t block, uint8_t* key, bool useKeyA, nfc_target nt) {
    uint8_t cmd[12]; // Authentication command buffer
    cmd[0] = useKeyA ? 0x60 : 0x61; // 0x60 for Key A, 0x61 for Key B authentication
    cmd[1] = block; // Target block for authentication
    memcpy(cmd + 2, key, 6); // The key (either Key A or Key B)
    // Use the last 4 bytes of the UID for authentication (common practice)
    memcpy(cmd + 8, nt.nti.nai.abtUid + (nt.nti.nai.szUidLen - 4), 4);

    // Send the command and check the response
    int res = nfc_initiator_transceive_bytes(pnd, cmd, sizeof(cmd), NULL, 0, 0);
    return res >= 0;
}



bool write_block(nfc_device* pnd, uint8_t block, uint8_t* data) {
    uint8_t cmd[18]; // Write command buffer
    cmd[0] = 0xA0; // Command code for writing to a block
    cmd[1] = block; // The block you want to write to
    memcpy(cmd + 2, data, 16); // Data to write

    // Send the command and check the response
    int res = nfc_initiator_transceive_bytes(pnd, cmd, sizeof(cmd), NULL, 0, 0);
    return res >= 0;
}

bool read_block(nfc_device* pnd, uint8_t block, uint8_t* out_data) {
    uint8_t cmd[2] = {0x30, block}; // MIFARE Classic READ command followed by the block number
    // Buffer for the response; note: might need to adjust the size based on your card
    uint8_t res[18];

    // Send the command and check if the read was successful
    int res_len = nfc_initiator_transceive_bytes(pnd, cmd, sizeof(cmd), res, sizeof(res), 0);
    if (res_len >= 0) {
        memcpy(out_data, res, 16); // Copy only the first 16 bytes (block data)
        return true;
    } else {
        return false;
    }
}



int main(__attribute__((unused)) int argc, const char *argv[]) {

    // ESPEAK TTS
    // Initialize eSpeak
//    espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
//    espeak_SetVoiceByName("en-us+f4");
//    espeak_SetParameter(espeakRATE, 170, 0);
//    espeak_SetSynthCallback(SynthCallback);
//    const char *text = "espeak has terse documentation";
//    unsigned int unique_identifier;
//    espeak_Synth(text, strlen(text) + 1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, &unique_identifier, NULL);
//    espeak_Synchronize();

    // FLITE TTS
    flite_init();
    cst_voice *voice;
    cst_features *features;
    voice = (cst_voice *) register_cmu_us_kal(NULL);
    const char *text = "Bus fifty nine departing soon at stop one";
    features = new_features();
    flite_feat_set_float(features, "duration_stretch", 1.5); // 1.0 is normal, greater than 1.0 is slower
    flite_feat_set_float(features, "int_f0_target_mean", 1500); // Adjust average pitch (Hz)
    flite_feat_set_float(features, "int_f0_target_stddev", 9); // Adjust pitch variability
    const char *output_file = "output.wav";
    flite_text_to_speech(text, voice, output_file);
    char command[256];
    snprintf(command, sizeof(command), "aplay %s", output_file);
    // Play the output.wav file
    system(command);

//    // NFC Constant Polling and Wait for Card
    nfc_device *pnd;
    nfc_target nt;
    nfc_context *context;

    nfc_init(&context);
    if (context == NULL) {
        printf("Unable to init libnfc (malloc)\n");
        exit(EXIT_FAILURE);
    }

    const char *acLibnfcVersion = nfc_version();
    printf("%s uses libnfc %s\n", argv[0], acLibnfcVersion);

    pnd = nfc_open(context, NULL);
    if (pnd == NULL) {
        printf("ERROR: Unable to open NFC device.\n");
        exit(EXIT_FAILURE);
    }

    if (nfc_initiator_init(pnd) < 0) {
        nfc_perror(pnd, "nfc_initiator_init");
        exit(EXIT_FAILURE);
    }

    printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));

    const nfc_modulation nmMifare = {
            .nmt = NMT_ISO14443A,
            .nbr = NBR_106,
    };

    // Poll for an ISO14443A (MIFARE) tag
    int res;
    if ((res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt)) <= 0) {
        printf("Waiting for card...\n");
    }
    while ((res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt)) <= 0) {
        sleep_ms(500);
    }

    printf("The following (NFC) ISO14443A tag was found:\n");
    printf("    ATQA (SENS_RES): ");
    print_hex(nt.nti.nai.abtAtqa, 2);
    printf("       UID (NFCID%c): ", (nt.nti.nai.abtUid[0] == 0x08 ? '3' : '1')); // Use this to identify Card ID
    print_hex(nt.nti.nai.abtUid, nt.nti.nai.szUidLen);
    printf("      SAK (SEL_RES): ");
    print_hex(&nt.nti.nai.btSak, 1);
//    print_nfc_target(&nt, false);
    if (nt.nti.nai.szAtsLen) {
        printf("          ATS (ATR): ");
        print_hex(nt.nti.nai.abtAts, nt.nti.nai.szAtsLen);
    }

//    // Wait for the card to be removed
//    if (nfc_initiator_target_is_present(pnd, NULL) == 0) {
//        printf("Waiting for card removal...\n");
//    }
//    while (nfc_initiator_target_is_present(pnd, NULL) == 0) {
//        sleep_ms(500);
//    }
//    printf("Card removed.\n");
    uint8_t blockData[16]; // Buffer to store read data
    // Assuming 'pnd' and 'nt' are already set up from detecting the card
    uint8_t sector = 4;
    uint8_t block = sector * 4; // Block calculation for Sector 2, Block 0


    if (authenticate_sector(pnd, block, keyA, true, nt)) {
        printf("Authenticated with Key A.\n");
        printf("Before writing...\n");
        for (uint8_t block2 = block; block2 <= block; block2++) {
            if (read_block(pnd, block2, blockData)) {
                printf("Block %d: ", block2);
                for (int i = 0; i < 16; i++) {
                    printf("%02x ", blockData[i]);
                }
                printf("\n");
            } else {
                printf("Failed to read block %d.\n", block2);
            }
        }
        write_block(pnd, block, data);
        printf("After writing...\n");
        for (uint8_t block2 = block; block2 < block; block2++) {
            if (read_block(pnd, block2, blockData)) {
                printf("Block %d: ", block2);
                for (int i = 0; i < 16; i++) {
                    printf("%02x ", blockData[i]);
                }
                printf("\n");
            } else {
                printf("Failed to read block %d.\n", block2);
            }
        }
    } else if (authenticate_sector(pnd, block, keyB, false, nt)) { // If Key A fails, try Key B
        printf("Authenticated with Key B.\n");
        // Proceed to write data...
    } else {
        printf("Failed to authenticate with either key.\n");
    }



    for (uint8_t block2 = block; block2 <= block; block2++) { // Loop over the blocks in Sector 2
        if (read_block(pnd, block2, blockData)) {
            printf("Block %d: ", block2);
            for (int i = 0; i < 16; i++) {
                printf("%02x ", blockData[i]);
            }
            printf("\n");
        } else {
            printf("Failed to read block %d.\n", block2);
        }
    }


    // Clean up
    nfc_close(pnd);
    nfc_exit(context);
    exit(EXIT_SUCCESS);
}