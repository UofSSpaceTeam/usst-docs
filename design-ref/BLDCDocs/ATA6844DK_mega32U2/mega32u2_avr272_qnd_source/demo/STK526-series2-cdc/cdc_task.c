/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file manages the CDC task.
//!
//! - Compiler:           IAR EWAVR and GNU GCC for AVR
//! - Supported devices:  AT90USB162, AT90USB82
//!
//! \author               Atmel Corporation: http://www.atmel.com \n
//!                       Support and FAQ: http://support.atmel.no/
//!
//! ***************************************************************************

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf_usb.h"
#include "cdc_task.h"
#include "lib_mcu/usb/usb_drv.h"
#include "usb_descriptors.h"
#include "modules/usb/device_chap9/usb_standard_request.h"
#include "usb_specific_request.h"
#include "lib_mcu/uart/uart_lib.h"
#include "uart_usb_lib.h"
#include "lib_mcu/util/start_boot.h"
#include <stdio.h>


//_____ M A C R O S ________________________________________________________



//_____ D E F I N I T I O N S ______________________________________________



//_____ D E C L A R A T I O N S ____________________________________________


volatile U8  cpt_sof;
extern U8    rx_counter;
extern U8    tx_counter;
extern volatile U8 usb_request_break_generation;
S_line_coding line_coding;
S_line_status line_status;      // for detection of serial state input lines
S_serial_state serial_state;   // for serial state output lines

volatile U8 rs2usb[10];


//! @brief This function initializes the hardware ressources required for CDC demo.
//!
//!
//! @param none
//!
//! @return none
//!
//!/
void cdc_task_init(void)
{
   uart_init();
   Uart_enable_it_rx();
   Leds_init();
   Joy_init();
   Hwb_button_init();
   Usb_enable_sof_interrupt();
#ifdef __GNUC__
    fdevopen((int (*)(char, FILE*))(uart_usb_putchar),(int (*)(FILE*))uart_usb_getchar); //for printf redirection 
#endif
}



//! @brief Entry point of the uart cdc management
//!
//! This function links the uart and the USB bus.
//!
//! @param none
//!
//! @return none
void cdc_task(void)
{
      
   if(Is_device_enumerated() && line_status.DTR) //Enumeration processs OK and COM port openned ?
        {
      if (Uart_tx_ready())    //USART free ?
      {
         if (uart_usb_test_hit())   // Something received from the USB ?
         {
             while (rx_counter)
            {

				// quick n dirty led toggle
   				PIND |= (1<<PD4);

               uart_putchar(uart_usb_getchar());   // loop back USB to USART
               Led3_toggle();
            }
         }
      }

      /*if ( cpt_sof>=REPEAT_KEY_PRESSED)   //Debounce joystick events
      {
         if (Is_btn_middle()){
            printf ("Select Pressed !\r\n");
         }
         if (Is_joy_right()) {
            printf ("Right Pressed !\r\n");
            serial_state.bDCD = TRUE;
         }
         else
            serial_state.bDCD = FALSE;

         if (Is_joy_left()) {
            printf ("Left Pressed !\r\n");
            serial_state.bDSR = TRUE;
         }
         else
            serial_state.bDSR = FALSE;

         if (Is_joy_down())
         printf ("Down Pressed !\r\n");

         if (Is_joy_up())
         printf ("Up Pressed !\r\n");

         if(Is_btn_left())
         printf("Hello from AT90USBXXX !\r\n");
         
         cdc_update_serial_state();
      }
*/
      if(usb_request_break_generation==TRUE)
      {
         usb_request_break_generation=FALSE;
         Led2_toggle();
      }
   }
}

//! @brief sof_action
//!
//! This function increments the cpt_sof counter each times
//! the USB Start Of Frame interrupt subroutine is executed (1ms)
//! Usefull to manage time delays
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
void sof_action()
{
   cpt_sof++;
}


//! @brief Uart Receive interrupt subroutine
//!
//! @param none
//!
//! @return none
#ifdef __GNUC__
 ISR(USART1_RX_vect)
#else
#pragma vector = USART1_RX_vect
__interrupt void usart_receive_interrupt()
#endif
{
   U8 i=0;
   U8 save_ep;
   
   // quick n dirty led toggle
   PIND |= (1<<PD4);

   if(Is_device_enumerated()) 
   {
      save_ep=Usb_get_selected_endpoint();   
      Usb_select_endpoint(TX_EP);
      do 
      {
         if(Uart_rx_ready())
         {
            rs2usb[i]=Uart_get_byte();
            i++;
         }
      }while(Is_usb_write_enabled()==FALSE );
      uart_usb_send_buffer((U8*)&rs2usb,i);
      Usb_select_endpoint(save_ep);
   }
}
