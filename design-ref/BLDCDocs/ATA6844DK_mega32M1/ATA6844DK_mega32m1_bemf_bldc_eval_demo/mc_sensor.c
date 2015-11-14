/*C****************************************************************************
  Use of this software is subject to Atmel's Software License Agreement.
-------------------------------------------------------------------------------
  $URL: http://www.atmel.com $
  $LastChangedRevision: 0.557alpha $
  $LastChangedDate: 100718 $
  $LastChangedBy: markus ekler $
-------------------------------------------------------------------------------
  Project: mega32m1_bemf_bldc
  Target MCU: ATmega32M1
  Compiler: IAR & GCC
-------------------------------------------------------------------------------
  Purpose: back emf sensor implementation and pipeline handler
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

#include "anacomp.h"
#include "timer1.h"
#include "mc_drv.h"
#include "mc_sensor.h"

#include "uart_32m1.h"


/*===========================================================================*/
/*  DEFINES                                                                  */
/*===========================================================================*/
typedef struct {
    unsigned char last_hs;
    unsigned short ts[8];
    unsigned short per[8];

    unsigned short last_ts;
} t_acpipe;

/*===========================================================================*/
/*  Modul Globals                                                            */
/*===========================================================================*/

/** \brief ac
    local data pipeline
*/
volatile t_acpipe ac;

/** \brief acpipe_configure
    data pipeline initial state
    \param none
    \return none
*/
void acpipe_configure(void) {
    unsigned char c;
    ac.last_hs = HS_010;
    for (c=0;c<=7;c++) {
        ac.ts[c] = 0;
        ac.per[c] = 0;
    }
}

/** \brief acpipe_push
    push commutation timestamp in pipeline
    \param new_hs       : virtual hallsensor
           cur_ts       : current timestamp
    \return none
*/
unsigned char acpipe_push(unsigned char new_hs,unsigned short cur_ts) {
    // param: hs = virtual hallsensor / ts = current timestamp
    
    ac.per[ac.last_hs] = cur_ts - ac.ts[ac.last_hs];
    ac.ts[ac.last_hs] = cur_ts;
    ac.last_hs = new_hs;

    ac.last_ts = cur_ts;

    return 1;
}

/** \brief mcsensor_reset
    reset data pipeline
    \param none
    \return none
*/        
void mcsensor_reset(void) {
    acpipe_configure();
}

/** \brief mcsensor_configure
    low level sensor initialization
    \param none
    \return none
*/
void mcsensor_configure(void) {
    AC0_configure();
    AC1_configure();
    AC2_configure();
        
    AC0_edgeselect(AC_TOGGLE);
    AC1_edgeselect(AC_TOGGLE);
    AC2_edgeselect(AC_TOGGLE);

    // AC interrupts enabled in PRELOCKED state to minimize interrupt load durring ramp up
    //AC0_ie(1);
    //AC1_ie(1);
    //AC2_ie(1);
}

/** \brief mcsensor_getmedian
    get median of all measurements
    \param none
    \return median value
*/
unsigned short mcsensor_getmedian(void) {
    unsigned short ret = 0;
    unsigned char c;    
    for (c=1;c<=6;c++) 
        ret += ac.per[c];
    ret += (ac.per[ac.last_hs]<<1);
    ret = ret >> 3;
    return ret;
}

/** \brief mcsensor_calc
    user defined calculation 
    \param none
    \return none
*/
void mcsensor_calc(void) {
    // additional filter calculation
    // add custom code here
}

