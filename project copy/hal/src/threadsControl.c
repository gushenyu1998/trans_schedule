// Source file for threadsControl.c
// Control Threads for fetching data from API, and JoyStick/Buttons

#include "hal/threadsControl.h"
#include "hal/useNFC.h"
#include "hal/useTTS.h"
#include <flite/flite.h>
#include <alsa/asoundlib.h>


// Global variables
bool stop = false;

// Mutexes for Shutdown signal
static pthread_mutex_t mainMutex = PTHREAD_MUTEX_INITIALIZER;

// Mutex for NFC, Display, and Update
static pthread_mutex_t displayMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t updateMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t threads[3];
void *regularUpdate(void *arg);
void *NFCSelection();
void *waitForButton();

void speak_and_play(const char *text) {
    char command[256];
    snprintf(command, sizeof(command), "flite '%s' > /dev/null 2>&1", text);
    system(command);
}

// Start all threads such as the udp and acclerometer
void startThreads(char* API_query) {
    char url[256];
    sprintf(url, "https://api.translink.ca/rttiapi/v1/stops/%s/estimates?apikey=JoKWW8MHpsoc04lKVKnA&count=3", API_query);
    printf("URL: %s\n", url);
    char *url_copy = strdup(url);
    pthread_create(&threads[0], NULL, regularUpdate, url_copy);
    pthread_create(&threads[1], NULL, NFCSelection, NULL);
    pthread_create(&threads[2], NULL, waitForButton, NULL);
}

// Join all threads
void joinThreads() {
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Signal all threads to shutdown
void signalShutdown() {
    pthread_mutex_lock(&mainMutex);
    stop = true;
    pthread_mutex_unlock(&mainMutex);
}

// Check if shutdown is signaled
bool ifShutdown() {
    bool result;
    pthread_mutex_lock(&mainMutex);
    result = stop;
    pthread_mutex_unlock(&mainMutex);
    return result;
}

// update the bus schedule once per minutes
void *regularUpdate(void *arg) {
    while (!ifShutdown()) {
        //update the bus schedule
        char *APIQuery = (char *) arg;
        UpdateSchedule(APIQuery);

        // play the audio of notice
        recallSchedule_t *recallSchedule = getRecallSchedule();
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            if (recallSchedule[i].sentence != NULL) {
                if (abs((int) difftime(recallSchedule[i].schedule_time, time(NULL))) < 60) {
                    // play the audio of notice
                    // using fltie or espeak
                    printf("Playing: %s\n", recallSchedule[i].sentence);
                    speak_and_play(recallSchedule[i].sentence);
                    free(recallSchedule[i].sentence);
                    recallSchedule[i].sentence = NULL;
                }
            }
        }

        // sleep for 60 seconds
        for (int i = 0; i < 60; ++i) {
            if(ifShutdown()){
                break;
            }
            sleep(1);
        }
    }
    return NULL;
}

void *NFCSelection() {
    while (!ifShutdown()) {
        char *name = NULL;
        name = readNFC(1);
        printf("NFC read: %s\n", name);
        if(name == NULL){
            continue;
        }
        if (strcmp(name, "Stop")==0){
            signalShutdown();
            transStruct_t *busStruct = getBusStruct();
            recallSchedule_t * busSchedule = getBusSchedule();
            freeScheduleBuffer(getBusScheduleSize(), busSchedule);
            freeTransStruct(getBusStructSize(), busStruct);
            free(name);
            return NULL;
        }
        selectScheduleRecall(name);
        free(name);
    }
    return NULL;
}

void *waitForButton() {
//    initialize_flite();
//    cst_voice *voice = setup_voice();
    char *button[] = {BUTTON_VALUE};
    int waitTimes[] = {-1};

    while (!ifShutdown()) {
        char result = waitForGpioEdge(button, waitTimes, 1);
        recallSchedule_t *busSchedule = getBusSchedule();
        if (result != 0) {
            int max_display = getMaxDisplay();
            for (int i = 0; i < max_display; ++i) {
                speak_and_play(busSchedule[i].sentence);
//                synthesize_text_to_file(busSchedule[i].sentence, voice, "play");
            }
        }
    }
    return NULL;
}

// lock and unlock the display mutex
void lockDisplayMutex() {
    pthread_mutex_lock(&displayMutex);
}
void unlockDisplayMutex() {
    pthread_mutex_unlock(&displayMutex);
}

// lock and unlock the update mutex
void lockUpdateMutex() {
    pthread_mutex_lock(&updateMutex);
}
void unlockUpdateMutex() {
    pthread_mutex_unlock(&updateMutex);
}