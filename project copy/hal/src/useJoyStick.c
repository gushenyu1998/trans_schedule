// Source File for useJoyStick.c
// Use the JoyStick to control and read the input from the JoyStick

#include "hal/useJoyStick.h"
#include "hal/useAPI.h"
#include "displaylogic.h"
#include "hal/readNWrite.h"
#include "hal/useNFC.h"

#define MAX_NAME_LENGTH 15  // Define maximum length of the name

void inititalize_JoyStick()
{
    char gpioCommand[1024];
    memset(gpioCommand, 0, sizeof(gpioCommand));
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_LEFT);
    runCommand(gpioCommand);
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_RIGHT);
    runCommand(gpioCommand);
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_UP);
    runCommand(gpioCommand);
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_DOWN);
    runCommand(gpioCommand);
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_PRESS);
    runCommand(gpioCommand);
    sprintf(gpioCommand, "config-pin %s gpio", BUTTON_GPIO);
    runCommand(gpioCommand);
    struct timespec reqDelay = {0, 500000000};
    nanosleep(&reqDelay, (struct timespec *)NULL);

    writeToFile(JOYSTICK_DIRECTION_PRESS, "in");
    writeToFile(JOYSTICK_EDGE_PRESS, "both");
    writeToFile(JOYSTICK_DIRECTION_LEFT, "in");
    writeToFile(JOYSTICK_EDGE_LEFT, "both");
    writeToFile(JOYSTICK_DIRECTION_RIGHT, "in");
    writeToFile(JOYSTICK_EDGE_RIGHT, "both");
    writeToFile(JOYSTICK_DIRECTION_UP, "in");
    writeToFile(JOYSTICK_EDGE_UP, "both");
    writeToFile(JOYSTICK_DIRECTION_DOWN, "in");
    writeToFile(JOYSTICK_EDGE_DOWN, "both");
    writeToFile(BUTTON_DIRECTION, "in");
    writeToFile(BUTTON_EDGE, "both");
}

/**
 * Refered from the use case of gpio wait. Modify it to read multiple files within an array.
 * Input:
 *  char *[] fileNameForGpioValue: the array of path of monitored gpio on the BBG
 *  int[] waitMillisecond: the array of maximun waiting time for the
 *  int numGpios: the number of how many gpio are wating
 * Return:
 *  char list the triggered GPIO in bitwise. 0 bit is not triggered, 1 is triggered
 */

char waitForGpioEdge(char **fileNamesForGpioValue, int *waitMilliseconds, int numGpios)
{
    int epollfd = epoll_create(1);
    if (epollfd == -1)
    {
        fprintf(stderr, "ERROR: epoll_create() returned (%d) = %s\n", epollfd, strerror(errno));
        return 0;
    }

    struct epoll_event epollStruct[numGpios];
    int fds[numGpios];
    char triggered = 0;

    // enroll each gpio in the array into the epoll by looping
    for (int i = 0; i < numGpios; i++)
    {
        fds[i] = open(fileNamesForGpioValue[i], O_RDONLY | O_NONBLOCK);
        if (fds[i] == -1)
        {
            fprintf(stderr, "ERROR: unable to open GPIO value file = %s\n", strerror(errno));
            // Cleanup
            while (--i >= 0)
                close(fds[i]);
            close(epollfd);
            return triggered;
        }

        // configure epoll to wait for events: read operation | edge triggered | urgent data
        epollStruct[i].events = EPOLLIN | EPOLLET | EPOLLPRI;
        epollStruct[i].data.fd = fds[i];

        // register file descriptor on the epoll instance, see: man epoll_ctl
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fds[i], &epollStruct[i]) == -1)
        {
            fprintf(stderr, "ERROR: epoll_ctl() failed = %s\n", strerror(errno));
            // Cleanup
            while (i >= 0)
                close(fds[i--]);
            close(epollfd);
            return triggered;
        }
    }

    // Wait for an event on any GPIO, but ignore the first trigger
    for (size_t i = 0; i < 2; i++)
    {
        if (epoll_wait(epollfd, epollStruct, numGpios, *waitMilliseconds) == -1)
        {
            fprintf(stderr, "ERROR: epoll_wait() failed = %s\n", strerror(errno));
            // Cleanup
            for (int i = 0; i < numGpios; i++)
                close(fds[i]);
            close(epollfd);
            return triggered;
        }
    }

    // Check which GPIOs were triggered by reading values
    for (int i = 0; i < numGpios; i++)
    {
        char buff[1024];
        int bytesRead = readLineFromFile(fileNamesForGpioValue[i], buff, 1024);
        if (bytesRead > 0 && atoi(buff) == 0)
        {
            triggered |= (1 << i); // If ith is tiggered, set ith bit in trigger to 1. For example, if second gpio triggered, set triggered to 0000 0010
        }
    }

    // Cleanup
    for (int i = 0; i < numGpios; i++)
        close(fds[i]);
    close(epollfd);

    return triggered;
}

// function for switch mode to user selection
void selectScheduleRecall(char *name)
{
    // Start critical section (WRITE)
    lockUpdateMutex();
    setNFCOrUpdate(false);
//    unlockUpdateMutex();

    char buffer[4];
    unsigned int maxLength = sizeof(buffer);
    readLineFromFile("/sys/class/gpio/gpio30/value", buffer, maxLength);
//    printf("Read value: %s\n", buffer);

    int busIndex = 0;
    int scheduleIndex = 0;
    int timeOfWait = 5;

    transStruct_t *busStruct = getBusStruct();
    int busStructSize = getBusStructSize();
    recallSchedule_t *recallSchedule = getRecallSchedule();
    char ** locations = getLocationBuffer();
    char ** times = getTimeBuffer();

    char *listOfDetection[] = {
            JOYSTICK_IN_PRESS,
            JOYSTICK_IN_UP,
            JOYSTICK_IN_DOWN,
            JOYSTICK_IN_LEFT,
            JOYSTICK_IN_RIGHT,
            BUTTON_GPIO
    };
    int waitTimes[] = {10000, 10000, 10000, 10000, 10000}; // wait operation for 10s, if no responce in 10s then return

    // check if the button is pressed, allow user to set the name
    if (atoi(buffer) == 1)
    {
        // Set up the alphabet array
        char alphabet[26];
        for (int i = 0; i < 26; i++)
            alphabet[i] = 'A' + i;

        char set_name[15];
        for (int i = 0; i < 15; i++)
            set_name[i] = '\0';  // Initialize the name array
        set_name[0] = alphabet[0];  // Start with the first alphabet character
        int nameIndex = 0;

        while(true)
        {
            // Update display and print the current name
            sprintf(locations[0], "Set Name:");
            sprintf(times[0], "%s", set_name);
            sprintf(locations[1], " ");
            sprintf(times[1], " ");
            sprintf(locations[2], " ");
            sprintf(times[2], " ");
            lockDisplayMutex();
            setFlagScreenDisplay(true);
            unlockDisplayMutex();
            char userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);
            if ((userInput & (1 << 1)) != 0)
            {
                set_name[nameIndex] = (set_name[nameIndex] - 'A' + 1) % 26 + 'A';
            }
            if ((userInput & (1 << 2)) != 0)
            {
                set_name[nameIndex] = (set_name[nameIndex] - 'A' + 25) % 26 + 'A';
            }
            if ((userInput & 1) != 0)
            {
                nameIndex++;
                readLineFromFile("/sys/class/gpio/gpio30/value", buffer, maxLength);
                if (nameIndex >= 15 || atoi(buffer) == 1) {
                    break;
                }
                if (set_name[nameIndex] == '\0') {
                    set_name[nameIndex] = 'A';
                }
            }

            userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);

        }
        sprintf(times[2], "Tap NFC Card");
        lockDisplayMutex();
        setFlagScreenDisplay(true);
        unlockDisplayMutex();
        printf("final name: %s\n", set_name);
        writeToNFC(set_name, 1);
        recallSchedule_t * scheduleBuffer = getBusSchedule();
        sprintf(locations[0],"%s ", scheduleBuffer[0].sentence);
        sprintf(times[0]," ");
        sprintf(locations[1],"%s ", scheduleBuffer[1].sentence);
        sprintf(times[1]," ");
        sprintf(locations[2],"%s ", scheduleBuffer[2].sentence);
        sprintf(times[2]," ");
        setNFCOrUpdate(true);
        unlockUpdateMutex();
        lockDisplayMutex();
        setFlagScreenDisplay(true);
        unlockDisplayMutex();
        return;

    }

    while (true)
    {
        // modify it to display
        // printf("Which Route you want to wait? %s\n", busStruct[busIndex].RouteNo);

        // setup location lines
        sprintf(locations[1], "Which BUS to take:");
        sprintf(locations[0], "Hello %s", name);
        sprintf(locations[2], " ");

        // setup time lines
        sprintf(times[1], "%s ", busStruct[busIndex].RouteNo);
        sprintf(times[0], " ");
        sprintf(times[2], " ");

        lockDisplayMutex();
        setFlagScreenDisplay(true);
        unlockDisplayMutex();

        char userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);
        printf("User input: %d\n", userInput);

        // user input up
        if ((userInput & (1 << 1)) != 0)
        {
            busIndex--;
            if (busIndex < 0)
                busIndex = busStructSize - 1;
        }

        // user input down
        if ((userInput & (1 << 2)) != 0)
        {
            busIndex = (busIndex + 1) % busStructSize;
        }

        // user input press
        if ((userInput & 1) != 0)
        {
            break;
        }
        if (userInput == 0){
            recallSchedule_t * scheduleBuffer = getBusSchedule();
            sprintf(locations[0],"%s ", scheduleBuffer[0].sentence);
            sprintf(times[0]," ");
            sprintf(locations[1],"%s ", scheduleBuffer[1].sentence);
            sprintf(times[1]," ");
            sprintf(locations[2],"%s ", scheduleBuffer[2].sentence);
            sprintf(times[2]," ");
            setNFCOrUpdate(true);
            unlockUpdateMutex();
            lockDisplayMutex();
            setFlagScreenDisplay(true);
            unlockDisplayMutex();
            return;
        }
        userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);
    }
    waitForGpioEdge(listOfDetection, waitTimes, 5);
    while (true)
    {
        // goes around the seconds level and select the bus schedule
        sprintf(locations[0], "Time to take %s:", busStruct[busIndex].RouteNo);
        sprintf(locations[1], " ");
        sprintf(locations[2], " ");

        // setup time lines
        sprintf(times[0], "%s ", busStruct[busIndex].schedule[scheduleIndex].ExpectedLeaveTime);
        sprintf(times[1], " ");
        sprintf(times[2], " ");

        lockDisplayMutex();
        setFlagScreenDisplay(true);
        unlockDisplayMutex();

        char userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);
        printf("User input2: %d\n", userInput);
        // user input up
        if ((userInput & (1 << 1)) != 0)
        {
            scheduleIndex--;
            if (scheduleIndex < 0)
                scheduleIndex = busStruct[busIndex].scheduleSize - 1;
        }

        // user input down
        if ((userInput & (1 << 2)) != 0)
        {
            scheduleIndex = (scheduleIndex + 1) % busStruct[busIndex].scheduleSize;
        }

        // user input press
        if ((userInput & 1) != 0)
        {
            break;
        }
        if (userInput == 0){
            recallSchedule_t * scheduleBuffer = getBusSchedule();
            sprintf(locations[0],"%s ", scheduleBuffer[0].sentence);
            sprintf(times[0]," ");
            sprintf(locations[1],"%s ", scheduleBuffer[1].sentence);
            sprintf(times[1]," ");
            sprintf(locations[2],"%s ", scheduleBuffer[2].sentence);
            sprintf(times[2]," ");
            setNFCOrUpdate(true);
            unlockUpdateMutex();
            lockDisplayMutex();
            setFlagScreenDisplay(true);
            unlockDisplayMutex();
            return;
        }
        waitForGpioEdge(listOfDetection, waitTimes, 5);
    }
    waitForGpioEdge(listOfDetection, waitTimes, 5);
    while (true)
    {
        // select the time to notice in advance
        printf("How much time you want to notice in advance? %d mins\n", timeOfWait);
        sprintf(locations[0], "Time notice in advance:");
        sprintf(locations[1], " ");
        sprintf(locations[2], " ");

        // setup time lines
        sprintf(times[0], "%d ", timeOfWait);
        sprintf(times[1], " ");
        sprintf(times[2], " ");

        lockDisplayMutex();
        setFlagScreenDisplay(true);
        unlockDisplayMutex();

        char userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);
        int expectedCountDown;
        sscanf(busStruct[busIndex].schedule[scheduleIndex].ExpectedCountdown, "%d", &expectedCountDown);
        // user input up
        if ((userInput & (1 << 1)) != 0)
        {
            timeOfWait--;
            if (timeOfWait < 0)
                timeOfWait = MIN(expectedCountDown, 15);
        }

        // user input down
        if ((userInput & (1 << 2)) != 0)
        {
            timeOfWait = (timeOfWait + 1) % MIN(expectedCountDown, 15);
        }

        // user input press
        if ((userInput & 1) != 0)
        {
            break;
        }

        if (userInput == 0) {
            recallSchedule_t * scheduleBuffer = getBusSchedule();
            sprintf(locations[0],"%s ", scheduleBuffer[0].sentence);
            sprintf(times[0]," ");
            sprintf(locations[1],"%s ", scheduleBuffer[1].sentence);
            sprintf(times[1]," ");
            sprintf(locations[2],"%s ", scheduleBuffer[2].sentence);
            sprintf(times[2]," ");
            setNFCOrUpdate(true);
            unlockUpdateMutex();
            lockDisplayMutex();
            setFlagScreenDisplay(true);
            unlockDisplayMutex();
            return;
        }
        waitForGpioEdge(listOfDetection, waitTimes, 5);
    }
    bool flag = false;
    // load the schedule of recall from here
    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {

        // find a block of the buff is NULL
        if (recallSchedule[i].sentence == NULL)
        {
            // prepare the sentence of
            char *recall = malloc(sizeof(char) * 100);
            sprintf(recall, "%s, route %s will come in %d minutes", name, busStruct[busIndex].RouteNo, timeOfWait);
            // load the time to the time_t based on the Expected count down
            int expected_countdown;
            sscanf(busStruct[busIndex].schedule[scheduleIndex].ExpectedCountdown, "%d", &expected_countdown);
            time_t scheduledTime;
            time(&scheduledTime);
            scheduledTime += expected_countdown*60;
            scheduledTime -= timeOfWait * 60;

            // load the data to the sturcture
            recallSchedule[i].schedule_time = scheduledTime;
            recallSchedule[i].sentence = recall;
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        printf("Buffer is full\n"); // replace it to displaying
    }
    sprintf(locations[0], "Ok We will notify %s", name);
    sprintf(locations[1], "%d MINS IN ADVANCE", timeOfWait);
    sprintf(locations[2], "for bus %s", busStruct[busIndex].RouteNo);

    // setup time lines
    sprintf(times[0], " ");
    sprintf(times[1], " ");
    sprintf(times[2], " ");

    lockDisplayMutex();
    setFlagScreenDisplay(true);
    unlockDisplayMutex();

    sleepForMs(5000);


    recallSchedule_t * scheduleBuffer = getBusSchedule();
    sprintf(locations[0],"%s ", scheduleBuffer[0].sentence);
    sprintf(times[0]," ");
    sprintf(locations[1],"%s ", scheduleBuffer[1].sentence);
    sprintf(times[1]," ");
    sprintf(locations[2],"%s ", scheduleBuffer[2].sentence);
    sprintf(times[2]," ");

//    lockUpdateMutex();
    setNFCOrUpdate(true);
    unlockUpdateMutex();

    lockDisplayMutex();
    setFlagScreenDisplay(true);
    unlockDisplayMutex();
}