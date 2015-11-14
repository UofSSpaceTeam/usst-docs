/*C****************************************************************************
  Use of this software is subject to Atmel's Software License Agreement.
-------------------------------------------------------------------------------
  $URL: http://www.atmel.com $
  $LastChangedRevision: 0.557alpha $
  $LastChangedDate: 101212 $
  $LastChangedBy: markus ekler $
-------------------------------------------------------------------------------
  Project: mega32m1_bemf_bldc
  Target MCU: ATmega32M1
  Compiler: IAR & GCC
-------------------------------------------------------------------------------
  Purpose: ATA6844 initialization 
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

#include "ata6844.h"

/*===========================================================================*/
/*  IMPLEMENTATION                                                           */
/*===========================================================================*/
/** \brief ATA6844_init_port
    port initialization for ATA6844-DK
    \param none
    \return none
*/
void ATA6844_init_port(void) {
    // ncoast pin
    ATA6844_ncoast_configure();
    ATA6844_ncoast(1);
    
    // sleep pin
    ATA6844_sleep_configure();
    ATA6844_sleep(0);
    
  
    // DG1-3 set mcu pins as input
    DDRC &= ~((1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3));

    // PWM outputs hardwired by PSC
    // PD0 = IH1 (low active)
    // PB7 = IL1
    // PC0 = IH2 (low active) 
    // PB6 = IL2
    // PB0 = IH3 (low active)
    // PB1 = IL3
    
    // set as output
    DDRB |= (1<<DDB7)|(1<<DDB6)|(1<<DDB1)|(1<<DDB0);
    DDRC |= (1<<DDC0);
    DDRD |= (1<<DDD0);

    // default value
    PORTB &= ~((1<<PORTB7)|(1<<PORTB6)|(1<<PORTB1));
#ifdef _DEBUG
    PORTD &= ~(1<<PORTD0);
    PORTC &= ~(1<<PORTC0);
    PORTB &= ~(1<<PORTB0);
#else
    PORTD |= (1<<PORTD0);
    PORTC |= (1<<PORTC0);
    PORTB |= (1<<PORTB0);
#endif 
}

/** \brief ATA6844_reset_DG1
    release power stage
    \param none
    \return none
*/
void ATA6844_reset_DG1(void) {
    // before the initialisation of the diagnostic alerts for short circiut detection
    // and/or voltage failure, the DG1 pin must reset by a rising edge on IL1
    // execute at first ATA6844_init_port in order to setup IL1
  
    PORTB |= (1<<PINB7);
    PORTB &= ~(1<<PINB7);
}
