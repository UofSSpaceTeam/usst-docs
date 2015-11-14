#ifndef __MC_DRV_H
#define __MC_DRV_H


// PLL handling
#define PLL_start64()           (PLLCSR = 0x06) // 64MHz
#define PLL_start32()           (PLLCSR = 0x02) // 32MHz

#define PLL_islocked()          (PLLCSR & (1<<PLOCK))
//#define PLL_ready()             while(!PLL_islocked())
#define PLL_stop()              (PLLCSR = 0x00)


// PSC handling
#define Set_Q1Q4()      POC = (0<<POEN0A)|(0<<POEN0B)|(0<<POEN1A)|(1<<POEN1B)|(1<<POEN2A)|(0<<POEN2B);
#define Set_Q1Q6()      POC = (0<<POEN0A)|(1<<POEN0B)|(0<<POEN1A)|(0<<POEN1B)|(1<<POEN2A)|(0<<POEN2B);
#define Set_Q3Q2()      POC = (0<<POEN0A)|(0<<POEN0B)|(1<<POEN1A)|(0<<POEN1B)|(0<<POEN2A)|(1<<POEN2B);
#define Set_Q3Q6()      POC = (0<<POEN0A)|(1<<POEN0B)|(1<<POEN1A)|(0<<POEN1B)|(0<<POEN2A)|(0<<POEN2B);
#define Set_Q5Q2()      POC = (1<<POEN0A)|(0<<POEN0B)|(0<<POEN1A)|(0<<POEN1B)|(0<<POEN2A)|(1<<POEN2B);
#define Set_Q5Q4()      POC = (1<<POEN0A)|(0<<POEN0B)|(0<<POEN1A)|(1<<POEN1B)|(0<<POEN2A)|(0<<POEN2B);

// virtual hall signal definition
#define HS_001  1
#define HS_010  2
#define HS_011  3
#define HS_100  4
#define HS_101  5
#define HS_110  6 

// definition for local motor control structure
// access to structures is optimized in compiler so using this structure is prefered compared to single variables ... besides to that, it looks better ;)
typedef struct {
    unsigned char state;
    unsigned char com_hs;
    unsigned char direction;
    unsigned short dutycycle;
    unsigned short setspeed;
    // for unlocked mode
    unsigned short current_delay;
    unsigned short next_delay;
    unsigned short setpoint_delay;
    // for failure detection
    unsigned char timeout_cnt;
    // for current measurement durring ramp up
    unsigned short delta4adc;
    // for pi speed control
    signed short S16_last_speed_error;
    unsigned short U16_measured_speed_rpm;
} t_mc;

// state values
#define MC_UNLOCKED     0
#define MC_RAMPUP       1
#define MC_PRELOCKED1   2 
#define MC_PRELOCKED2   3
#define MC_PRELOCKED3   4
#define MC_LOCKED       5

// direction values
#define DIR_CW      0       // direction = clockwise
#define DIR_CCW     1       // direction = counter clockwise

// motor control prototypes
void mcdrv_configure(void);
void mcdrv_reset(void);
void mcdrv_setspeed(unsigned short s);
void mcdrv_switch_commutation(unsigned char U8_position);
void mcdrv_start(unsigned char dir);
void mcdrv_stop(void);

void mc_task(void);

// DEBUG prototypes - these are not really mandatory for motor control but ease the way to find the right parameters for an unknown motor in unlocked mode
// TODO: introduce get-t_mc-struct function instead of those small DEBUG-functions
void DEBUG_enable(unsigned char en);
unsigned char DEBUG_mcdrv_getstate(void);
void DEBUG_mcdrv_setstate(unsigned char c);
unsigned short DEBUG_mcdrv_getdelay(void);
void DEBUG_mcdrv_setdelay(unsigned short s);
unsigned short DEBUG_mcdrv_getmeasuredrpm(void);
unsigned short DEBUG_mcdrv_getduty(void);
void mcdrv_setdutycycle(unsigned short d);
unsigned short mcdrv_getsetspeed(void);

#endif /* __MC_DRV_H */ 
