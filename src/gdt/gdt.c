#include "gdt.h"

extern void gdt_flush(uint32_t);

#define GDT_TABLE_SIZE 5

gdt_entry gdt_entries[GDT_TABLE_SIZE];
gdt_ptr gdt_p;

void init_gdt() {
    gdt_p.size = (sizeof(struct gdt_entry_struct) * GDT_TABLE_SIZE) - 1;
    gdt_p.base = (uint32_t)&gdt_entries;

    struct gdt_entry_flags_struct null_flags = { .G = 0, .DB = 0, .L = 0 };
    struct gdt_entry_flags_struct other_flags = { .G = 1, .DB = 1, .L = 0 };

    set_gdt_gate(0, 0, 0, 0, null_flags); // NULL segment
    set_gdt_gate(1, 0, 0xFFFFF, 0x9A, other_flags); // kernel code segment
    set_gdt_gate(2, 0, 0xFFFFF, 0x92, other_flags); // kernel data segment
    set_gdt_gate(3, 0, 0xFFFFF, 0xFA, other_flags); // User code segment
    set_gdt_gate(4, 0, 0xFFFFF, 0xF2, other_flags); // User data segment

    gdt_flush((uint32_t)&gdt_p);
}

void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, gdt_entry_flags flags) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].limit_high = (limit >> 16) & 0x0F;
    gdt_entries[num].access = access;
    gdt_entries[num].flags = flags;
}
