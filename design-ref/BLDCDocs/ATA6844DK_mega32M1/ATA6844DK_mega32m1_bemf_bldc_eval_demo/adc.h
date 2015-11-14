#ifndef __ADC_H
#define __ADC_H

#define ADC_start()             (ADCSRA|=(1<<ADSC))
#define	ADC_done()              ((ADCSRA&(1<<ADSC))?0:1)

#define ADC_clearflag()         (ADCSRA|=(1<<ADIF))

#define ADC_get()               (ADCH)


void ADC_configure(void);
void ADC_setdelta(unsigned short d);
void ADC_getfiltered(unsigned char *s);
unsigned char ADC_ISR(void);

// this was intended as private function but needs to be used in commutation handling
// to be renamed in next sw revision
void t_adc_state_reset(void);

#endif /* __ADC_H */
