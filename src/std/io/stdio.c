#include "../../include/stdio.h"
#include "../../include/stdbool.h"
#include "../../drivers/vga/vga.h"
#include "../../include/stdmath.h"
#include "../math/div.h"

void putc(char c) {
    drawc(c);
}

void puts(const char* str) {
    while (*str != 0){
        putc(*str);
        str++;
    }
}

void puts_f(const char* str) {
    while (*str != 0){
        putc(*str);
        str++;
    }
}

void printf(const char* fmt, ...) {
    int* argp = (int*) &fmt;
    int state = PRINTF_STATE_START;
    int length = PRINTF_LENGTH_START;
    int radix = 10;
    bool sign = false;
    bool upper = false;

    argp++;
    while (*fmt != 0) {
        switch (state)
        {
        case PRINTF_STATE_START:
            switch (*fmt)
            {
            case '%':
                state = PRINTF_STATE_LENGTH;
                break;
            default:
                putc(*fmt);
                break;
            }
            break;

        case PRINTF_STATE_LENGTH:
            switch (*fmt)
            {
            case 'h':
                length = PRINTF_LENGTH_SHORT;
                state = PRINTF_STATE_SHORT;
                break;
            case 'l':
                length = PRINTF_LENGTH_LONG;
                state = PRINTF_STATE_LONG;
                break;
            default:
                goto PRINTF_STATE_SPEC_;
                break;
            }
            break;

        case PRINTF_STATE_SHORT:
            switch (*fmt)
            {
            case 'h':
                length = PRINTF_LENGTH_SHORT_SHORT;
                state = PRINTF_STATE_SPEC;
                break;
            default:
                goto PRINTF_STATE_SPEC_;
                break;
            }
            break;

        case PRINTF_STATE_LONG:
            switch (*fmt)
            {
            case 'l':
                length = PRINTF_LENGTH_LONG_LONG;
                state = PRINTF_STATE_SPEC;
                break;
            default:
                goto PRINTF_STATE_SPEC_;
                break;
            }
            break;

        case PRINTF_STATE_SPEC:
            PRINTF_STATE_SPEC_:
                switch (*fmt)
                {
                case 'c':
                    putc((char)*argp);
                    argp++;
                    break;
                case 's':
                    switch (length)
                    {
                    case PRINTF_LENGTH_LONG:
                    case PRINTF_LENGTH_LONG_LONG:
                        puts_f(*(const char**)argp);
                        argp += 2;
                        break;
                    default:
                        puts(*(const char**)argp);
                        argp++;
                        break;
                    }
                    break;
                case '%':
                    putc('%');
                    break;
                case 'd':
                case 'i':
                    radix = 10;
                    sign = true;
                    upper = false;
                    argp = printf_number(argp, length, sign, radix, upper);
                    break;
                case 'u':
                    radix = 10;
                    sign = false;
                    upper = false;
                    argp = printf_number(argp, length, sign, radix, upper);
                    break;
                case 'x':
                case 'p':
                    radix = 16;
                    sign = false;
                    upper = false;
                    argp = printf_number(argp, length, sign, radix, upper);
                    break;
                case 'X':
                    radix = 16;
                    sign = false;
                    upper = true;
                    argp = printf_number(argp, length, sign, radix, upper);
                    break;
                case 'o':
                    radix = 8;
                    sign = false;
                    upper = false;
                    argp = printf_number(argp, length, sign, radix, upper);
                    break;
                case 'b':
                    radix = 2;
                    sign = false;
                    upper = false;
                    argp = printf_number(argp, length, sign, radix, upper);
                    break;
                default:
                    break;
                }
            state = PRINTF_STATE_START;
            length = PRINTF_LENGTH_START;
            radix = 10;
            sign = false;
            upper = false;
            break;
        }
        fmt++;
    }
}



const char possible_chars[] = "0123456789abcdef";
int* printf_number(int* argp, int length, bool sign, int radix, bool upper) {
    char buffer[32];
    unsigned long long number = 0;
    int number_sign = 1;
    int pos = 0;

    switch (length)
    {
    case PRINTF_LENGTH_SHORT:
    case PRINTF_LENGTH_SHORT_SHORT:
    case PRINTF_LENGTH_START:
        if (sign) {
            int n = *argp;
            if (n < 0) {
                n = -n;
                number_sign = -1;
            }
            number = (unsigned long long) n;
        } else {
            number = *(unsigned int*) argp;
        }
        argp++;
        break;
    case PRINTF_LENGTH_LONG:
        if (sign) {
            long int n = *(long int*) argp;
            if (n < 0) {
                n = -n;
                number_sign = -1;
            }
            number = (unsigned long long) n;
        } else {
            number = *(unsigned long int*) argp;
        }
        argp += 2;
        break;
    case PRINTF_LENGTH_LONG_LONG:
        if (sign) {
            long long int n = *(long long int*)argp;
            if (n < 0) {
                n = -n;
                number_sign = -1;
            }
            number = (unsigned long long) n;
        } else {
            number = *(unsigned long long int*) argp;
        }
        argp += 4;
        break;
    }

    uint32_t rem;
    do {
        // rem = number % radix;
        // number = number / radix;
        x86_div64_32(number, radix, &number, &rem);
        buffer[pos++] = possible_chars[rem];
    } while(number > 0);

    if (sign && number_sign < 0) {
        buffer[pos++] = '-';
    }

    while (--pos >= 0) {
        char c = buffer[pos];
        if (upper) {
            c = toupper(c);
        }
        putc(c);
    }

    return argp;
}

bool islower(char c) {
    return (c >= 0x61 && c <= 0x7a);
}

char toupper(char c) {
    return islower(c) ? c - 'a' + 'A' : c;
}