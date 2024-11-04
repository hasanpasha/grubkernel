[BITS 32]
extern kmain

section .text
    ALIGN 4
    DD 0x1BADB002
    DD 0x00000003
    DD -(0x1BADB002 + 0x00000003)

global start
start:
    CLI
    MOV esp, stack_space
    PUSH ebx
    PUSH eax
    CALL kmain

    HLT

halt:
    CLI
    HLT
    JMP halt

section .bss
RESB 0x2000
stack_space:
