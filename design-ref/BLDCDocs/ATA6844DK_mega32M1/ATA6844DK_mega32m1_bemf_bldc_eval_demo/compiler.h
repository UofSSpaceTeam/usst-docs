#ifndef __COMPILER_H
#define __COMPILER_H

// include
#if __GNUC__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#elif __ICCAVR__
#include <ioavr.h>
#include <inavr.h>
#else
#error Unknown compiler
#endif  // compiler selection

// compatibility layer
#if __GNUC__


#elif __ICCAVR__

#define prog_char       __flash char
#define PSTR         
#define pgm_read_byte   *

#define sei()           __enable_interrupt()
#define cli()           __disable_interrupt()

#define ISR_PRAGMA(param) _Pragma(#param)

#define ISR(vectorname)\
    ISR_PRAGMA(vector = vectorname)\
    __interrupt void vectorname##_handler(void)

// compatibility for analog comparator interrupts
#ifndef ANACOMP0_vect
#define ANACOMP0_vect   ANA_COMP0_vect
#endif // ANACOMP0_vect

#ifndef ANACOMP1_vect
#define ANACOMP1_vect   ANA_COMP1_vect
#endif // ANACOMP1_vect

#ifndef ANACOMP2_vect
#define ANACOMP2_vect   ANA_COMP2_vect
#endif // ANACOMP2_vect

#endif  // compiler selection

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
typedef unsigned char Bool;
typedef char S8;
typedef int S16;
typedef long S32;
typedef long long S64;


#endif // __COMPILER_H
