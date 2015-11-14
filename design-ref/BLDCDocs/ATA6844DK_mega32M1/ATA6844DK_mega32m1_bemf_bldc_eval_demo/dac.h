#ifndef __DAC_H
#define __DAC_H

#define DAC_configure()         (DACON |= (1<<DALA)|(1<<DAEN))
#define DAC_outputenable()      (DACON |= (1<<DAOE))
#define DAC_outputdisable()     (DACON &=~(1<<DAOE))
#define DAC_output(m)           ((m)?(DAC_outputenable()):(DAC_outputdisable()))

#define DAC_set(m)              (DACH = (m))

#endif // __DAC_H
