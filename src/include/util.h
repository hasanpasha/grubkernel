#include "stdint.h"

struct interrupt_registers_struct {
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));

void memset(void* dest, char value, uint32_t size);
void out_port_b(uint16_t port, uint8_t value);