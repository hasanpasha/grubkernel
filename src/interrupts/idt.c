#include "../include/stdint.h"
#include "../include/stdbool.h"
#include "../include/stdio.h"
#include "idt.h"

extern void idt_flush(uint32_t);

#define IDT_SIZE 256

struct idt_entry_struct idt_entries[IDT_SIZE];
struct idt_ptr_struct idt_p;

void init_idt() {
    idt_p.limit = (sizeof(struct idt_entry_struct) * IDT_SIZE) - 1;
    idt_p.base = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * IDT_SIZE);

    // PICs
    // 0x20 commands and 0x21 data  [master]
    // 0xA0 commands and 0xA1 data  [slave]
    out_port_b(0x20, 0x11); // init ports
    out_port_b(0xA0, 0x11);

    out_port_b(0x21, 0x20);
    out_port_b(0xA1, 0x28);

    out_port_b(0x21, 0x04);
    out_port_b(0xA1, 0x02);

    out_port_b(0x21, 0x01);
    out_port_b(0xA1, 0x01);

    out_port_b(0x21, 0x0);
    out_port_b(0xA1, 0x0);

    union idt_segment_selector_union sel = { .selector_value = 0x08 }; // 0b00001000, 0x08
    union idt_flags_union flags = { .flags = { .P = 1, .DPL = 0, .gate_type = IDT_GATE_TYPE_32_INTERRUPT } }; // 0b10001110, 0x8E
    set_idt_gate(0, (uint32_t)isr0, sel, flags);
    set_idt_gate(1, (uint32_t)isr1, sel, flags);
    set_idt_gate(2, (uint32_t)isr2, sel, flags);
    set_idt_gate(3, (uint32_t)isr3, sel, flags);
    set_idt_gate(4, (uint32_t)isr4, sel, flags);
    set_idt_gate(5, (uint32_t)isr5, sel, flags);
    set_idt_gate(6, (uint32_t)isr6, sel, flags);
    set_idt_gate(7, (uint32_t)isr7, sel, flags);
    set_idt_gate(8, (uint32_t)isr8, sel, flags);
    set_idt_gate(9, (uint32_t)isr9, sel, flags);
    set_idt_gate(10, (uint32_t)isr10, sel, flags);
    set_idt_gate(11, (uint32_t)isr11, sel, flags);
    set_idt_gate(12, (uint32_t)isr12, sel, flags);
    set_idt_gate(13, (uint32_t)isr13, sel, flags);
    set_idt_gate(14, (uint32_t)isr14, sel, flags);
    set_idt_gate(15, (uint32_t)isr15, sel, flags);
    set_idt_gate(16, (uint32_t)isr16, sel, flags);
    set_idt_gate(17, (uint32_t)isr17, sel, flags);
    set_idt_gate(18, (uint32_t)isr18, sel, flags);
    set_idt_gate(19, (uint32_t)isr19, sel, flags);
    set_idt_gate(20, (uint32_t)isr20, sel, flags);
    set_idt_gate(21, (uint32_t)isr21, sel, flags);
    set_idt_gate(22, (uint32_t)isr22, sel, flags);
    set_idt_gate(23, (uint32_t)isr23, sel, flags);
    set_idt_gate(24, (uint32_t)isr24, sel, flags);
    set_idt_gate(25, (uint32_t)isr25, sel, flags);
    set_idt_gate(26, (uint32_t)isr26, sel, flags);
    set_idt_gate(27, (uint32_t)isr27, sel, flags);
    set_idt_gate(28, (uint32_t)isr28, sel, flags);
    set_idt_gate(29, (uint32_t)isr29, sel, flags);
    set_idt_gate(30, (uint32_t)isr30, sel, flags);
    set_idt_gate(31, (uint32_t)isr31, sel, flags);

    // Interrupt request
    set_idt_gate(32, (uint32_t)irq0, sel, flags);
    set_idt_gate(33, (uint32_t)irq1, sel, flags);
    set_idt_gate(34, (uint32_t)irq2, sel, flags);
    set_idt_gate(35, (uint32_t)irq3, sel, flags);
    set_idt_gate(36, (uint32_t)irq4, sel, flags);
    set_idt_gate(37, (uint32_t)irq5, sel, flags);
    set_idt_gate(38, (uint32_t)irq6, sel, flags);
    set_idt_gate(39, (uint32_t)irq7, sel, flags);
    set_idt_gate(40, (uint32_t)irq8, sel, flags);
    set_idt_gate(41, (uint32_t)irq9, sel, flags);
    set_idt_gate(42, (uint32_t)irq10, sel, flags);
    set_idt_gate(43, (uint32_t)irq11, sel, flags);
    set_idt_gate(44, (uint32_t)irq12, sel, flags);
    set_idt_gate(45, (uint32_t)irq13, sel, flags);
    set_idt_gate(46, (uint32_t)irq14, sel, flags);
    set_idt_gate(47, (uint32_t)irq15, sel, flags);


    set_idt_gate(30, (uint32_t)isr128, sel, flags); // system calls
    set_idt_gate(31, (uint32_t)isr177, sel, flags); // system calls

    idt_flush((uint32_t)&idt_p);
}

void set_idt_gate(uint8_t num, uint32_t base, union idt_segment_selector_union selector, union idt_flags_union flags) {
    idt_entries[num].offset_low = base & 0xFFFF;
    idt_entries[num].offset_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector = selector.selector_value;
    idt_entries[num].flags = flags.flags_value;
}

unsigned char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Fault",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

void isr_handler(struct interrupt_registers_struct* regs) {
    if (regs->int_no < 32) {
        printf("Exception: %s\n", exception_messages[regs->int_no]);
        puts("System Halted!\n");
        while (true);
    }
}

void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

void irq_install_handler(int irq, void (*handler)(struct interrupt_registers_struct* r)) {
    irq_routines[irq] = handler;
}


void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

void irq_handler(struct interrupt_registers_struct* regs) {
    void (*handler)(struct interrupt_registers_struct* regs);
    handler = irq_routines[regs->int_no - 32];

    if (handler) {
        handler(regs);
    }

    if (regs->int_no >= 40) {
        out_port_b(0xA0, 0x20);
    }

    out_port_b(0x20, 0x20);
}