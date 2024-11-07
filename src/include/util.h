#pragma once

#include "stdint.h"
#include "../gdt/gdt.h"
#include "../interrupts/idt.h"


void memset(void* dest, char value, uint32_t size);
void out_port_b(uint16_t port, uint8_t value);
char in_port_b(uint16_t port);

void print_gdt(gdt_entry_t gdt_entry);
void print_idt(idt_entry_t entry);