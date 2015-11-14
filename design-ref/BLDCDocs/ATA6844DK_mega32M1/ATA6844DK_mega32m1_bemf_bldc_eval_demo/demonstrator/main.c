/*C****************************************************************************
  Use of this software is subject to Atmel's Software License Agreement.
-------------------------------------------------------------------------------
  $URL: http://www.atmel.com $
  $LastChangedRevision: 0.557alpha $
  $LastChangedDate: 110328 $
  $LastChangedBy: markus ekler $
-------------------------------------------------------------------------------
  Project: demonstrator
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
/*  Module Globals                                                           */
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
    
    unsigned char countdown = 5;

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


    // countdown for autostart
	countdown = 6;
	while (countdown > 1) {
		uart_task();

		if (SWT2_timeout()) {
			SWT2_setdelayms(1000);
			countdown--;
			uart_puts_P(PSTR("Starting motor in "));
			uart_putc('0'+countdown);
			uart_puts_P(PSTR(" seconds\r\n"));
		}
	}
	uart_puts_P(PSTR("Starting motor...\r\n"));
	mcdrv_reset();
	mcsensor_reset();
	ATA6844_ncoast(1);
    mcdrv_start(DIR_CW);

	// set ADC for poti input
	ADMUX = 0xE0;
    
    while (1) {

        uart_task();
        
        // motor control task
		if (SWT1_timeout()) {
            SWT1_setdelayms(10);

            mc_task();

			// set setpoint according to adc value
			mcdrv_setspeed(1500+(((unsigned short)ADC_get())*10));
            
            // status led handling
            LED(DEBUG_mcdrv_getstate()==MC_LOCKED);
        }

		// rpm output
		if (SWT2_timeout()) {
			SWT2_setdelayms(500);

			uart_puts_P(PSTR("RPM: "));
            uart_puti(DEBUG_mcdrv_getmeasuredrpm());
            uart_puts_P(PSTR(" / SET: "));
            uart_puti(mcdrv_getsetspeed());
            uart_puts_P(PSTR("\r"));

		}

        mcsensor_calc();

    }

    return 0;
}



