CROSS-COMPILER = arm-none-eabi-
QEMU = ./gnu-mcu-eclipse-qemu/bin/qemu-system-gnuarmeclipse
all: symbols.bin

symbols.bin: symbols.c isr_vectors.s
	$(CROSS-COMPILER)gcc -Wall -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld symbols.c isr_vectors.s -o symbols.elf
	$(CROSS-COMPILER)objcopy -O binary symbols.elf symbols.bin

qemu:
	@echo
	@echo "Press Ctrl+A and then press X to exit QEMU"
	@echo
	$(QEMU) -M STM32F4-Discovery -nographic -gdb tcp::1234 -S -kernel symbols.bin

flash:
	st-flash --reset write symbols.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
