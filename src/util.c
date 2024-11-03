#include "include/util.h"

void memset(void* dest, char value, uint32_t size) {
    char* temp = (char*)dest;
    for (; size != 0; size--) {
        *temp++ = value;
    }
}

void out_port_b(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}