// Header File to use PWM
// Use the PWM to control the LED

#include "hal/usePWM.h"

void initialize_PWM()
{
    char buff[1024];
    // config PWM
    sprintf(buff, "config-pin %s pwm", PWM_GPIO);
    runCommand(buff);

    // enable the pwm
    sprintf(buff, "%sduty_cycle", PWM_PATH);
    writeToFile(buff, "0");
    writeToFile(PWM_PATH_enable, "1");
    printf("PWM init complete\n");
}

void PWM_buzz_hit()
{
    char buff[1024];
    writeToFile(PWM_PATH_duty_cycle, "0");
    sprintf(buff, "%d", 2000000);
    writeToFile(PWM_PATH_period, buff);
    sprintf(buff, "%d", 100000);
    writeToFile(PWM_PATH_duty_cycle, buff);
    usleep(3000);
}

void PWM_buzz_miss()
{
    char buff[1024];
    writeToFile(PWM_PATH_duty_cycle, "0");
    sprintf(buff, "%d", 2000000);
    writeToFile(PWM_PATH_period, buff);
    sprintf(buff, "%d", 300000);
    writeToFile(PWM_PATH_duty_cycle, buff);
    usleep(3000);
}
