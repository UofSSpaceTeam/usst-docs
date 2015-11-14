/*C****************************************************************************
  Use of this software is subject to Atmel's Software License Agreement.
-------------------------------------------------------------------------------
  $URL: http://www.atmel.com $
  $LastChangedRevision: 0.557alpha $
  $LastChangedDate: 110207 $
  $LastChangedBy: markus ekler $
-------------------------------------------------------------------------------
  Project: mega32m1_bemf_bldc
  Target MCU: ATmega32M1
  Compiler: IAR & GCC
-------------------------------------------------------------------------------
  Purpose: ATA6844-DK demonstrator software
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

#include "adc.h"
#include "anacomp.h"
#include "timer1.h"
#include "dac.h"

#include "ata6844.h"
#include "mc_drv.h"
#include "mc_sensor.h"

#include "sw_timer.h"
#include "uart_32m1.h"

/*===========================================================================*/
/*  DEFINES                                                                  */
/*===========================================================================*/
/** \brief LED_xxx
    status led on ATA6844-DK
*/
#define LED_configure()     (DDRB |=   (1<<PORTB4))
#define LED_on()            (PORTB |=  (1<<PORTB4))
#define LED_off()           (PORTB &= ~(1<<PORTB4))
#define LED_toggle()        (PINB |=   (1<<PORTB4))
#define LED(b)              ((b)?(LED_on()):(LED_off()))

/*===========================================================================*/
/*  Modul Globals                                                            */
/*===========================================================================*/
/** \brief SWT
    local software timer structure
*/
volatile t_swtimer SWT;

/*===========================================================================*/
/*  IMPLEMENTATION                                                           */
/*===========================================================================*/
/** \brief main
    program entry point
    \param none
    \return none
*/
int main() {

    unsigned char cmd;

    unsigned char uart_getnumber = 0;
    unsigned int uart_i = 0;

    // ATA6844 driver stage configuration
    ATA6844_init_port();
    // reset DG1 pin before init of alerts
    ATA6844_reset_DG1();
    
    // status led
    LED_configure();
    LED(0);
    
    // ADC
    ADC_configure();
    ADC_start();

    // control interface
    uart_init();
    uart_puts_P(PSTR("\r\nATMEL ATA6844-DK Demonstrator "));
    uart_puts(VERSION);
    uart_puts_P(PSTR("\r\n"));

    
    // motor control
    mcdrv_configure();
    mcsensor_configure();
    mcdrv_reset();
    
    // software timer
    swt_configure((t_swtimer*)&SWT);

    // timer1
    TIMER1_start();
    
    // enable interrupts
    sei();


    // configuration for autostart
    SWT2_setdelayms(1000);
    
    while (1) {

        uart_task();
      
        if (SWT1_timeout()) {
            SWT1_setdelayms(10);

            mc_task();
            
            // status led handling
            LED(DEBUG_mcdrv_getstate()==MC_LOCKED);
        }

        mcsensor_calc();

        // me-shell
        if (uart_received()) {
            cmd = uart_getc();

            if (uart_getnumber) {
                uart_putc(cmd);
                if (cmd == '\r') {
                    uart_putc('\n');
                    switch (uart_getnumber) {
                        case 1:                            
                            // set set_speed
                            uart_puts_P(PSTR("NEW SPEED:"));
                            uart_puti(uart_i);
                            mcdrv_setspeed(uart_i);
                            uart_puts_P(PSTR("\r\n"));
                            break;
                        case 2:
                            // set scref
                            uart_puts_P(PSTR("NEW SCREV:"));
                            uart_puti(uart_i);
                            DAC_set((unsigned char)uart_i);
                            uart_puts_P(PSTR("\r\n"));
                            break;
                    }
                    uart_getnumber = 0;
                    continue;
                } else if (cmd >= '0' && cmd <= '9') {
                    uart_i *= 10;
                    uart_i += cmd-'0';
                } else {
                    uart_getnumber = 0;
                    uart_puts_P(PSTR("ERR\r\n"));
                    continue;
                }
            }

            
            switch (cmd) {
                case 'r':
                    uart_puts_P(PSTR("RUN (clockwise)\r\n"));
                    mcdrv_stop();
                    mcdrv_reset();
                    mcsensor_reset();                    
                    ATA6844_ncoast(1);
                    mcdrv_start(DIR_CW);
                    break;
                case 'R':
                    uart_puts_P(PSTR("RUN (counterclockwise)\r\n"));
                    mcdrv_stop();
                    mcdrv_reset();
                    mcsensor_reset();
                    ATA6844_ncoast(1);
                    mcdrv_start(DIR_CCW);
                    break;
                case 's':
                    uart_puts_P(PSTR("STOP\r\n"));
                    mcdrv_stop();                    
                    break;
                case 'd':
                    uart_puts_P(PSTR("DEBUG on\r\n"));
                    DEBUG_enable(1);
                    break;
                case 'D':
                    uart_puts_P(PSTR("DEBUG off\r\n"));
                    DEBUG_enable(0);
                    break;
                case '?':                    
                    uart_puts_P(PSTR("DG3: "));
                    uart_putc(((ATA6844_getdg()&DG3)?('1'):('0')));
                    uart_puts_P(PSTR(" (Overtemperature)\r\n"));
                    uart_puts_P(PSTR("DG2: "));
                    uart_putc(((ATA6844_getdg()&DG2)?('1'):('0')));
                    uart_puts_P(PSTR(" (Voltage)\r\n"));
                    uart_puts_P(PSTR("DG1: "));
                    uart_putc(((ATA6844_getdg()&DG1)?('1'):('0')));
                    uart_puts_P(PSTR(" (Shortcircuit)\r\n"));
                    break;
                case 'l':
                    uart_puts_P(PSTR("Force state MC_LOCKED.\r\n"));
                    DEBUG_mcdrv_setstate(MC_LOCKED);
                    break;
                case 'p':
                    uart_puts_P(PSTR("Force state MC_PRELOCKED1.\r\n"));
                    DEBUG_mcdrv_setstate(MC_PRELOCKED1);
                    break;
                case 'i':
                    uart_puts_P(PSTR("Enable AC interrupts.\r\n"));
                    AC_clearflags();
                    AC0_ie(1);
                    AC1_ie(1);
                    AC2_ie(1);
                    break;
                case 'I':
                    uart_puts_P(PSTR("Disable AC interrupts.\r\n"));
                    AC0_ie(0);
                    AC1_ie(0);
                    AC2_ie(0);
                    break;
                case '+':
                    DEBUG_mcdrv_setdelay(DEBUG_mcdrv_getdelay()+1);
                    uart_puts_P(PSTR("DELAY: "));
                    uart_puti(DEBUG_mcdrv_getdelay());
                    uart_puts_P(PSTR("\r\n"));
                    break;
                case '-':
                    DEBUG_mcdrv_setdelay(DEBUG_mcdrv_getdelay()-1);
                    uart_puts_P(PSTR("DELAY: "));
                    uart_puti(DEBUG_mcdrv_getdelay());
                    uart_puts_P(PSTR("\r\n"));
                    break;
                case '*':
                    mcdrv_setdutycycle(DEBUG_mcdrv_getduty()+1);
                    uart_puts_P(PSTR("PWM: "));
                    uart_puti(DEBUG_mcdrv_getduty());
                    uart_puts_P(PSTR("\r\n"));
                    break;
                case '_':
                    mcdrv_setdutycycle(DEBUG_mcdrv_getduty()-1);
                    uart_puts_P(PSTR("PWM: "));
                    uart_puti(DEBUG_mcdrv_getduty());
                    uart_puts_P(PSTR("\r\n"));
                    break;
                case ' ':
                    uart_puts_P(PSTR("RPM: "));
                    uart_puti(DEBUG_mcdrv_getmeasuredrpm());
                    uart_puts_P(PSTR(" / SET: "));
                    uart_puti(mcdrv_getsetspeed());
                    uart_puts_P(PSTR("\r\n"));
                    break;
                case 'S':
                    uart_getnumber = 1;
                    uart_i = 0;
                    uart_puts_P(PSTR("SETPOINT:"));
                    break;
                case 'v':
                    uart_getnumber = 2;
                    uart_i = 0;
                    uart_puts_P(PSTR("SCREF ():"));
                    break;
                case 'c':
                    // emergency shutdown of motor
                    ATA6844_ncoast(0);
                    mcdrv_stop();
                    uart_puts_P(PSTR("COAST activated.\r\n"));
                    break;
                case 'C':
                    // release coast
                    ATA6844_ncoast(1);
                    uart_puts_P(PSTR("COAST released.\r\n"));
                    break;
#warning EXPERIMENTAL RAMP UP NOT IMPLEMENTED IN THIS RELEASE
/*                case 'x':
                    // experimental ramp up 
                    uart_puts_P(PSTR("EXPERIMENTAL ramp-up (clockwise)\r\n"));
                    mcdrv_reset();
                    mcdrv_stop();                    
                    DEBUG_mcdrv_setstate(MC_RAMPUP);
                    mcsensor_reset();                    
                    ATA6844_ncoast(1);
                    mcdrv_start(DIR_CW);
                    break;
                case 'X':
                    // experimental ramp up 
                    uart_puts_P(PSTR("EXPERIMENTAL ramp-up (counterclockwise)\r\n"));
                    mcdrv_reset();
                    mcdrv_stop();                    
                    DEBUG_mcdrv_setstate(MC_RAMPUP);
                    mcsensor_reset();                    
                    ATA6844_ncoast(1);
                    mcdrv_start(DIR_CCW);
                    break;*/
            }
        }
    }

    return 0;
}


