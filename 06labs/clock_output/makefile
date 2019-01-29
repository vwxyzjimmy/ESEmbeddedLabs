CROSS-COMPILER = arm-none-eabi-

all: clock.bin

clock.bin: clock.c blink.c startup.c vector_table.s
	$(CROSS-COMPILER)gcc -Wall -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld clock.c blink.c startup.c vector_table.s -o clock.elf
	$(CROSS-COMPILER)objcopy -O binary clock.elf clock.bin

flash:
	st-flash --reset write clock.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
