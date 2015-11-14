#ifndef __ATA6844_H
#define __ATA6844_H

#define ATA6844_getdg()                 (PINC&0x0E)
#define DG3                             0x08
#define DG2                             0x04
#define DG1                             0x02

// change DDR,PORT & P to define different ncoast pin
#define ATA6844_ncoast_DDR              DDRB
#define ATA6844_ncoast_PORT             PORTB
#define ATA6844_ncoast_P                PORTB3
#define ATA6844_ncoast_configure()      (ATA6844_ncoast_DDR=(1<<ATA6844_ncoast_P))
#define ATA6844_ncoast_set()            (ATA6844_ncoast_PORT|=(1<<ATA6844_ncoast_P))
#define ATA6844_ncoast_clear()          (ATA6844_ncoast_PORT&=~(1<<ATA6844_ncoast_P))
#define ATA6844_ncoast(x)               ((x)?(ATA6844_ncoast_set()):(ATA6844_ncoast_clear()))

// change DDR,PORT & P to define different sleep pin
#define ATA6844_sleep_DDR               DDRD
#define ATA6844_sleep_PORT              PORTD
#define ATA6844_sleep_P                 PORTD1
#define ATA6844_sleep_configure()       (ATA6844_sleep_DDR |= (1<<ATA6844_sleep_P))
#define ATA6844_sleep_set()             (ATA6844_sleep_PORT |= (1<<ATA6844_sleep_P))
#define ATA6844_sleep_clear()           (ATA6844_sleep_PORT &= ~(1<<ATA6844_sleep_P))
#define ATA6844_sleep(x)                ((x)?(ATA6844_sleep_set()):(ATA6844_sleep_clear()))


void ATA6844_init_port(void);
void ATA6844_reset_DG1(void);

#endif /* __ATA6844_H */
