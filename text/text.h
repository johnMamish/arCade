#ifndef _TEXT_H
#define _TEXT_H

#include <stdint.h>

// jmfont_t's definition is given in text.c
typedef struct jmfont jmfont_t;

//////////////////////////////////////////
//  These fonts should be supplied to text_scroller_init.
//  Right now we only have 1 font.
//////////////////////////////////////////
extern const jmfont_t jmtext_font_3x5;

typedef struct jmfont_screen_descriptor jmfont_screen_descriptor_t;
struct jmfont_screen_descriptor
{
    uint32_t screen_width;    ///< pixel width of buffer
    uint32_t screen_height;   ///< pixel height of buffer

    ///< For the below two fields, default "screen format" is
    ///<     buf[0]      -->  (0, 0)
    ///<     buf[1]      -->  (0, 1)
    ///<     buf[width]  -->  (width, 0)
    ///< where (0, 0) is the top left. If buf[0] / (0, 0) is not the top left, these fields should
    ///< be used.
    int flipped_vertical;
    int flipped_horizontal;
};

typedef struct text_scroller text_scroller_t;
struct text_scroller
{
    jmfont_screen_descriptor_t jsd;

    const jmfont_t *font;

    int x;
    int y;

    int step_time;
    int time_count;

    int str_width;
    const char *s;
};

void text_scroller_init(text_scroller_t *ts, const jmfont_screen_descriptor_t* sd, const jmfont_t *font,
                        int y, int step_time_us, const char *s);
void text_scroller_draw(uint8_t *buf, text_scroller_t *ts);
void text_scroller_advance(text_scroller_t *ts, int dt_usec);
void text_scroller_reset(text_scroller_t *ts);

#endif
