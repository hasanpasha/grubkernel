#include "drivers/vga/vga.h"
#include "include/stdio.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"
#include "timer/timer.h"

void kmain(void);

void kmain(void) {
    reset();

    init_gdt();
    printf("GDT is done\n");

    init_idt();
    printf("IDT is done\n");

    init_timer();
    printf("Timer is done\n");

    for (;;);
    // printf("%d\n", 1/0);
}