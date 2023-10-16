CC=gcc
CFLAGS=-Wall -pedantic -Werror

build: boot.bin encode.exe

%.exe:%.c
	@echo "Building $@..."
	@$(CC) $(CFLAGS) -o $@ $<

boot.bin: boot.asm
	@echo "Building $@..."
	@nasm -fbin $< -o $@

run:
	@qemu-system-i386 -fda boot.bin -display curses

clean:
	@echo "Cleaning"
	@rm -fv *.exe *.bin