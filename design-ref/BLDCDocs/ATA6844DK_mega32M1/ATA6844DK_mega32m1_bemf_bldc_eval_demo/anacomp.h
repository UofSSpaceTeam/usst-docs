#ifndef __ANACOMP_H
#define __ANACOMP_H

#define AC0_configure()         (AC0CON = (1<<AC0EN)|(0<<AC0IE)|(0<<AC0IS1)|(0<<AC0IS0)|/*(0<<AC0ICE)|*/(1<<AC0M2)|(1<<AC0M1)|(0<<AC0M0))
#define AC1_configure()         (AC1CON = (1<<AC1EN)|(0<<AC1IE)|(0<<AC1IS1)|(0<<AC1IS0)|/*(0<<AC1ICE)|*/(1<<AC1M2)|(1<<AC1M1)|(0<<AC1M0))
#define AC2_configure()         (AC2CON = (1<<AC2EN)|(0<<AC2IE)|(0<<AC2IS1)|(0<<AC2IS0)|/*(0<<AC2ICE)|*/(1<<AC2M2)|(1<<AC2M1)|(0<<AC2M0))

#define AC0_ieset()             (AC0CON |= (1<<AC0IE))
#define AC1_ieset()             (AC1CON |= (1<<AC1IE))
#define AC2_ieset()             (AC2CON |= (1<<AC2IE))
#define AC0_ieclear()           (AC0CON &= ~(1<<AC0IE))
#define AC1_ieclear()           (AC1CON &= ~(1<<AC1IE))
#define AC2_ieclear()           (AC2CON &= ~(1<<AC2IE))

#define AC_TOGGLE               0x00
#define AC_FALLING              0x20
#define AC_RISING               0x30

#define AC0_ie(c)               ((c)?AC0_ieset():AC0_ieclear())
#define AC1_ie(c)               ((c)?AC1_ieset():AC1_ieclear())
#define AC2_ie(c)               ((c)?AC2_ieset():AC2_ieclear())

#define AC0_get()               ((ACSR & (1<<AC0O))?1:0)
#define AC1_get()               ((ACSR & (1<<AC1O))?1:0)
#define AC2_get()               ((ACSR & (1<<AC2O))?1:0)

#define AC_clearflags()         (ACSR|=(1<<AC2IF)|(1<<AC1IF)|(1<<AC0IF))

#define AC0_edgeselect(c)       { AC0CON &= ~c; AC0CON |= c; }
#define AC1_edgeselect(c)       { AC1CON &= ~c; AC1CON |= c; }
#define AC2_edgeselect(c)       { AC2CON &= ~c; AC2CON |= c; }

#endif /* __ANACOMP_H */

