#include "include/stdio.h"
#include "include/stdint.h"
#include "drivers/vga/vga.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"
#include "timer/timer.h"
#include "keyboard/keyboard.h"
#include "mem/mem.h"
#include "kernel.h"

void kmain(uint32_t magic, multiboot_info_t* boot_info) {
    reset();

    init_gdt();
    printf("GDT is done\n");

    init_idt();
    printf("IDT is done\n");

    // init_timer();
    // printf("Timer is done\n");

    init_keyboard();
    printf("keyboard is initialized!\n");

    init_mem(boot_info);


    for (;;);
    // printf("%d\n", 1/0);
}