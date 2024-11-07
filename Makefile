CC=./crosscompiler/usr/opt/cross/bin/i686-elf-gcc
LD=./crosscompiler/usr/opt/cross/bin/i686-elf-ld
CFLAGS= -ffreestanding -Wall -Wextra -lm -g -O2
ASM=nasm
OS_NAME=ZizOS
SRC_DIR=src
DRIVERS_SRC_DIR=${SRC_DIR}/drivers
STD_SRC_DIR=${SRC_DIR}/std
BUILD_DIR=build
ISO_ROOT_DIR=build/${OS_NAME}

all: always iso
${BUILD_DIR}/kernel: ${BUILD_DIR}/boot.o ${BUILD_DIR}/vga.o ${BUILD_DIR}/kernel.o ${BUILD_DIR}/gdts.o ${BUILD_DIR}/gdt.o ${BUILD_DIR}/div.o ${BUILD_DIR}/stdio.o ${BUILD_DIR}/util.o ${BUILD_DIR}/idt.o ${BUILD_DIR}/idts.o ${BUILD_DIR}/timer.o ${BUILD_DIR}/keyboard.o ${BUILD_DIR}/mem.o ${BUILD_DIR}/math.o
	${LD} -m elf_i386 -T linker.ld -o $@ $^

${BUILD_DIR}/kernel.o: ${SRC_DIR}/kernel.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/vga.o:  ${DRIVERS_SRC_DIR}/vga/vga.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/stdio.o:  ${STD_SRC_DIR}/io/stdio.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/math.o:  ${STD_SRC_DIR}/math/math.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/idt.o:  ${SRC_DIR}/interrupts/idt.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/gdt.o:  ${SRC_DIR}/gdt/gdt.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/timer.o:  ${SRC_DIR}/timer/timer.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/keyboard.o:  ${SRC_DIR}/keyboard/keyboard.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/mem.o:  ${SRC_DIR}/mem/mem.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/util.o:  ${SRC_DIR}/util.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/gdts.o:  ${SRC_DIR}/gdt/gdt.s
	${ASM} -f elf32 $^ -o $@

${BUILD_DIR}/idts.o:  ${SRC_DIR}/interrupts/idt.s
	${ASM} -f elf32 $^ -o $@

${BUILD_DIR}/boot.o: ${SRC_DIR}/boot.s
	${ASM} -f elf32 $^ -o $@

${BUILD_DIR}/div.o: ${STD_SRC_DIR}/math/div.s
	${ASM} -f elf32 $^ -o $@

always: ${SRC_DIR}/grub.cfg
	mkdir -p ${ISO_ROOT_DIR}/boot/grub
	cp ${SRC_DIR}/grub.cfg ${ISO_ROOT_DIR}/boot/grub/grub.cfg

iso: ${BUILD_DIR}/kernel
	cp ${BUILD_DIR}/kernel ${ISO_ROOT_DIR}/boot/kernel
	grub-mkrescue -o ${BUILD_DIR}/${OS_NAME}.iso ${ISO_ROOT_DIR}

.PHONY = clean
clean:
	rm -rf ${BUILD_DIR}