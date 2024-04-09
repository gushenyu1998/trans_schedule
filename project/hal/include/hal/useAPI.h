#ifndef _API_PARSING_
#define _API_PARSING_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <stdbool.h>
#include <time.h>

#define buffer_size 100
#define max_display 3

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
} transStruct_t;

typedef struct recallSchedule_t
{
    time_t schedule_time;
    char * sentence;
}recallSchedule_t;


//the data sturcture for storing the bus schedule
char* scheduleBuffer[buffer_size];
int schduleBufferSize = 0;
transStruct_t * busStruct;
int busStructSize = 0;

//the data stucture store the recall, set the buffer size up to 100
recallSchedule_t recallSchedule[buffer_size];


//this function will call the API and update the schdule, need to be call once per minutes
void UpdateSchedule(char * API_query);
#endif