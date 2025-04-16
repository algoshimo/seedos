#include "types.h"
#include "memlayout.h"

/*
 * The UART control registers are memory-mapped at address UART0. 
 * This macro returns the address of one of the registers.
 */
#define UART_REG(reg) ((volatile uint8 *)(UART0 + reg))

#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

#define LSR_RX_READY (1 << 0)
#define LSR_TX_IDLE  (1 << 5)

#define uart_read_reg(reg) (*(UART_REG(reg)))
#define uart_write_reg(reg, v) (*(UART_REG(reg)) = (v))

void uart_init()
{
	/* disable interrupts. */
	uart_write_reg(IER, 0x00);
	uint8 lcr = uart_read_reg(LCR);
	uart_write_reg(LCR, lcr | (1 << 7));
	uart_write_reg(DLL, 0x03);
	uart_write_reg(DLM, 0x00);
	lcr = 0;
	uart_write_reg(LCR, lcr | (3 << 0));
}
#define UART0_THR (UART0 + 0x00)
#define UART0_LSR (UART0 + 0x05)
#define UART0_LSR_EMPTY_MASK 0x20
#define UART0 0x10000000L
int uart_putc(char ch)
{
	// while ((uart_read_reg(LSR) & LSR_TX_IDLE) == 0);
	// return uart_write_reg(THR, ch);
	while (!(*(volatile uint8 *)(UART0_LSR) & UART0_LSR_EMPTY_MASK));
    *(volatile uint8 *)(UART0_THR) = ch;
	return 0;
}

void uart_puts(char *s)
{
	while (*s) {
		uart_putc(*s++);
	}
}


// OUTPUT_ARCH( "riscv" )
// ENTRY( _entry )

// SECTIONS
// {
//   /*
//    * ensure that entry.S / _entry is at 0x80000000,
//    * where qemu's -kernel jumps.
//    */
//   . = 0x80000000;

//   .text : {
//     *(.text .text.*)
//     . = ALIGN(0x1000);
//     PROVIDE(etext = .);
//   }

//   .rodata : {
//     . = ALIGN(16);
//     *(.srodata .srodata.*) /* do not need to distinguish this from .rodata */
//     . = ALIGN(16);
//     *(.rodata .rodata.*)
//   }

//   .data : {
//     . = ALIGN(16);
//     *(.sdata .sdata.*) /* do not need to distinguish this from .data */
//     . = ALIGN(16);
//     *(.data .data.*)
//   }

//   .bss : {
//     . = ALIGN(16);
//     *(.sbss .sbss.*) /* do not need to distinguish this from .bss */
//     . = ALIGN(16);
//     *(.bss .bss.*)
//   }

//   PROVIDE(end = .);
// }