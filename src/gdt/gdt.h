#include "../include/stdint.h"


struct gdt_entry_flags_struct {
    uint8_t pad : 1;
    uint8_t L : 1;  // Long-mode code flag
    uint8_t DB : 1; // Size flag
    uint8_t G : 1;  // Granularity flag
} __attribute__((packed));
typedef struct gdt_entry_flags_struct gdt_entry_flags;

union gdt_flags_union
{
    uint8_t flags_value : 4;
    struct gdt_entry_flags_struct flags;
} __attribute__((packed));

struct gdt_entry_access_struct {
    uint8_t A: 1;   // Accessed bit
    uint8_t RW: 1;  // Readable bit/Writable bit
    uint8_t DC: 1;  // Direction bit/Conforming bit
    uint8_t E: 1;   // Executable bit
    uint8_t S: 1;   // Descriptor type bit
    uint8_t DPL: 2; // Descriptor privilege level field
    uint8_t P: 1;   // Present bit
} __attribute__((packed));
typedef struct gdt_entry_access_struct gdt_entry_access;

union gdt_access_union
{
    uint8_t access_value;
    struct gdt_entry_access_struct access;
} __attribute__((packed));

struct gdt_entry_struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t limit_high : 4;
    uint8_t flags : 4;
    // gdt_flags flags;
    uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry;

struct gdt_ptr_struct {
    uint16_t size;
    unsigned int base;
}__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr;

void init_gdt();
void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, union gdt_access_union access, union gdt_flags_union flags);