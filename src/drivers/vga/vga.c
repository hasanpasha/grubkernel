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
    case '\b':
        if (column == 0 && line != 0) {
            line--;
            column = __VGA_WIDTH__;
        }
        vga[line * __VGA_WIDTH__ + (--column)] = ' ' | current_color;
        break;
    case '\t':
        if (column == __VGA_WIDTH__) {
            new_line();
        }
        uint16_t tab_len = 4 - (column % 4);
        while (tab_len != 0) {
            vga[line * __VGA_WIDTH__ + (column++)] = ' ' | current_color;
            tab_len--;
        }
        break;
    default:
        if (column == __VGA_WIDTH__) {
            new_line();
        }
        vga[line * __VGA_WIDTH__ + (column++)] = c | current_color;
        break;
    }
}

void scroll_up() {
    for (uint16_t y = 0; y < __VGA_HEIGHT__; y++) {
        for (uint16_t x = 0; x < __VGA_WIDTH__; x++) {
            vga[(y-1) * __VGA_WIDTH__ + x] = vga[y * __VGA_WIDTH__ + x];
        }
    }

    for (uint16_t x = 0; x < __VGA_WIDTH__; x++) {
        vga[(__VGA_HEIGHT__-1) * __VGA_WIDTH__ + x] = ' ' | current_color;
    }
}

void new_line() {
    if (line < __VGA_HEIGHT__ - 1) {
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

    for (uint16_t y = 0; y < __VGA_HEIGHT__; y++) {
        for (uint16_t x = 0; x < __VGA_WIDTH__; x++) {
            vga[y * __VGA_WIDTH__ + x] = ' ' | default_color;
        }
    }
}

