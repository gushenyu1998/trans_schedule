#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <stdbool.h>

struct json_object *parsed_json;
struct json_object *route;
struct json_object *route_no;
struct json_object *route_name;
struct json_object *direction;
struct json_object *schedules;
struct json_object *route_schedule;

struct json_object *expected_leave_time;
struct json_object *expected_count_down;
struct json_object *destination;
struct json_object *cancelled_trip;
struct json_object *cancelled_Stop;


struct MemoryStruct
{
    char *memory;
    size_t size;
};

typedef struct scheduleStruct {
    char * Destination;
    char * ExpectedLeaveTime;
    char * ExpectedCountdown;
    bool CancelledTrip;
    bool CancelledStop;
}scheduleStruct;

typedef struct transStruct {
    char * RouteNo;
    char * RouteName;
    struct scheduleStruct * schedule;
}transStruct;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL)
    {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

struct transStruct* ReadFromTransAPI(int *size, char * APIquery){
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = NULL;
    chunk.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl)
    {   
        curl_easy_setopt(curl, CURLOPT_URL, APIquery);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, "Accept: application/JSON"));
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            transStruct *busSchedule;
            printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            parsed_json = json_tokener_parse(chunk.memory);
            size_t json_size = 0;
            if (json_object_get_type(parsed_json) == json_type_array)
            {
                // Get the size of the top-level JSON array
                json_size = json_object_array_length(parsed_json);
                *size  = json_size;
                busSchedule = malloc(sizeof(transStruct)*json_size);
            }
            else
            {
                printf("Error: Top-level JSON is not an array.\n");
            }
            printf("JSON Size: %zu\n", json_size);
            for (size_t i = 0; i < json_size; i++)
            {
                route = json_object_array_get_idx(parsed_json, i);
                json_object_object_get_ex(route, "RouteNo", &route_no);
                json_object_object_get_ex(route, "RouteName", &route_name);
                json_object_object_get_ex(route, "Direction", &direction);
                json_object_object_get_ex(route, "Schedules", &schedules);
                busSchedule[i].RouteNo = json_object_get_string(route_no);
                busSchedule[i].RouteName = json_object_get_string(route_name);
                
                
                route_schedule = json_object_array_get_idx(schedules, 0);
                size_t schedule_size = json_object_array_length(route_schedule);
                json_object_object_get_ex(route_schedule, "ExpectedLeaveTime", &expected_leave_time);
                printf("Route:\n");
                printf("Route No: %s\n", json_object_get_string(route_no));
                printf("Route Name: %s\n", json_object_get_string(route_name));
                printf("Direction: %s\n", json_object_get_string(direction));
                printf("First Schedule's Expected Leave Time: %s\n", json_object_get_string(expected_leave_time));
            }
        }
        curl_easy_cleanup(curl);
        free(chunk.memory);
    }
    curl_global_cleanup();
    return NULL;
}

int main(void)
{
    int size;
    char *calling = "https://api.translink.ca/rttiapi/v1/stops/60980/estimates?apikey=JoKWW8MHpsoc04lKVKnA&count=2";
    int count = 2;
    ReadFromTransAPI(&size, calling);
    return 1;
}