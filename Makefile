k=kernel

CROSS = riscv64-unknown-elf-
CC = $(CROSS)gcc
LD = $(CROSS)ld

OBJCOPY = $(CROSS)objcopy

CFLAGS = -march=rv64g -mabi=lp64 -Wall -O2 -nostdlib -mcmodel=medany

all: $k/kernel.bin

SRC_S = $k/entry.S

SRC_C = $k/uart.c	\
 		$k/start.c	\


OBJ =  $(SRC_S:.S=.o) $(SRC_C:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

$k/kernel.elf: $(OBJ)
	$(CC) $(CFLAGS) -T $k/kernel.ld -o $@ $^


$k/kernel.bin: $k/kernel.elf
	$(OBJCOPY) -O binary $< $@

run: $k/kernel.bin
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@qemu-system-riscv64 -machine virt -nographic -bios none -kernel $k/kernel.bin

clean:
	rm -f kernel/*.o kernel/*.elf kernel/*.bin
