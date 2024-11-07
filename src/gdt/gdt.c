#include "gdt.h"
#include "../include/util.h"

extern void gdt_flush(uint32_t);
extern void tss_flush();

#define GDT_TABLE_SIZE 6

gdt_entry_t gdt_entries[GDT_TABLE_SIZE];
gdt_ptr_t gdt_p;
struct tss_entry_struct tss_entry;

void init_gdt() {
    gdt_p.size = (sizeof(struct gdt_entry_struct) * GDT_TABLE_SIZE) - 1;
    gdt_p.base = (uint32_t)&gdt_entries;

    union gdt_flags_union null_flags = { .flags_value = 0 };
    union gdt_access_union null_access = { .access_value = 0 };
    set_gdt_gate(0, 0, 0, null_access, null_flags); // NULL segment

    union gdt_flags_union other_flags = { .flags = { .G = 1, .DB = 1, .L = 0 } };   // 0b1100, 0xC

    union gdt_access_union kernel_code_access = { .access = { .P = 1, .DPL = 0, .S = 1, .E = 1, .DC = 0, .RW = 1, .A = 0 } }; // 0b10011010 , 0x9A
    set_gdt_gate(1, 0, 0xFFFFF, kernel_code_access, other_flags); // kernel code segment

    union gdt_access_union kernel_data_access = { .access = { .P = 1, .DPL = 0, .S = 1, .E = 0, .DC = 0, .RW = 1, .A = 0 } }; // 0b10010010, 0x92
    set_gdt_gate(2, 0, 0xFFFFF, kernel_data_access, other_flags); // kernel data segment

    union gdt_access_union user_code_access = { .access = { .P = 1, .DPL = 3, .S = 1, .E = 1, .DC = 0, .RW = 1, .A = 0 } }; // 0b11111010, 0xFA
    set_gdt_gate(3, 0, 0xFFFFF, user_code_access, other_flags); // User code segment


    union gdt_access_union user_data_access = { .access = { .P = 1, .DPL = 3, .S = 1, .E = 0, .DC = 0, .RW = 1, .A = 0 } }; // 0b11110010, 0xF2
    set_gdt_gate(4, 0, 0xFFFFF, user_data_access, other_flags); // User data segment

    write_tss(5, 0x10, 0x0); // TSS segment

    gdt_flush((uint32_t)&gdt_p);
    tss_flush();
}

void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, union gdt_access_union access, union gdt_flags_union flags) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].limit_high = (limit >> 16) & 0x0F;
    gdt_entries[num].access = access.access_value;
    gdt_entries[num].flags = flags.flags_value;
}

void write_tss(uint32_t num, uint16_t ss0, uint32_t esp0) {
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    union gdt_flags_union flags = { .flags_value = 0 };
    union gdt_access_union access = { .access = { .P = 1, .DPL = 3, .S = 0, .E = 1, .DC = 0, .RW = 0, .A = 1 } }; // 0b11101001, 0xE9
    set_gdt_gate(num, base, limit, access, flags); // TSS segment

    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = 0x08 | 0x3;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x3;
}
