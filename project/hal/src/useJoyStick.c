#include "hal/useJoyStick.h"

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

void *useJoystick()
{
    
    char *listOfDetection[] = {
        JOYSTICK_IN_PRESS,
        JOYSTICK_IN_UP,
        JOYSTICK_IN_DOWN,
        JOYSTICK_IN_LEFT,
        JOYSTICK_IN_RIGHT,
    };
    int waitTimes[] = {-1, -1, -1, -1, -1};
    waitForGpioEdge(listOfDetection, waitTimes, 5);
    while (!ifShutdown())
    {
        // if ((triggeredGpios >> 1) & 1) // fire, judge and count down
        // {
        //     if (shootTarget())
        //     {
        //         hit_number++;
        //         setNumber(hit_number);
        //         PWM_buzz_hit();
        //         generateNewTarget();
        //     }
        //     else
        //     {
        //         PWM_buzz_miss();
        //     }
        // }
        // if ((triggeredGpios >> 3) & 1)
        // {
        //     signalShutdown();
        //     break;
        // }
        waitForGpioEdge(listOfDetection, waitTimes, 5);
    }
    printf("JoyStick Shutdown\n");
    return NULL;
}