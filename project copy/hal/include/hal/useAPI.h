// Header file for useAPI.c
// Used to call the API and update the schedule

#ifndef _API_PARSING_
#define _API_PARSING_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <stdbool.h>
#include <time.h>
#include "hal/threadsControl.h"
#include "displaylogic.h"

#define BUFFER_SIZE 100

typedef struct scheduleStruct
{
    const char *Destination;
    const char *ExpectedLeaveTime;
    const char *ExpectedCountdown;
    bool CancelledTrip;
    bool CancelledStop;
} scheduleStruct;

typedef struct transStruct_t
{
    const char *RouteNo;
    const char *RouteName;
    scheduleStruct *schedule;
    int scheduleSize;
} transStruct_t;

typedef struct recallSchedule_t
{
    time_t schedule_time;
    char *sentence;
} recallSchedule_t;

// this function will call the API and update the schdule, need to be call once per minutes
void UpdateSchedule(char *API_query);

void freeTransStruct(int size, transStruct_t *trans_info);
void freeScheduleBuffer(int size, recallSchedule_t *buffer);

recallSchedule_t * getBusSchedule();
int getBusScheduleSize();
transStruct_t *getBusStruct();
int getBusStructSize();
recallSchedule_t *getRecallSchedule();
int getMaxDisplay();
void setMaxDisplay(int newMaxDisplay);

#endif