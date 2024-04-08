#ifndef _API_PARSING_
#define _API_PARSING_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <stdbool.h>

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

typedef struct transStruct
{
    const char *RouteNo;
    const char *RouteName;
    scheduleStruct *schedule;
} transStruct;

char* scheduleBuffer[buffer_size];
int schduleBufferSize = 0;
transStruct * busStruct;
int busStructSize = 0;


struct transStruct *ReadFromTransAPI(int *size, char *APIquery);
void freeTransStruct(int size, transStruct* trans_info);
#endif