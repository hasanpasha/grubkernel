#include "../include/stdint.h"
#include "../interrupts/idt.h"
#include "../include/stdio.h"
#include "timer.h"

uint64_t ticks;
const uint32_t freq = 100;

void init_timer() {
    ticks = 0;
    irq_install_handler(0, &on_irq0);

    // 1.19318 MHz
    uint32_t divisor = 1193180/freq;

    // 0b00110110
    out_port_b(0x43, 0x36);
    out_port_b(0x40, (uint8_t)(divisor & 0xFF));
    out_port_b(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

void on_irq0(struct interrupt_registers_struct* _) {
    ticks++;

    printf("Timer Ticked!, %d\n", ticks);
}