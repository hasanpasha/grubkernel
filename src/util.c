#include "include/util.h"
#include "include/stdio.h"

void memset(void* dest, char value, uint32_t size) {
    char* temp = (char*)dest;
    for (; size != 0; size--) {
        *temp++ = value;
    }
}

void out_port_b(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

char in_port_b(uint16_t port) {
    char rv;
    asm volatile("inb %1, %0": "=a" (rv) : "dN" (port));
    return rv;
}

void print_gdt(gdt_entry_t gdt_entry) {
    uint32_t base = (((uint32_t)gdt_entry.base_high << 24) | ((uint32_t)gdt_entry.base_middle << 16)) | gdt_entry.base_low;
    uint32_t limit = ((uint32_t)gdt_entry.limit_high << 16) | gdt_entry.limit_low;
    union gdt_flags_union flags = { .flags_value = gdt_entry.flags };
    // union gdt_flags_union flags = gdt_entry.flags;
    union gdt_access_union access = { .access_value = gdt_entry.access };
    printf("gdt: base: %x, limit: %x, ", base, limit);
    printf("flags: G: %b, DB: %b, L: %b, ", flags.flags.G, flags.flags.DB, flags.flags.L);
    printf("access: P: %b, DPL: %d, S: %b, E: %b, DC: %b, RW: %b, A: %b", access.access.P, access.access.DPL, 
    access.access.S, access.access.E, access.access.DC, access.access.RW, access.access.A);
    puts("\n");
}

void print_idt(idt_entry_t entry) {
    uint32_t offset = ((uint32_t)entry.offset_high << 16) | entry.offset_low;
    union idt_flags_union flags = { .flags_value = entry.flags };
    union idt_segment_selector_union sel = { .selector_value = entry.selector };
    printf("idt: offset: %x, ", offset);
    printf("flags: gate_type: %X, DPL: %d, P: %b, ", flags.flags.gate_type, flags.flags.DPL, flags.flags.P);
    printf("selector: index: %d, TI: %b, RPL: %d", sel.selector.index, sel.selector.TI, sel.selector.RPL);
    putc('\n');
}