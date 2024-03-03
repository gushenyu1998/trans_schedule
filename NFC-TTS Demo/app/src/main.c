/*-
 * Free/Libre Near Field Communication (NFC) library
 *
 * Libnfc historical contributors:
 * Copyright (C) 2009      Roel Verdult
 * Copyright (C) 2009-2013 Romuald Conty
 * Copyright (C) 2010-2012 Romain Tartière
 * Copyright (C) 2010-2013 Philippe Teuwen
 * Copyright (C) 2012-2013 Ludovic Rousseau
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
// https://github.com/nfc-tools/libnfc
#include "nfc-utils.h"

#define MAX_DEVICE_COUNT 16


// To compile this simple example:
// $ gcc -o quick_start_example1 quick_start_example1.c -lnfc
// ./quick_start_example1

#include "utils.h"
#include <flite/flite.h>
// https://github.com/festvox/flite

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

int main(__attribute__((unused)) int argc, const char *argv[]) {

    // ESPEAK TTS
    // Initialize eSpeak
//    espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
//    espeak_SetVoiceByName("en-us+f4");
//    espeak_SetParameter(espeakRATE, 170, 0);
//    espeak_SetSynthCallback(SynthCallback);
//    const char *text = "Bus is departing soon.";
//    unsigned int unique_identifier;
//    espeak_Synth(text, strlen(text) + 1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, &unique_identifier, NULL);
//    espeak_Synchronize();

    // FLITE TTS
    flite_init();
    cst_voice *voice;
    voice = (cst_voice *) register_cmu_us_kal(NULL);
    const char *text = "Bus is departing soon.";
    const char *output_file = "output.wav";
    flite_text_to_speech(text, voice, output_file);
//    char command[256];
//    snprintf(command, sizeof(command), "aplay %s", output_file);
    // Play the output.wav file

    // NFC Constant Polling and Wait for Card
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
    print_nfc_target(&nt, false);
    if (nt.nti.nai.szAtsLen) {
        printf("          ATS (ATR): ");
        print_hex(nt.nti.nai.abtAts, nt.nti.nai.szAtsLen);
    }

    // Wait for the card to be removed
    if (nfc_initiator_target_is_present(pnd, NULL) == 0) {
        printf("Waiting for card removal...\n");
    }
    while (nfc_initiator_target_is_present(pnd, NULL) == 0) {
        sleep_ms(500);
    }
    printf("Card removed.\n");

    // Clean up
    nfc_close(pnd);
    nfc_exit(context);
    exit(EXIT_SUCCESS);
}