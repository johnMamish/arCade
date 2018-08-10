#ifndef _3x5_FONT_H
#define _3x5_FONT_H

#include <stdint.h>


void text_draw_puts(uint8_t *buf, int width, int height, const char *s, int x, int y);

int text_get_width(const char *s);

#endif
