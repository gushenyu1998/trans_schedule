#include "main.h"
#include "hal/useNFC.h"
#include "hal/useTTS.h"
#include <flite/flite.h>
#include <alsa/asoundlib.h>


#include <stdlib.h>

void speak_and_play(const char* text) {
    char command[256];
    snprintf(command, sizeof(command), "espeak '%s' -w test.wav > /dev/null 2>&1", text);
    system(command);
    system("aplay test.wav > /dev/null 2>&1");
}

int main(void)
{
//	int size;
//	char *calling = "https://api.translink.ca/rttiapi/v1/stops/55713/estimates?apikey=JoKWW8MHpsoc04lKVKnA&count=2";
//	transStruct_t *a = (transStruct_t *) ReadFromTransAPI(&size, calling);
//	freeTransStruct(size, a);

    // ---------------------------- TTS ----------------------------

//    espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
//    espeak_SetVoiceByName("en-us+f4");
//    espeak_SetParameter(espeakRATE, 170, 0);
//    espeak_SetSynthCallback(SynthCallback);
//    const char *text = "Bus fifty nine departing soon at stop one";
//    unsigned int unique_identifier;
//    espeak_Synth(text, strlen(text) + 1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, &unique_identifier, NULL);
//    espeak_Synchronize();

    // ---------------------------- TTS ----------------------------

//    initialize_flite();
//    cst_voice *voice = setup_voice();
//
//    const char *text = "Bus fifty nine departing soon at stop one";
//    synthesize_text_to_file(text, voice, "play");

    // play the text with espeak
    speak_and_play("Bus fifty nine departing soon at stop one");


    // ---------------------------- NFC ----------------------------

    init_nfc();

    char inputString2[] = "Tommy";
    writeToNFC(inputString2, 1);

    char inputString[] = "Santa";
    writeToNFC(inputString, 2);

    for (int i=1; i<=2; i++){
        char* output = readNFC(i);
        printf("Name: %s\n", output);
        free(output);
    }

	return 1;
}

