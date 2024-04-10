#include "hal/useJoyStick.h"
#include "hal/useAPI.h"

void inititalize_JoyStick()
{
    char gpioCommand[1024];
    memset(gpioCommand, 0, sizeof(gpioCommand));
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_LEFT);
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_RIGHT);
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_UP);
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_DOWN);
    sprintf(gpioCommand, "config-pin %s gpio", JOYSTICK_GPIO_PRESS);
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
    int busIndex = 0;
    int scheduleIndex = 0;
    int timeOfWait = 5;

    char *listOfDetection[] = {
        JOYSTICK_IN_PRESS,
        JOYSTICK_IN_UP,
        JOYSTICK_IN_DOWN,
        JOYSTICK_IN_LEFT,
        JOYSTICK_IN_RIGHT,
    };
    int waitTimes[] = {10000, 10000, 10000, 10000, 10000}; // wait operation for 10s, if no responce in 10s then return

    while (true)
    {
        // modify it to display
        printf("Which Route you want to wait? %s\n", busStruct[busIndex].RouteNo);
        char userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);
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

        if (userInput == 0)
            return;
    }

    while (true)
    {
        // goes around the seconds level and select the bus schedule
        printf("Which time do you want to pick up? %s\n", busStruct[busIndex].schedule[scheduleIndex].ExpectedLeaveTime);
        char userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);
        // user input up
        if ((userInput & (1 << 1)) != 0)
        {
            scheduleIndex--;
            if (scheduleIndex < 0)
                scheduleIndex = max_display - 1;
        }

        // user input down
        if ((userInput & (1 << 2)) != 0)
        {
            scheduleIndex = (scheduleIndex + 1) % max_display;
        }

        // user input press
        if ((userInput & 1) != 0)
        {
            break;
        }

        if (userInput == 0)
            return;
    }

    while (true)
    {
        // goes around the seconds level and select the bus schedule
        printf("How much time you want to notice in advance? %d mins\n", timeOfWait);
        char userInput = waitForGpioEdge(listOfDetection, waitTimes, 5);
        // user input up
        if ((userInput & (1 << 1)) != 0)
        {
            timeOfWait--;
            if (timeOfWait < 0)
                timeOfWait = timeOfWait - 1;
        }

        // user input down
        if ((userInput & (1 << 2)) != 0)
        {
           timeOfWait = (timeOfWait + 1) % 15;
        }

        // user input press
        if ((userInput & 1) != 0)
        {
            break;
        }

        if (userInput == 0)
            return;
        // select the time to notice in advance
    }

    // load the schedule of recall from here
    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        bool flag = false;
        //find a block of the buff is NULL
        if (recallSchedule[i].sentence == NULL)
        {
            // prepare the sentence of 
            char * recall = malloc(sizeof(char)*100);
            sprintf(recall, "%s, route %s will come in %d minutes", name, busStruct[busIndex].RouteNo, timeOfWait);  

            //load the time to the time_t based on the Expected count down
            int expected_countdown;
            sscanf(busStruct[busIndex].schedule[scheduleIndex].ExpectedCountdown, "%d", &expected_countdown);
            expected_countdown *=60;
            time_t scheduledTime;
            time(&scheduledTime);
            scheduledTime += expected_countdown;

            //load the data to the sturcture
            recallSchedule[i].schedule_time = scheduledTime;
            recallSchedule[i].sentence = recall;
            flag = true;
        }
        if (!flag)
        {
            printf("Buffer is full\n"); // replace it to displaying
        }
    }
}