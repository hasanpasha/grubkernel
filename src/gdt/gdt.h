#include "../include/stdint.h"


struct gdt_entry_flags_struct {
    uint8_t pad : 1;
    uint8_t L : 1;
    uint8_t DB : 1;
    uint8_t G : 1;
} __attribute__((packed));
typedef struct gdt_entry_flags_struct gdt_entry_flags;

struct gdt_entry_struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t limit_high : 4;
    struct gdt_entry_flags_struct flags;
    uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry;

struct gdt_ptr_struct {
    uint16_t size;
    unsigned int base;
}__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr;

void init_gdt();
void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, gdt_entry_flags flags);