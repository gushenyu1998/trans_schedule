#include "main.h"
#include <flite/flite.h>
#include <alsa/asoundlib.h>

int main(void)
{
//	int size;
//	char *calling = "https://api.translink.ca/rttiapi/v1/stops/55713/estimates?apikey=JoKWW8MHpsoc04lKVKnA&count=2";
//	transStruct_t *a = (transStruct_t *) ReadFromTransAPI(&size, calling);
//	freeTransStruct(size, a);


    // // ---------------------------- TTS ----------------------------

    // initialize_flite();
    // cst_voice *voice = setup_voice();

    // const char *text = "Bus fifty nine departing soon at stop one";
    // synthesize_text_to_file(text, voice, "play");

    // // ---------------------------- NFC ----------------------------

    init_nfc();
    inititalize_JoyStick();
    startThreads();
    joinThreads();

//    char inputString[] = "Santa";
//    writeToNFC(inputString, 2);
//
//    for (int i=1; i<=2; i++){
//        char* output = readNFC(i);
//        printf("Name: %s\n", output);
//        free(output);
//    }
}

