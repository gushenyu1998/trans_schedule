// Header File to use PWM
// Use the PWM to control the LED

#ifndef _LED_H_
#define _LED_H_
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // Errors
#include <string.h>
#include <unistd.h>
#include "hal/readNWrite.h"
#include "hal/threadsControl.h"

#define PWM_GPIO "p9_22"
#define PWM_PATH "/dev/bone/pwm/0/a/"
#define PWM_PATH_enable "/dev/bone/pwm/0/a/enable"
#define PWM_PATH_duty_cycle "/dev/bone/pwm/0/a/duty_cycle"
#define PWM_PATH_period "/dev/bone/pwm/0/a/period"
#define PWM_CHIP_CH_A "/sys/class/pwm/pwmchip3"

#define AIN0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw" 

#define MAX_Frequency 40 //100hz in example
#define MAX_AIN 4096
#define MAX_Votage 1.88

/**
 * Set up the GPIO and disable channel A for using PWM in channel B
*/
void initialize_PWM();

#endif