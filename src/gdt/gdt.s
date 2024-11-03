
global gdt_flush
gdt_flush:
    MOV ebx, [esp+4]
    LGDT [ebx]

    XOR eax, eax
    MOV eax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax
    MOV ss, ax

    JMP 0x08:.flush ; set code segment

.flush:
    RET

global tss_flush
tss_flush:
    MOV ax, 0x2B ; offset of tss segment entry
    LTR ax
    RET
