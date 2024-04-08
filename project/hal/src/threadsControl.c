#include "hal/threadsControl.h"
// Global variables
bool stop = false;

// Mutexes for useLED and Sampler modules
pthread_mutex_t mainMutex = PTHREAD_MUTEX_INITIALIZER;

// Start all threads such as the udp and acclerometer
void startThreads() {
    printf("create thread\n");
}

// Join all threads
void joinThreads(pthread_t *threads) {
    for (int i = 0; i < 2; i++) {
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