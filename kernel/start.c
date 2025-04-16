#include "param.h"

__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

extern void uart_init();
extern int uart_puts(char *s);

void start()
{
    uart_init();
	uart_puts("Hello\n");

    while(1){};
}
