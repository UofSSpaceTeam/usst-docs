#ifndef __UART_32M1_H
#define __UART_32M1_H

void uart_init(void);

void uart_task(void);

void uart_putc(unsigned char ch);
unsigned char uart_received(void);
unsigned char uart_getc(void);

void uart_puts(char *str);
#ifdef __GNUC__
void uart_puts_P(prog_char *str);
#elif __ICCAVR__
#define uart_puts_P(x)      uart_puts(x)
#endif // compiler layer
void uart_putx(unsigned char c);
void uart_puti(unsigned int i);

#endif /* __UART_32M1_H */
