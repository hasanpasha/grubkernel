#include "drivers/vga/vga.h"
#include "include/stdio.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"

void kmain(void);

void kmain(void) {
    reset();

    init_gdt();
    printf("GDT is done\n");

    init_idt();
    printf("IDT is done\n");

    // printf(1/0);
}