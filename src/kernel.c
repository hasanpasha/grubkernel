#include "include/stdio.h"
#include "include/stdint.h"
#include "drivers/vga/vga.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"
#include "timer/timer.h"
#include "keyboard/keyboard.h"
#include "mem/mem.h"
#include "kernel.h"


bool on_time(uint64_t ticks, void* data) {
    uint8_t* stop_after = (uint8_t*)data;
    printf("interval passed, ticks: %d\n", ticks);

    *stop_after -= 1;
    if (*stop_after == 0) {
        return true;
    }

    return false;
}

void kmain(uint32_t magic, multiboot_info_t* boot_info) {
    reset();

    init_gdt();
    printf("GDT is done\n");

    init_idt();
    printf("IDT is done\n");

    init_timer();
    printf("Timer is done\n");

    uint8_t stop_after = 10;
    uint8_t idx = timer_install_callback(3, on_time, (void*)&stop_after);
    if (idx == 0) {
        printf("error: failed to install a timer callback, [no more space]\n");
    } else {
        printf("installed a timer callback: index: %d\n", idx);
        // timer_uninstall_callback(idx);
    }

    init_keyboard();
    printf("keyboard is initialized!\n");

    init_mem(boot_info);


    while(1) { 
        // printf("timer: %d\n", timer_get_ticks()); 
    }
    // printf("%d\n", 1/0);
}