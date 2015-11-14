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
  Purpose: ADC low level driver and filter pipeline
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

#include "adc.h"
#include "mc_drv.h"
#include "timer1.h"

// for debug
#include "uart_32m1.h"


/*===========================================================================*/
/*  DEFINES                                                                  */
/*===========================================================================*/
typedef struct {
    unsigned char index;
    unsigned char data[4];
    unsigned char filtered;
} t_adc;

/*===========================================================================*/
/*  Modul Globals                                                            */
/*===========================================================================*/

/** \brief a
    local data array for median calculation
*/
volatile t_adc a[4];

/** \brief t_adc_state
    local data array index
*/
volatile unsigned char t_adc_state = 0;

/*===========================================================================*/
/*  IMPLEMENTATION                                                           */
/*===========================================================================*/

/** \brief t_adc_state_reset
    reset state index
    \param none
    \return none
*/
void t_adc_state_reset(void) {
    t_adc_state = 0;
}

/** \brief t_adc_configure
    reset pipeline data object
    \param a        : pointer to data structure
    \return none
*/
void t_adc_configure(t_adc *a) {
    a->index = 0;
    a->filtered = 0;
}

/** \brief t_adc_push
    push current ADC value in data object
    \param a        : pointer to data structure
    \return none
*/
void t_adc_push(t_adc *a) {
    a->data[a->index] = ADC_get();
    a->index += 1;
    a->index %= 4;
}

/** \brief t_adc_getfiltered
    get median of adc data object
    \param a        : pointer to data structure
    \return median value
*/
unsigned char t_adc_getfiltered(t_adc *a) {
    unsigned short ret = a->data[0]+a->data[1]+a->data[2]+a->data[3];
    ret /= 4;
    return (unsigned char)(ret);
}

/** \brief ADC_getfiltered
    get median array of all pipeline members
    \param s        : pointer to array uint8_t[4]
    \return none
*/
void ADC_getfiltered(unsigned char *s) {
    s[0] = t_adc_getfiltered((t_adc*)&a[0]);
    s[1] = t_adc_getfiltered((t_adc*)&a[1]);
    s[2] = t_adc_getfiltered((t_adc*)&a[2]);
    s[3] = t_adc_getfiltered((t_adc*)&a[3]);
}

/** \brief ADC_ISR
    trigger ADC conversion
    \param none
    \return current ADC state / number of measurements of this commutation period
*/
unsigned char ADC_ISR(void) {
    // trigger ADC conversion
    if (t_adc_state < 4) {
        ADC_start();
    } 
    return t_adc_state;
}

/** \brief ISR(ADC_vect)
    ADC interrupt service routine
    \param none
    \return none
*/
ISR(ADC_vect) {
    t_adc_push((t_adc*)&a[t_adc_state]);
    if (t_adc_state < 4)
        t_adc_state++;
    //t_adc_state %= 4;
}

/** \brief ADC_configure
    low level ADC initialization
    \param none
    \return none
*/
void ADC_configure(void) {
    // 2.56V internal ref / left adjust / amp1
    ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0);
    // enable adc / single shot mode / interrupt enable / presc 1/128
    ADCSRA |= (1<<ADEN)|(0<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS1);
    // no automatic trigger
    ADCSRB |= (0<<ADTS3)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
    // Disable digital input
    DIDR1 |= 0x03;

    // Amplifier configuration
    // enable / gain 10x /
    AMP1CSR |= (1<<AMP1EN)|(0<<AMP1G1)|(1<<AMP1G0);
    
    // reset structures
    t_adc_configure((t_adc*)&a[0]);
    t_adc_configure((t_adc*)&a[1]);
    t_adc_configure((t_adc*)&a[2]);
    t_adc_configure((t_adc*)&a[3]);    
}
