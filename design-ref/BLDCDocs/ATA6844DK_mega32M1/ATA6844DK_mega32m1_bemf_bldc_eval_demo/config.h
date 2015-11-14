#ifndef __CONFIG_H
#define __CONFIG_H

#define VERSION         "V0.557alpha"

// uart baudrate
#define BAUDRATE 9600

// min and max values for duty cycle
#define DUTY_MAX 1000
#define DUTY_MIN 10

// default values for mcdrv_reset()
#define DEF_DUTYCYCLE           200
#define DEF_SETSPEED            1500
#define DEF_DELAY               700
#define DEF_SETPOINTDELAY       400

#endif // __CONFIG_H
