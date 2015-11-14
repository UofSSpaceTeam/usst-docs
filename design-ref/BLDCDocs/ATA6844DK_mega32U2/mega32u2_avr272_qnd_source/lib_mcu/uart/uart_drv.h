/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief Provide Uart driver
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

#ifndef _UART_DRV_H_
#define _UART_DRV_H_

#define MSK_UART_5BIT           0x00 /* UCSRnC register */
#define MSK_UART_6BIT           0x02
#define MSK_UART_7BIT           0x04
#define MSK_UART_8BIT           0x06
#define MSK_UART_9BIT           0x06

#define MSK_UART_RX_DONE        0x80 /* UCSRnA register */
#define MSK_UART_TX_COMPLET     0x40
#define MSK_UART_DRE            0x20

#define MSK_UART_ENABLE_IT_RX   0x80
#define MSK_UART_ENABLE_IT_TX   0x40
#define MSK_UART_ENABLE_RX      0x10 /* UCSRnB register */
#define MSK_UART_ENABLE_TX      0x08
#define MSK_UART_TX_BIT9        0x01
#define MSK_UART_RX_BIT9        0x02

#ifdef USE_UART1
    #define UCSRC   (UCSR0C)
    #define UCSRB   (UCSR0B)
    #define UCSRA   (UCSR0A)
    #define UDR     (UDR0)
    #define UBRRL   (UBRR0L)
    #define UBRRH   (UBRR0H)
    #define UBRR    (UBRR0)
#endif
#ifdef USE_UART2
    #define UCSRC    (UCSR1C)
    #define UCSRB    (UCSR1B)
    #define UCSRA    (UCSR1A)
    #define UDR      (UDR1)
    #define UBRRL    (UBRR1L)
    #define UBRRH    (UBRR1H)
    #define UBRR     (UBRR1)

#endif


#define Uart_hw_init(config)    (UCSRC=config)
#define Uart_enable()           (UCSRB|=MSK_UART_ENABLE_RX|MSK_UART_ENABLE_TX)
#define Uart_tx_ready()         (UCSRA&MSK_UART_DRE)
#define Uart_set_tx_busy()
#define Uart_send_byte(ch)      (UDR=ch)
#define Uart_rx_ready()         (UCSRA&MSK_UART_RX_DONE)
#define Uart_get_byte()         (UDR)
#define Uart_ack_rx_byte()

#define Uart_enable_it_rx()    (UCSRB|=MSK_UART_ENABLE_IT_RX)
#define Uart_enable_it_tx()    (UCSRB|=MSK_UART_ENABLE_IT_TX)
#define Uart_disable_it_rx()   (UCSRB&=~MSK_UART_ENABLE_IT_RX)

#endif
