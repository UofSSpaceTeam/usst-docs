/*C****************************************************************************
  Use of this software is subject to Atmel's Software License Agreement.
-------------------------------------------------------------------------------
  $URL: http://www.atmel.com $
  $LastChangedRevision: 0.557alpha $
  $LastChangedDate: 100705 $
  $LastChangedBy: markus ekler $
-------------------------------------------------------------------------------
  Project: mega32m1_bemf_bldc
  Target MCU: ATmega32M1
  Compiler: IAR & GCC
-------------------------------------------------------------------------------
  Purpose: bldc motor driver implementation and initialization
******************************************************************************
* Copyright 2010, Atmel Automotive GmbH                                       *
*                                                                             *
* This software is owned by the Atmel Automotive GmbH                         *
* and is protected by and subject to worldwide patent protection.             *
* Atmel hereby grants to licensee a personal,                                 *
* non-exclusive, non-transferable license to copy, use, modify, create        *
* derivative works of, and compile the Atmel Source Code and derivative       *
* works for the sole purpose of creating custom software in support of        *
* licensee product to be used only in conjunction with a Atmel integrated     *
* circuit as specified in the applicable agreement. Any reproduction,         *
* modification, translation, compilation, or representation of this           *
* software except as specified above is prohibited without the express        *
* written permission of Atmel.                                                *
*                                                                             *
* Disclaimer: ATMEL MAKES NO WARRANTY OF ANY KIND,EXPRESS OR IMPLIED,         *
* WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED    *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.         *
* Atmel reserves the right to make changes without further notice to the      *
* materials described herein. Atmel does not assume any liability arising     *
* out of the application or use of any product or circuit described herein.   *
* Atmel does not authorize its products for use as critical components in     *
* life-support systems where a malfunction or failure may reasonably be       *
* expected to result in significant injury to the user. The inclusion of      *
* Atmel products in a life-support systems application implies that the       *
* manufacturer assumes all risk of such use and in doing so indemnifies       *
* Atmel against all charges.                                                  *
*                                                                             *
* Use may be limited by and subject to the applicable Atmel software          *
* license agreement.                                                          *
******************************************************************************/
/*===========================================================================*/
/*  INCLUDES                                                                 */
/*===========================================================================*/
#include "compiler.h"
#include "config.h"

#include "timer1.h"
#include "anacomp.h"
#include "adc.h"
#include "mc_drv.h"
#include "mc_sensor.h"


/*===========================================================================*/
/*  Modul Globals                                                            */
/*===========================================================================*/
/** \brief mc
    local motor control structure
*/
volatile t_mc mc;

/** \brief debug_mode
    debug mode variable
*/
unsigned char debug_mode = 0;

/** \brief sw_cnt
    timeout handling for automatic state transition from unlocked -> prelocked1
*/
unsigned short sw_cnt = 0;
#define TIMEOUT()       ((sw_cnt&0x000F)==0)

/** \brief nexthall_xxx
    timeout handling for automatic state transition from unlocked -> prelocked1
*/
// flash array to calculate next hall sensor
// ............................0.....1.....2......3......4......5......6......7
prog_char nexthall_cw[8]  = { 0xFF,HS_101,HS_011,HS_001,HS_110,HS_100,HS_010,0xFF };
prog_char nexthall_ccw[8] = { 0xFF,HS_011,HS_110,HS_010,HS_101,HS_001,HS_100,0xFF };

/*===========================================================================*/
/*  IMPLEMENTATION                                                           */
/*===========================================================================*/

/** \brief mcdrv_getnexths
    get next virtual hall sensor depending on rotation direction
    \param hs           : current virtual hall sensor
    \return next hall sensor
*/
unsigned char mcdrv_getnexths(unsigned char hs) {
    unsigned char ret = 0;
    hs %= 8;
    if (mc.direction == DIR_CW) {
        ret = pgm_read_byte(nexthall_cw+hs);
    } else {
        ret = pgm_read_byte(nexthall_ccw+hs);
    }
    return ret;
}

//
// REMARK: 
// The DEBUG_ functions below are not mandatory for stand alone motor operation
// but they are quite useful for displaying mcdrv data and evaluating motors

void DEBUG_enable(unsigned char en) {   
    debug_mode = en;
}

unsigned char DEBUG_mcdrv_getstate(void) {
    return mc.state;
}

void DEBUG_mcdrv_setstate(unsigned char c) {
    mc.state = c;
}

unsigned short DEBUG_mcdrv_getdelay(void) {
    return mc.next_delay;
}

void DEBUG_mcdrv_setdelay(unsigned short s) {
    mc.next_delay = s;
}

unsigned short DEBUG_mcdrv_getmeasuredrpm(void) {
    return mc.U16_measured_speed_rpm;
}

unsigned short DEBUG_mcdrv_getduty(void) {
    return mc.dutycycle;
}   

/** \brief mcdrv_setspeed
    set setpoint for pi regulator
    \param s            : setpoint in RPM
    \return none
*/
void mcdrv_setspeed(unsigned short s) {
    mc.setspeed = s;
} 

/** \brief mcdrv_getsetspeed
    get setpoint for pi regulator
    \param none
    \return setpoint in RPM
*/
unsigned short mcdrv_getsetspeed(void) {
    return mc.setspeed;
}

/** \brief mcdrv_setdutycycle
    set duty cycle
    \param d            : new duty cylce
    \return none
*/
void mcdrv_setdutycycle(unsigned short d) {
    if (d >= DUTY_MAX)
        mc.dutycycle = DUTY_MAX;
    else if (d <= DUTY_MIN) 
        mc.dutycycle = DUTY_MIN;
    else 
        mc.dutycycle = d;
}

/** \brief mcdrv_configure
    low level initialization for psc and related peripherals
    \param none
    \return none
*/
void mcdrv_configure(void) {
    // init PLL 64MHz
    PLL_start64();
    // wait until pll is locked
    while (!PLL_islocked());

    // init PSC     
    POCR0RA = 0;              // register not used in center aligned mode     
    POCR1RA = 0;              // register not used in center aligned mode      
    POCR2RA = 0;              // register not used in center aligned mode

    // initial values for psc registers
    POCR_RB = 0x03FF;

    POCR0SA = 0x0000;        
    POCR1SA = 0x0000;
    POCR2SA = 0x0000;
    POCR0SB = 0x0000;    
    POCR1SB = 0x0000;
    POCR2SB = 0x0000;
    
    // inverted highside stage on highside fets
#ifdef _DEBUG_ATA6834
    // ata6834
    PCNF |= (1<<POPA);
#else
    // ata6844
    PCNF |= (0<<POPA);
#endif
    PCNF |= (1<<POPB);

    // center aligned mode, PSC output high active
    PCNF |= (1<<PMODE);
    // start PSC, 64MHz source, no prescaler
    PCTL |= /*(1<<PRUN)|*/(1<<PCLKSEL)|(0<<PPRE1)|(0<<PPRE0);

    // disable overlap function
    PMIC0 |= (1<<POVEN0);
    PMIC1 |= (1<<POVEN1);
    PMIC2 |= (1<<POVEN2);
}

/** \brief mcdrv_reset
    reset of mc state machine / restore default values
    \param none
    \return none
*/
void mcdrv_reset(void) {
    mc.state = MC_UNLOCKED;
    mc.com_hs = HS_101;
    mc.direction = DIR_CW;
    mc.dutycycle = DEF_DUTYCYCLE;
    mc.setspeed = DEF_SETSPEED;
    mc.current_delay = DEF_DELAY;
    mc.next_delay = DEF_DELAY;
    mc.setpoint_delay = DEF_SETPOINTDELAY;
    mc.timeout_cnt = 0;
    mc.delta4adc = 0;
    mc.S16_last_speed_error = 0;
}

/** \brief mcdrv_switch_commutation
    switch power stage according to rotation direction and current, virtual hall sensor
    \param pos          : virtual hall sensor state
    \return none
*/
void mcdrv_switch_commutation(unsigned char pos) {

    POCR0SA = mc.dutycycle;        
    POCR2SA = mc.dutycycle;
    POCR1SA = mc.dutycycle;        
    POCR1SB = 0x000;
    POCR0SB = 0x000;      
    POCR2SB = 0x000;

    if (mc.direction == DIR_CW) {
        switch (pos) {
            case HS_001: Set_Q5Q2(); break;
            case HS_101: Set_Q5Q4(); break;
            case HS_100: Set_Q1Q4(); break;
            case HS_110: Set_Q1Q6(); break;
            case HS_010: Set_Q3Q6(); break;
            case HS_011: Set_Q3Q2(); break;
            default: break;
        }
    } else {
        switch (pos) {
            case HS_001: Set_Q1Q6(); break;
            case HS_101: Set_Q3Q6(); break;
            case HS_100: Set_Q3Q2(); break;
            case HS_110: Set_Q5Q2(); break;
            case HS_010: Set_Q5Q4(); break;
            case HS_011: Set_Q1Q4(); break;
            default: break;
        }
    }
}

/** \brief mcdrv_start
    start motor
    \param dir          : rotation direction
    \return none
*/
void mcdrv_start(unsigned char dir) {
    
    // set direction in control structure
    mc.direction = dir;
    
    // start commutation isr
    TIMER1_setOCRA(mc.current_delay);
    TIMER1_ieoca(1);

    // start adc isr
    TIMER1_ocbclearflag();
    TIMER1_setOCRB(mc.current_delay+10); // should not trigger befor ocra
    TIMER1_ieocb(1);
    
    // start psc
    PCTL |= (1<<PRUN);    
}

/** \brief mcdrv_stop
    stop motor
    \param none
    \return none
*/
void mcdrv_stop(void) {
    
    // kill commutation & adc isr
    TIMER1_ieoca(0);
    TIMER1_ieocb(0);
    
    // stop psc
    PCTL &= ~(1<<PRUN);
    
    // reset mc state
    mc.state = MC_UNLOCKED;
}

/** \brief mcdrv_calc_rpm
    calculate RPM from given delay
    \param U32_date         : input delay
    \return none
*/
unsigned short mcdrv_calc_rpm(unsigned short U32_date) {
    unsigned long int U32_buffer1 = 0; 
    unsigned short U16_buffer2 = 1;

    if(U32_date >= (unsigned short)62500) { //minimum possible speed is 1 round per second
        return 1;
    } else {
        if(U32_date != 0) {
            U32_buffer1 = U32_date;
            do { // accelerate the caculation by dint of multiplication with 2
                U32_buffer1 = U32_buffer1 << 1; 
                U16_buffer2 = U16_buffer2 << 1;
            } while(U32_buffer1 < (unsigned long int)3750000); 
      
            U32_buffer1 = U32_buffer1 >> 1; 
            U16_buffer2 = U16_buffer2 >> 1;
      
            for ( U32_buffer1 = U32_buffer1; U32_buffer1 < (unsigned long int)3750000; U32_buffer1 = U32_buffer1 + U32_date ) { //rest wird auf summiert
                U16_buffer2++;
            }
        } else {
            return 0; //no speed    
        }
    }
    return U16_buffer2;
}

/** \brief mcdrv_control_speed
    pi regulator implementation
    \param none
    \return none
*/
void mcdrv_control_speed(void) {
    unsigned short U16_new_duty_cycle = 0;
    signed short S16_speed_error = 0;
    // moved to mc structure for optimization
    //unsigned short U16_measured_speed_rpm;
  
    mc.U16_measured_speed_rpm = mcdrv_calc_rpm(mcsensor_getmedian());

    // Error calculation
    S16_speed_error = mc.setspeed - mc.U16_measured_speed_rpm;
    if( S16_speed_error > 1000 ) {
        S16_speed_error = 1000;
    }
    S16_speed_error = S16_speed_error >> 2; 
  
    //PI control for fast motion 
    U16_new_duty_cycle = mc.dutycycle + (S16_speed_error>>2) - (mc.S16_last_speed_error>>3);
    mc.S16_last_speed_error = S16_speed_error;
    
    mcdrv_setdutycycle(U16_new_duty_cycle);
}

/** \brief mc_task
    motor control task
    \param none
    \return none
*/
void mc_task(void) {
    unsigned char adc_value[4];
    unsigned short med,min,max;

    ADC_getfiltered(adc_value);    

    // mc task implementation
    switch (mc.state) {
        case MC_UNLOCKED:
            if (!debug_mode) {
                sw_cnt++;
                if (TIMEOUT()) {
                    mc.state = MC_PRELOCKED1;
                }
            }
            break;
        case MC_RAMPUP:
#warning EXPERIMENTAL RAMP UP NOT IMPLEMENTED IN THIS RELEASE
        break;
        case MC_PRELOCKED1:
            // transition state
            //  - enable AC interrupts and wait for next taskcall to settle interrupt loop
            //  - set next state
            AC_clearflags();
            AC0_ie(1);
            AC1_ie(1);
            AC2_ie(1);
            mc.state = MC_PRELOCKED2;
            break;
        case MC_PRELOCKED2:
            med = mcsensor_getmedian();
            min = mc.current_delay*4;
            max = min*2;
            
            if (med >= min && med <= max) 
                mc.state = MC_PRELOCKED3;
            break;
        case MC_PRELOCKED3:
            // transition state
            //  - transition to locked state in ISR to create a defined timeslot for transition
            break;
        case MC_LOCKED:
            mcdrv_control_speed();
            break;
    }
}

/** \brief ISR(TIMER1_COMPA_vect)
    interrupt service routine for commutation
    \param none
    \return none
*/
ISR(TIMER1_COMPA_vect) {
    // commutation
    mc.com_hs = mcdrv_getnexths(mc.com_hs);
    mcdrv_switch_commutation(mc.com_hs);

    // time and state handling
    switch (mc.state) {
        case MC_UNLOCKED:
        case MC_RAMPUP:
        case MC_PRELOCKED1:
        case MC_PRELOCKED2:
            mc.current_delay = mc.next_delay;
            mc.delta4adc = mc.current_delay/4;
            break;
        case MC_PRELOCKED3:
            mc.state = MC_LOCKED;            
            break;
        case MC_LOCKED:
        default:
            break;
    }
    // next delay for non-locked modes and fallback in locked mode
    TIMER1_setOCRA(mc.next_delay);
    
    // ADC trigger
    TIMER1_setOCRB(5);

    // configure adc statemachine
    // definierter adc state
    t_adc_state_reset();
    // ADC flag löschen
    ADC_clearflag();
    // start first adc conversion
    ADC_start();

}


/** \brief ISR(TIMER1_COMPB_vect)
    interrupt service routine for adc triggering and safety functions
    \param none
    \return none
*/
ISR(TIMER1_COMPB_vect) {
    // set next timing for COMPB
    //TIMER1_setOCRB(delta);
    OCR1B = TCNT1 + mc.delta4adc;
    
    // call ADC handler
    if (ADC_ISR() >= 4) {
        mc.timeout_cnt++;
        if (mc.timeout_cnt >= 2) 
            // if commutation is missed fallback to UNLOCKED mode
            mc.state = MC_UNLOCKED;
    } else {
        mc.timeout_cnt = 0;
    }
}




// MC_SENSOR ISRs


// these two variables are only mandatory for sensor isr
// todo: move to structure or somewhere else
volatile unsigned char lasths = HS_101;
volatile unsigned short lastts = 0;


/** \brief ACISR
    common isr for analog comp handling
    \param none
    \return none
*/
void ACISR(void) {
    unsigned char s;
    unsigned short t1 = TCNT1;

    s = GET_HS();
    
    // is valid?
    if (mcdrv_getnexths(lasths) == s) {

        lasths = s;
        acpipe_push(s,t1);
        if (mc.state == MC_LOCKED) {
            OCR1A = t1 + ((t1-lastts)>>1);
            mc.delta4adc = (t1-lastts);
        }
        lastts = t1;
    } 
}


ISR(ANACOMP0_vect) {
    ACISR();
}

ISR(ANACOMP1_vect) {
    ACISR();
}

ISR(ANACOMP2_vect) {
    ACISR();
}

