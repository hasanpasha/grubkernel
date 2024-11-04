#pragma once

#define COLOR8_BLACK 0
#define COLOR8_LIGHT_GRAY 7

#define __VGA_WIDTH__ 80
#define __VGA_HEIGHT__ 25

void drawc(char c);
void scroll_up();
void new_line();
void reset();

