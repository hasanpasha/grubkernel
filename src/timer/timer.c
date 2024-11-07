#include "../include/stdint.h"
#include "../interrupts/idt.h"
#include "../include/stdio.h"
#include "../include/util.h"
#include "../include/stdmath.h"
#include "timer.h"

#define TIMER_CALLBACKS_SIZE 256

timer_callbacks_entry_t cbs[TIMER_CALLBACKS_SIZE];
uint8_t cbs_len;

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

    cbs_len = 1;
}

void on_irq0(struct interrupt_registers_struct* _) {
    ticks++;

    // printf("Timer Ticked!, %d\n", ticks);

    for (int i = 0; i < cbs_len; i++) {
        timer_callbacks_entry_t entry = cbs[i];
        if (!entry.active || ticks < entry.every) continue;
        uint32_t rem;
        div64(ticks, entry.every, &rem);
        if (rem == 0) {
            timer_callback_t cb = entry.callback;
            if (cb(ticks, entry.userdata)) {
                cbs[i].active = false;
            }
        }
    }
}

uint64_t timer_get_ticks() {
    return ticks;
}

uint8_t timer_install_callback(uint32_t every, timer_callback_t cb, void* userdata) {
    if (cbs_len >= TIMER_CALLBACKS_SIZE) {
        return 0;
    }

    cbs[cbs_len].active = true;
    cbs[cbs_len].every = every;
    cbs[cbs_len].callback = cb;
    cbs[cbs_len].userdata = userdata;

    return cbs_len++;
}

void timer_uninstall_callback(uint8_t index) {
    cbs[index].active = false;
}