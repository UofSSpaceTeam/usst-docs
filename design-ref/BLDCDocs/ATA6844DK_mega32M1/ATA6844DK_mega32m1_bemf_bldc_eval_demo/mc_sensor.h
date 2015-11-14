#ifndef __MC_SENSOR_H
#define __MC_SENSOR_H

void mcsensor_debug(void);

void mcsensor_reset(void);
void mcsensor_configure(void);

void mcsensor_calc_offset(unsigned short delay);
void mcsensor_calc(void);

unsigned short mcsensor_read(void);
unsigned short mcsensor_getmedian(void);

unsigned char acpipe_push(unsigned char new_hs,unsigned short cur_ts);

#define GET_HS()        ((ACSR)&0x07)

#endif // __MC_SENSOR_H

