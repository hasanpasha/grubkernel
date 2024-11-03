#include "gdt.h"

extern void gdt_flush(uint32_t);

#define GDT_TABLE_SIZE 5

struct gdt_entry_struct gdt_entries[GDT_TABLE_SIZE];
struct gdt_ptr_struct gdt_ptr;

void init_gdt() {
    gdt_ptr.size = (sizeof(struct gdt_entry_struct) * GDT_TABLE_SIZE) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    set_gdt_gate(0, 0, 0, 0, 0); // NULL segment
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // kernel code segment
    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // kernel data segment
    set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment
    set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment

    gdt_flush((uint32_t)&gdt_ptr);
}

void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].limit_high = (limit >> 16) & 0x0F;
    gdt_entries[num].flags = (gran & 0x0F);
    gdt_entries[num].access = access;
}
