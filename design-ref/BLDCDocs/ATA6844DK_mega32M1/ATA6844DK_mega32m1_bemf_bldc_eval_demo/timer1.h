#ifndef __TIMER1_H
#define __TIMER1_H

// Use Timer1 to measure delays between virtual hall sensor changes (presc. 1/64)
#define TIMER1_configure()      ()

#define TIMER1_ieocaset()       (TIMSK1|=(1<<OCIE1A))
#define TIMER1_ieocaclear()     (TIMSK1&=~(1<<OCIE1A))
#define TIMER1_ieoca(x)         ((x)?(TIMER1_ieocaset()):(TIMER1_ieocaclear()))

#define TIMER1_ieocbset()       (TIMSK1|=(1<<OCIE1B))
#define TIMER1_ieocbclear()     (TIMSK1&=~(1<<OCIE1B))
#define TIMER1_ieocb(x)         ((x)?(TIMER1_ieocbset()):(TIMER1_ieocbclear()))

#define TIMER1_ocbclearflag()   (TIFR1|=(1<<OCF1B))

#define TIMER1_setOCRA(x)       (OCR1A = TCNT1 + x)
#define TIMER1_setOCRB(x)       (OCR1B = TCNT1 + x)


#define TIMER1_start()          (TCCR1B|=(0<<CS12)|(1<<CS11)|(1<<CS10))
#define TIMER1_stop()           (TCCR1B&=0xF8)
#define TIMER1_reset()          (TCNT1=0)
#define TIMER1_get()            (TCNT1)

#endif /* __TIMER1_H */
