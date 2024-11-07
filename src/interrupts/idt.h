#pragma once

#include "../include/stdint.h"
#include "../include/common.h"

typedef enum {
    IDT_GATE_TYPE_TASK = 0x5,
    IDT_GATE_TYPE_16_INTERRUPT = 0x6,
    IDT_GATE_TYPE_16_TRAP = 0x7,
    IDT_GATE_TYPE_32_INTERRUPT = 0xE,
    IDT_GATE_TYPE_32_TRAP = 0xF,
} idt_gate_type_t;

struct idt_entry_flags_struct {
    uint8_t gate_type : 4;  // the type of gate this interrupt represents [idt_gate_type]
    uint8_t pad1 : 1;
    uint8_t DPL : 2;        // allowed cpu privilege level to access this interrupt
    uint8_t P : 1;          // present bit, must be 1 for the descriptor to be valid
} __attribute__((packed));
typedef struct idt_entry_flags_struct idt_entry_flags_t;

union idt_flags_union {
    uint8_t flags_value;
    struct idt_entry_flags_struct flags;
} __attribute__((packed));

struct idt_entry_segment_selector_struct {
    uint16_t RPL : 2;   // privilege level
    uint16_t TI : 1;    // 0 -> gdt, 1 -> ldt
    uint16_t index : 13;// index of gdt or ldt entry
} __attribute__((packed));
typedef struct idt_entry_segment_selector_struct idt_entry_segment_selector_t;

union idt_segment_selector_union {
    uint16_t selector_value;
    struct idt_entry_segment_selector_struct selector;
} __attribute__((packed));

struct idt_entry_struct {
    uint16_t offset_low;    // address of entry of point of interrupt service routine
    uint16_t selector;      // segment selector
    uint8_t pad0;
    uint8_t flags;          // [idt_entr_flags_struct]
    uint16_t offset_high;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

void init_idt();
void set_idt_gate(uint8_t num, uint32_t base, union idt_segment_selector_union selector, union idt_flags_union flags);

void isr_handler(struct interrupt_registers_struct* regs);
void irq_install_handler(int irq, void (*handler)(struct interrupt_registers_struct*));
void irq_uninstall_handler(int irq);
void irq_handler(struct interrupt_registers_struct* regs);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();


extern void isr128();
extern void isr177();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();