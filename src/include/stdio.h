#pragma once

#include "./stdint.h"
#include "./stdbool.h"
#include "../drivers/vga/vga.h"

// void putc(char c);
void puts(const char* str);
void printf(const char* fmt, ...);
void puts_f(const char* str);
int* printf_number(int* argp, int length, bool sign, int radix, bool upper);
bool islower(char c);
char toupper(char c);

#define PRINTF_STATE_START 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_SHORT 2
#define PRINTF_STATE_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_START 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4
