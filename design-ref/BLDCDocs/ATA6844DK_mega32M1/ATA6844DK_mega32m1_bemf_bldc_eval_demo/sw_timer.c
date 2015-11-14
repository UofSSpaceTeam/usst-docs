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
  Purpose: software timer implementation for basic scheduler and delays
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

#include "sw_timer.h"

/*===========================================================================*/
/*  DEFINES                                                                  */
/*===========================================================================*/
//  presc 1/64
#define TIMER0_start()          (TCCR0B|=(0<<CS02)|(1<<CS01)|(1<<CS00))
#define TIMER0_stop()           (TCCR0B&=0xF8)

/*===========================================================================*/
/*  Modul Globals                                                            */
/*===========================================================================*/
/** \brief lp_swt
    local pointer to software timer structure
*/
volatile t_swtimer *lp_swt;

/** \brief swt_configure
    low level software timer initialization
    \param swt          : software timer structure
    \return none
*/
void swt_configure(t_swtimer *swt) {
    // TIMER0 configuration
    //   clear timer on compare match
    TCCR0A |= (1<<WGM01);
    //   set overflow value for 250us interrupts @ 16MHz
    OCR0A = 62;
    //   enable interrupt
    TIMSK0 |= (1<<OCIE0A);

    // configure global structure
    lp_swt = swt;
    lp_swt->sw_timer1 = 0;
    lp_swt->sw_timer2 = 0;
    
    // start timer0
    TIMER0_start();
}

/** \brief ISR(TIMER0_COMPA_vect)
    interrupt service routine
    \param none
    \return none
*/
ISR(TIMER0_COMPA_vect) {
    if (lp_swt->sw_timer1 != 0) 
        lp_swt->sw_timer1 -= 1;
    if (lp_swt->sw_timer2 != 0) 
        lp_swt->sw_timer2 -= 1;
}
