#include "hal/useAPI.h"

int max_display = 3;

// the data sturcture for storing the bus schedule
char *scheduleBuffer[BUFFER_SIZE];
int schduleBufferSize = 0;
transStruct_t *busStruct;
int busStructSize = 0;

// the data stucture store the recall, set the buffer size up to 100
recallSchedule_t recallSchedule[BUFFER_SIZE];

struct MemoryStruct
{
    char *memory;
    size_t size;
};

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

struct transStruct_t *ReadFromTransAPI(int *size, char *APIquery)
{
    struct json_object *parsed_json;
    struct json_object *route;
    struct json_object *route_no;
    struct json_object *route_name;
    struct json_object *schedules;

    struct json_object *route_schedule;
    struct json_object *expected_leave_time;
    struct json_object *expected_count_down;
    struct json_object *destination;
    struct json_object *cancelled_trip;
    struct json_object *cancelled_Stop;

    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = NULL;
    chunk.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    transStruct_t *busSchedule = NULL;                       // create new structure to store the bus schdule
    memset(scheduleBuffer, 0, sizeof(char *) * BUFFER_SIZE); // clean up the buffer
    schduleBufferSize = 0;
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
            printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            parsed_json = json_tokener_parse(chunk.memory);
            size_t json_size = 0;
            if (json_object_get_type(parsed_json) == json_type_array)
            {
                // Get the size of the top-level JSON array
                json_size = json_object_array_length(parsed_json);
                *size = json_size;
                busSchedule = malloc(sizeof(transStruct_t) * json_size);
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
                json_object_object_get_ex(route, "Schedules", &schedules);
                busSchedule[i].RouteNo = json_object_get_string(route_no);
                busSchedule[i].RouteName = json_object_get_string(route_name);
                size_t schedule_size = json_object_array_length(schedules);
                busSchedule[i].schedule = malloc(sizeof(scheduleStruct) * schedule_size);
                for (size_t j = 0; j < schedule_size; j++)
                {

                    route_schedule = json_object_array_get_idx(schedules, j);
                    json_object_object_get_ex(route_schedule, "Destination", &destination);
                    json_object_object_get_ex(route_schedule, "ExpectedLeaveTime", &expected_leave_time);
                    json_object_object_get_ex(route_schedule, "ExpectedCountdown", &expected_count_down);
                    json_object_object_get_ex(route_schedule, "CancelledTrip", &cancelled_trip);
                    json_object_object_get_ex(route_schedule, "CancelledStop", &cancelled_Stop);
                    busSchedule[i].schedule[j].Destination = json_object_get_string(destination);
                    busSchedule[i].schedule[j].ExpectedLeaveTime = json_object_get_string(expected_leave_time);
                    busSchedule[i].schedule[j].ExpectedCountdown = json_object_get_string(expected_count_down);
                    busSchedule[i].schedule[j].CancelledStop = json_object_get_boolean(cancelled_Stop);
                    busSchedule[i].schedule[j].CancelledTrip = json_object_get_boolean(cancelled_trip);
                    if (!busSchedule[i].schedule[j].CancelledStop && busSchedule[i].schedule[j].CancelledTrip) // if the schedule or the stop is not cancelled, then add the size of buffer
                    {
                        char *schedule = malloc(sizeof(char) * BUFFER_SIZE);
                        sprintf("%s %s %s, %smins", busSchedule[i].RouteNo, busSchedule[i].schedule[j].Destination, busSchedule[i].schedule[j].ExpectedLeaveTime, busSchedule[i].schedule[j].ExpectedCountdown);
                        scheduleBuffer[schduleBufferSize] = schedule;
                        schduleBufferSize++;
                    }
                }
            }
        }
        curl_easy_cleanup(curl);
        free(chunk.memory);
    }
    curl_global_cleanup();
    return busSchedule;
}

void freeTransStruct(int size, transStruct_t *trans_info)
{
    for (int i = 0; i < size - 1; i++)
    {
        free(trans_info[i].schedule);
    }
    free(trans_info);
}

void freeScheduleBuffer(int size, char **buffer)
{
    for (int i = 0; i < size; i++)
    {
        free(buffer[i]);
    }
}

// this function should call every minutes
void UpdateSchedule(char *API_query)
{
    freeTransStruct(busStructSize, busStruct);
    freeScheduleBuffer(schduleBufferSize, scheduleBuffer);
    busStruct = ReadFromTransAPI(&busStructSize, API_query);
    for (int i = 0; i < schduleBufferSize; i++)
    {
        if (i > max_display)
            break;
        printf(scheduleBuffer[i]); // replace this line to the displaying
    }

    // search the recall schedule here and play the sound
}

// play the sound of bus schdule
void playBusSchdule()
{
    for (int i = 0; i < schduleBufferSize; i++)
    {
        if (i > max_display)
            break;
        // play the sound at here
    }
}

// get bus schedule
char **getBusSchedule()
{
    return scheduleBuffer;
}

int getBusScheduleSize()
{
    return schduleBufferSize;
}

transStruct_t *getBusStruct()
{
    return busStruct;
}

int getBusStructSize()
{
    return busStructSize;
}

recallSchedule_t *getRecallSchedule()
{
    return recallSchedule;
}

int getMaxDisplay()
{
    return max_display;
}

void setMaxDisplay(int newMaxDisplay)
{
    return newMaxDisplay;
}