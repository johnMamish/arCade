#ifndef _3x5_FONT_H
#define _3x5_FONT_H

#include <stdint.h>

typedef struct text_scroller text_scroller_t;
struct text_scroller
{
    int x;
    int y;

    int screen_width;
    int screen_height;

    int step_time;
    int time_count;

    int str_width;
    const char *s;
};

void text_draw_puts(uint8_t *buf, int width, int height, const char *s, int x, int y);

void text_scroller_init(text_scroller_t *ts, int y, int screen_width,
                        int screen_height, int step_time_us, const char *s);
void text_scroller_draw(uint8_t *buf, text_scroller_t *ts);
void text_scroller_advance(text_scroller_t *ts, int dt_usec);
void text_scroller_reset(text_scroller_t *ts);

int text_get_width(const char *s);

#endif
