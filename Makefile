CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
UF2CONV=~/src/uf2/utils/uf2conv.py

CFLAGS=-mthumb -mcpu=cortex-m4

.PHONY: all
all: blink_led.bin

.PHONY: clean
clean:
	rm -f blink_led.o blink_led.elf blink_led.bin vectors.o

blink_led.o: blink_led.c
	$(CC) $(CFLAGS) -c blink_led.c -o blink_led.o

vectors.o: vectors.c
	$(CC) $(CFLAGS) -c vectors.c -o vectors.o

blink_led.elf: blink_led.o vectors.o bootloader.ld
	$(LD) -T bootloader.ld blink_led.o vectors.o -o blink_led.elf

blink_led.bin: blink_led.elf
	$(OBJCOPY) -O binary blink_led.elf blink_led.bin

.PHONY: flash
flash: blink_led.bin
	$(UF2CONV) -b 0x4000 blink_led.bin
