# lab2-gpio/Makefile
# Copyright (c) 2018, J. M. Spivey

all: game_suite.hex

CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = -O -g -Wall
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as

%.elf: %.o startup.o gio.o gio_extended.o rng.o game_list.o snake_game.o pong_game.o dodge_game.o
	$(CC) $(CPU) $(CFLAGS) -TNRF51822.ld -nostdlib \
	    $^ -lgcc -o $@ -Wl,-Map,$*.map 
	arm-none-eabi-size $@

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@ 

%.hex: %.elf
	arm-none-eabi-objcopy -O ihex $< $@

clean:
	rm -f *.hex *.elf *.map *.o 

# Don't delete intermediate files
.SECONDARY:
