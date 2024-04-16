// Header File for useJoyStick.c
// Use the JoyStick to control and read the input from the JoyStick

#ifndef _USEJOYSTICK_H_
#define _USEJOYSTICK_H_

#define JOYSTICK_DIRECTION_LEFT "/sys/class/gpio/gpio65/direction"
#define JOYSTICK_EDGE_LEFT "/sys/class/gpio/gpio65/edge"
#define JOYSTICK_IN_LEFT "/sys/class/gpio/gpio65/value"

#define JOYSTICK_DIRECTION_RIGHT "/sys/class/gpio/gpio47/direction"
#define JOYSTICK_EDGE_RIGHT "/sys/class/gpio/gpio47/edge"
#define JOYSTICK_IN_RIGHT "/sys/class/gpio/gpio47/value"

#define JOYSTICK_DIRECTION_UP "/sys/class/gpio/gpio26/direction"
#define JOYSTICK_EDGE_UP "/sys/class/gpio/gpio26/edge"
#define JOYSTICK_IN_UP "/sys/class/gpio/gpio26/value"

#define JOYSTICK_DIRECTION_DOWN "/sys/class/gpio/gpio46/direction"
#define JOYSTICK_EDGE_DOWN "/sys/class/gpio/gpio46/edge"
#define JOYSTICK_IN_DOWN "/sys/class/gpio/gpio46/value"

#define JOYSTICK_DIRECTION_PRESS "/sys/class/gpio/gpio27/direction"
#define JOYSTICK_EDGE_PRESS "/sys/class/gpio/gpio27/edge"
#define JOYSTICK_IN_PRESS "/sys/class/gpio/gpio27/value"

#define JOYSTICK_GPIO_LEFT "p8.18"
#define JOYSTICK_GPIO_RIGHT "p8.15"
#define JOYSTICK_GPIO_UP "p8.14"
#define JOYSTICK_GPIO_DOWN "p8.16"
#define JOYSTICK_GPIO_PRESS "p8.17"

#define BUTTON_GPIO "p9.11"
#define BUTTON_DIRECTION "/sys/class/gpio/gpio30/direction"
#define BUTTON_EDGE "/sys/class/gpio/gpio30/edge"
#define BUTTON_VALUE "/sys/class/gpio/gpio30/value"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // Errors
#include <string.h>
#include <sys/epoll.h> // for epoll()
#include <fcntl.h>     // for open()
#include <unistd.h>    // for close()

#include "displaylogic.h"
#include "hal/threadsControl.h"
#include "hal/readNWrite.h"
#include "hal/usePWM.h"
// initialize the joy stick and set up the epoll function
void inititalize_JoyStick();

char waitForGpioEdge(char **fileNamesForGpioValue, int *waitMilliseconds, int numGpios);

// Thread function of using the joy stick with Epoll
void selectScheduleRecall(char *name);

#endif