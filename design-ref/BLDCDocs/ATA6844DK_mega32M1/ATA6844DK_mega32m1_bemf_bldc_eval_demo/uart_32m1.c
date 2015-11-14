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
  Purpose: uart communication driver for mega32m1
            - added fifo buffer
            - added uart task to avoid blocking scheduler execution
            - added configurable settings
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


/*===========================================================================*/
/*  DEFINES                                                                  */
/*===========================================================================*/
#define BITSAMPLING 8    /* 8 samples per bit */

#define MYBRR (short)(((((long)F_CPU)/((long)BAUDRATE*BITSAMPLING/2))+1)/2)-1

#define FIFO_LEN     128

typedef struct {
    unsigned char pos1;
    unsigned char pos2;
    unsigned char data[FIFO_LEN];
} t_fifo;

/*===========================================================================*/
/*  Modul Globals                                                            */
/*===========================================================================*/
/** \brief uart
    fifo buffer for uart task
*/
t_fifo uart;

/*===========================================================================*/
/*  IMPLEMENTATION                                                           */
/*===========================================================================*/

void fifo_configure(t_fifo *u) {
    u->pos1=0;
    u->pos2=0;
}

int fifo_size(t_fifo *u) {
    return (u->pos2-u->pos1)&(FIFO_LEN-1);
}

void fifo_push(t_fifo *u,unsigned char c) {
    u->data[u->pos2] = c;
    u->pos2 = u->pos2 + 1;
    u->pos2 %= FIFO_LEN;
}

unsigned char fifo_pop(t_fifo *u) {    
    unsigned char ret = u->data[u->pos1];
    u->pos1 = u->pos1 + 1;
    u->pos1 %= FIFO_LEN;
    return ret;
}

void uart_putc(unsigned char ch);

void uart_task(void) {
    if (fifo_size(&uart)) {
        //uart_putc(fifo_pop(&uart));
        if ((LINSIR&(1<<LTXOK))) {
            LINDAT = fifo_pop(&uart);
        }
    }
}

void uart_init(void) {
    unsigned char tmp;
    tmp = LINCR;
    LINCR  &= ~(1<<LENA);
    LINBTR  = ((1<<LDISR) | (0x3F & BITSAMPLING));
    //LINBRR = MYBRR;
    LINBRRH = 0x00;
    LINBRRL = 0xCF;
    LINCR = tmp; 
    
    
    //Byte_transfer_enable();
    LINCR = (0<<LIN13)|(1<<LENA)|(1<<LCMD2)|(1<<LCMD1)|(1<<LCMD0);
    LINDAT = 0xFF;
    
    // fifo configure
    fifo_configure(&uart);
}

void uart_putc(unsigned char ch) {
    fifo_push(&uart,ch);
    //while (!(LINSIR & (1<<LTXOK)));
    //LINDAT = ch;
}

unsigned char uart_received(void) {
    return (LINSIR & (1<<LRXOK));
}

unsigned char uart_getc(void) {
    unsigned char ret;
    while (!uart_received());
    ret = LINDAT;
    return ret;
}


void uart_puts(char *str) {
    while (*str) {
        uart_putc(*str);
        str++;
    }
}

#ifdef __GNUC__
void uart_puts_P(prog_char *str) {
    unsigned char c;
    while (1) {
        c = pgm_read_byte(str);
        if (c == '\0') 
            break;
        uart_putc(c);
        str++;
    }
}
#endif // __GNUC__

void uart_putx(unsigned char c) {
    unsigned char hex[] = "0123456789ABCDEF";
    
    uart_putc(hex[(c>>4)&0x0F]);
    uart_putc(hex[(c&0x0F)]);
}

void uart_puti(unsigned int i) {
    char str[6]="\0\0\0\0\0\0";
    int o=0;

    if (i == 0) {
        uart_putc('0');
        return;
    }

    while (i != 0) {
        str[o] = (i%10);
        o++;
        i/=10;
    }

    while (o > 0) {
        uart_putc(str[o-1]+'0');
        o--;
    }
}
