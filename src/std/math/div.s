


section .text
global x86_div64_32
x86_div64_32:
    PUSH ebp
    MOV ebp, esp

    PUSH ebx

    MOV eax, [ebp+12]   ; upper 32bits of dividends
    MOV ecx, [ebp+16]   ; divisor
    XOR edx, edx        ; will store remainder
    DIV ecx

    MOV ebx, [ebp+20]
    MOV [ebx+4], eax    ; upper 32bits of quotient

    MOV eax, [ebp+8]    ; lower 32bits of dividends
    DIV ecx

    MOV ebx, [ebp+20]
    MOV [ebx], eax      ; lower 32bits of quotient

    MOV ebx, [ebp+24]   ; remainder
    MOV [ebx], edx

    POP ebx

    MOV esp, ebp
    POP ebp

    RET