#include "drivers/vga/vga.h"
#include "include/stdio.h"
#include "gdt/gdt.h"

void kmain(void);

void kmain(void) {
    init_gdt();
    
    reset();
    printf("GDT is done\n");
}