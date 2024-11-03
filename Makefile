CC=gcc
CFLAGS=-g -m32 -fno-stack-protector -fno-builtin
ASM=nasm
OS_NAME=ZizOS
SRC_DIR=src
DRIVERS_SRC_DIR=${SRC_DIR}/drivers
STD_SRC_DIR=${SRC_DIR}/std
BUILD_DIR=build
ISO_ROOT_DIR=build/${OS_NAME}

all: always iso
# ${BUILD_DIR}/div.o ${BUILD_DIR}/stdio.o
${BUILD_DIR}/kernel: ${BUILD_DIR}/boot.o ${BUILD_DIR}/vga.o ${BUILD_DIR}/kernel.o ${BUILD_DIR}/gdts.o ${BUILD_DIR}/gdt.o
	ld -m elf_i386 -T linker.ld -o $@ $^

${BUILD_DIR}/kernel.o: ${SRC_DIR}/kernel.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/vga.o:  ${DRIVERS_SRC_DIR}/vga/vga.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/stdio.o:  ${STD_SRC_DIR}/io/stdio.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/gdt.o:  ${SRC_DIR}/gdt/gdt.c
	${CC} ${CFLAGS} -c $^ -o $@

${BUILD_DIR}/gdts.o:  ${SRC_DIR}/gdt/gdt.s
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