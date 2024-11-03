#include "vga.h"
#include "../include/stdint.h"

uint16_t column = 0;
uint16_t line = 0;
uint16_t* const vga = (uint16_t* const)0xB8000;
const uint16_t default_color = (COLOR8_LIGHT_GRAY << 8) | (COLOR8_BLACK << 12);
uint16_t current_color = default_color;

void drawc(char c) {
    switch (c)
    {
    case '\n':
        new_line();
        break;
    case '\r':
        column = 0;
        break;
    case '\t':
        if (column == width) {
            new_line();
        }
        uint16_t tab_len = 4 - (column % 4);
        while (tab_len != 0) {
            vga[line * width + (column++)] = ' ' | current_color;
        }
        break;
    default:
        if (column == width) {
            new_line();
        }
        vga[line * width + (column++)] = c | current_color;
        break;
    }
}

void puts(const char* str) {
    while (*str) {
        drawc(*str);
        str++;
    }
}

void scroll_up() {
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            vga[(y-1) * width + x] = vga[y * width + x];
        }
    }

    for (uint16_t x = 0; x < width; x++) {
        vga[(height-1) * width + x] = ' ' | current_color;
    }
}

void new_line() {
    if (line < height - 1) {
        line++;
    } else {
        scroll_up();
    }
    column = 0;
}

void reset() {
    line = 0;
    column = 0;
    current_color = default_color;

    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            vga[y * width + x] = ' ' | default_color;
        }
    }
}

