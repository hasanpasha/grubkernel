#include "../include/common.h"
#include "../include/stdbool.h"

typedef bool (*timer_callback_t)(uint64_t, void*);

typedef struct
{
    bool active;
    uint32_t every;
    timer_callback_t callback;
    void* userdata;
} timer_callbacks_entry_t;



void init_timer();
void on_irq0(struct interrupt_registers_struct* regs);

uint64_t timer_get_ticks();

uint8_t timer_install_callback(uint32_t every, timer_callback_t cb, void* userdata);
void timer_uninstall_callback(uint8_t index);