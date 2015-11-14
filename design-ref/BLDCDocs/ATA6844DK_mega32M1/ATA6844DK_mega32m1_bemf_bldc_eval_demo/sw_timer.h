#ifndef __SW_TIMER_H
#define __SW_TIMER_H

typedef struct {
    unsigned short sw_timer1;
    unsigned short sw_timer2;
} t_swtimer;

//#define SWT     SWT

void swt_configure(t_swtimer *swt);

#define SWT1_setdelayms(x)      (SWT.sw_timer1=((unsigned short)(x)<<2))
#define SWT1_setdelayticks(x)   (SWT.sw_timer1=((unsigned short)(x)))
#define SWT1_timeout()          ((SWT.sw_timer1)?(0):(1))

#define SWT2_setdelayms(x)      (SWT.sw_timer2=((unsigned short)(x)<<2))
#define SWT2_setdelayticks(x)   (SWT.sw_timer2=((unsigned short)(x)))
#define SWT2_timeout()          ((SWT.sw_timer2)?(0):(1))

#endif /* __SW_TIMER */
