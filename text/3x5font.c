#include "3x5font.h"
#include <stdint.h>

/*Store the image of the letters (glyph)*/
static const uint8_t glyph_bitmap_3x5_font[] =
{
  /*Unicode: U+0020 ( ) , Width: 2 */
  0x00,  //..
  0x00,  //..
  0x00,  //..
  0x00,  //..
  0x00,  //..


  /*Unicode: U+0021 (!) , Width: 1 */
  0x80,  //%
  0x80,  //%
  0x80,  //%
  0x00,  //.
  0x80,  //%


  /*Unicode: U+0022 (") , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0x00,  //...
  0x00,  //...
  0x00,  //...


  /*Unicode: U+0023 (#) , Width: 3 */
  0xa0,  //%.%
  0xe0,  //%%%
  0xa0,  //%.%
  0xe0,  //%%%
  0xa0,  //%.%


  /*Unicode: U+0024 ($) , Width: 3 */
  0xe0,  //%%%
  0xc0,  //%%.
  0xe0,  //%%%
  0x60,  //.%%
  0xe0,  //%%%


  /*Unicode: U+0025 (%) , Width: 3 */
  0xa0,  //%.%
  0x20,  //..%
  0x40,  //.%.
  0x80,  //%..
  0xa0,  //%.%


  /*Unicode: U+0026 (&) , Width: 3 */
  0xc0,  //%%.
  0x80,  //%..
  0xe0,  //%%%
  0x80,  //%..
  0xe0,  //%%%


  /*Unicode: U+0027 (') , Width: 1 */
  0x80,  //%
  0x80,  //%
  0x00,  //.
  0x00,  //.
  0x00,  //.


  /*Unicode: U+0028 (() , Width: 2 */
  0x40,  //.%
  0x80,  //%.
  0x80,  //%.
  0x80,  //%.
  0x40,  //.%


  /*Unicode: U+0029 ()) , Width: 2 */
  0x80,  //%.
  0x40,  //.%
  0x40,  //.%
  0x40,  //.%
  0x80,  //%.


  /*Unicode: U+002a (*) , Width: 3 */
  0xa0,  //%.%
  0x40,  //.%.
  0xa0,  //%.%
  0x00,  //...
  0x00,  //...


  /*Unicode: U+002b (+) , Width: 3 */
  0x00,  //...
  0x40,  //.%.
  0xe0,  //%%%
  0x40,  //.%.
  0x00,  //...


  /*Unicode: U+002c (,) , Width: 2 */
  0x00,  //..
  0x00,  //..
  0x00,  //..
  0xc0,  //%%
  0x40,  //.%


  /*Unicode: U+002d (-) , Width: 3 */
  0x00,  //...
  0x00,  //...
  0xe0,  //%%%
  0x00,  //...
  0x00,  //...


  /*Unicode: U+002e (.) , Width: 1 */
  0x00,  //.
  0x00,  //.
  0x00,  //.
  0x00,  //.
  0x80,  //%


  /*Unicode: U+002f (/) , Width: 3 */
  0x20,  //..%
  0x20,  //..%
  0x40,  //.%.
  0x80,  //%..
  0x80,  //%..


  /*Unicode: U+0030 (0) , Width: 3 */
  0x40,  //.%.
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0x40,  //.%.


  /*Unicode: U+0031 (1) , Width: 3 */
  0x40,  //.%.
  0xc0,  //%%.
  0x40,  //.%.
  0x40,  //.%.
  0xe0,  //%%%


  /*Unicode: U+0032 (2) , Width: 3 */
  0xc0,  //%%.
  0x20,  //..%
  0x40,  //.%.
  0x80,  //%..
  0xe0,  //%%%


  /*Unicode: U+0033 (3) , Width: 3 */
  0xe0,  //%%%
  0x20,  //..%
  0x60,  //.%%
  0x20,  //..%
  0xe0,  //%%%


  /*Unicode: U+0034 (4) , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0xe0,  //%%%
  0x20,  //..%
  0x20,  //..%


  /*Unicode: U+0035 (5) , Width: 3 */
  0xe0,  //%%%
  0x80,  //%..
  0xe0,  //%%%
  0x20,  //..%
  0xc0,  //%%.


  /*Unicode: U+0036 (6) , Width: 3 */
  0x60,  //.%%
  0x80,  //%..
  0xe0,  //%%%
  0xa0,  //%.%
  0xe0,  //%%%


  /*Unicode: U+0037 (7) , Width: 3 */
  0xe0,  //%%%
  0x20,  //..%
  0x40,  //.%.
  0x80,  //%..
  0x80,  //%..


  /*Unicode: U+0038 (8) , Width: 3 */
  0xe0,  //%%%
  0xa0,  //%.%
  0xe0,  //%%%
  0xa0,  //%.%
  0xe0,  //%%%


  /*Unicode: U+0039 (9) , Width: 3 */
  0xe0,  //%%%
  0xa0,  //%.%
  0xe0,  //%%%
  0x20,  //..%
  0xc0,  //%%.


  /*Unicode: U+003a (:) , Width: 1 */
  0x00,  //.
  0x80,  //%
  0x00,  //.
  0x80,  //%
  0x00,  //.


  /*Unicode: U+003b (;) , Width: 2 */
  0x00,  //..
  0x40,  //.%
  0x00,  //..
  0xc0,  //%%
  0x40,  //.%


  /*Unicode: U+003c (<) , Width: 3 */
  0x20,  //..%
  0x40,  //.%.
  0x80,  //%..
  0x40,  //.%.
  0x20,  //..%


  /*Unicode: U+003d (=) , Width: 3 */
  0x00,  //...
  0xe0,  //%%%
  0x00,  //...
  0xe0,  //%%%
  0x00,  //...


  /*Unicode: U+003e (>) , Width: 3 */
  0x80,  //%..
  0x40,  //.%.
  0x20,  //..%
  0x40,  //.%.
  0x80,  //%..


  /*Unicode: U+003f (?) , Width: 3 */
  0xe0,  //%%%
  0x20,  //..%
  0x60,  //.%%
  0x00,  //...
  0x40,  //.%.


  /*Unicode: U+0040 (@) , Width: 3 */
  0xe0,  //%%%
  0xa0,  //%.%
  0xa0,  //%.%
  0x80,  //%..
  0xe0,  //%%%


  /*Unicode: U+0041 (A) , Width: 3 */
  0xe0,  //%%%
  0xa0,  //%.%
  0xe0,  //%%%
  0xa0,  //%.%
  0xa0,  //%.%


  /*Unicode: U+0042 (B) , Width: 3 */
  0xc0,  //%%.
  0xa0,  //%.%
  0xc0,  //%%.
  0xa0,  //%.%
  0xc0,  //%%.


  /*Unicode: U+0043 (C) , Width: 3 */
  0x60,  //.%%
  0x80,  //%..
  0x80,  //%..
  0x80,  //%..
  0x60,  //.%%


  /*Unicode: U+0044 (D) , Width: 3 */
  0xc0,  //%%.
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0xc0,  //%%.


  /*Unicode: U+0045 (E) , Width: 3 */
  0xe0,  //%%%
  0x80,  //%..
  0xe0,  //%%%
  0x80,  //%..
  0xe0,  //%%%


  /*Unicode: U+0046 (F) , Width: 3 */
  0xe0,  //%%%
  0x80,  //%..
  0xe0,  //%%%
  0x80,  //%..
  0x80,  //%..


  /*Unicode: U+0047 (G) , Width: 3 */
  0x60,  //.%%
  0x80,  //%..
  0xa0,  //%.%
  0xa0,  //%.%
  0x60,  //.%%


  /*Unicode: U+0048 (H) , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0xe0,  //%%%
  0xa0,  //%.%
  0xa0,  //%.%


  /*Unicode: U+0049 (I) , Width: 3 */
  0xe0,  //%%%
  0x40,  //.%.
  0x40,  //.%.
  0x40,  //.%.
  0xe0,  //%%%


  /*Unicode: U+004a (J) , Width: 3 */
  0x20,  //..%
  0x20,  //..%
  0x20,  //..%
  0xa0,  //%.%
  0xe0,  //%%%


  /*Unicode: U+004b (K) , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0xc0,  //%%.
  0xa0,  //%.%
  0xa0,  //%.%


  /*Unicode: U+004c (L) , Width: 3 */
  0x80,  //%..
  0x80,  //%..
  0x80,  //%..
  0x80,  //%..
  0xe0,  //%%%


  /*Unicode: U+004d (M) , Width: 3 */
  0xa0,  //%.%
  0xe0,  //%%%
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%


  /*Unicode: U+004e (N) , Width: 3 */
  0xe0,  //%%%
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%


  /*Unicode: U+004f (O) , Width: 3 */
  0xe0,  //%%%
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0xe0,  //%%%


  /*Unicode: U+0050 (P) , Width: 3 */
  0xe0,  //%%%
  0xa0,  //%.%
  0xe0,  //%%%
  0x80,  //%..
  0x80,  //%..


  /*Unicode: U+0051 (Q) , Width: 3 */
  0xc0,  //%%.
  0xc0,  //%%.
  0xc0,  //%%.
  0xc0,  //%%.
  0xe0,  //%%%


  /*Unicode: U+0052 (R) , Width: 3 */
  0xc0,  //%%.
  0xa0,  //%.%
  0xc0,  //%%.
  0xa0,  //%.%
  0xa0,  //%.%


  /*Unicode: U+0053 (S) , Width: 3 */
  0x60,  //.%%
  0x80,  //%..
  0x40,  //.%.
  0x20,  //..%
  0xc0,  //%%.


  /*Unicode: U+0054 (T) , Width: 3 */
  0xe0,  //%%%
  0x40,  //.%.
  0x40,  //.%.
  0x40,  //.%.
  0x40,  //.%.


  /*Unicode: U+0055 (U) , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0xe0,  //%%%


  /*Unicode: U+0056 (V) , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0xc0,  //%%.


  /*Unicode: U+0057 (W) , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0xa0,  //%.%
  0xe0,  //%%%
  0xa0,  //%.%


  /*Unicode: U+0058 (X) , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0x40,  //.%.
  0xa0,  //%.%
  0xa0,  //%.%


  /*Unicode: U+0059 (Y) , Width: 3 */
  0xa0,  //%.%
  0xa0,  //%.%
  0xe0,  //%%%
  0x40,  //.%.
  0x40,  //.%.


  /*Unicode: U+005a (Z) , Width: 3 */
  0xe0,  //%%%
  0x20,  //..%
  0x40,  //.%.
  0x80,  //%..
  0xe0,  //%%%


  /*Unicode: U+005b ([) , Width: 2 */
  0xc0,  //%%
  0x80,  //%.
  0x80,  //%.
  0x80,  //%.
  0xc0,  //%%


  /*Unicode: U+005c (\) , Width: 3 */
  0x80,  //%..
  0x80,  //%..
  0x40,  //.%.
  0x20,  //..%
  0x20,  //..%


  /*Unicode: U+005d (]) , Width: 2 */
  0xc0,  //%%
  0x40,  //.%
  0x40,  //.%
  0x40,  //.%
  0xc0,  //%%


  /*Unicode: U+005e (^) , Width: 3 */
  0x40,  //.%.
  0xa0,  //%.%
  0xa0,  //%.%
  0x00,  //...
  0x00,  //...


  /*Unicode: U+005f (_) , Width: 3 */
  0x00,  //...
  0x00,  //...
  0x00,  //...
  0x00,  //...
  0xe0,  //%%%


};

typedef struct glyph_descriptor glyph_descriptor_t;
struct glyph_descriptor
{
    uint32_t w_px;
    uint32_t glyph_index;
};

/*Store the glyph descriptions*/
static const glyph_descriptor_t glyphs_3x5[] =
{
  {.w_px = 2,	.glyph_index = 0},	/*Unicode: U+0020 ( )*/
  {.w_px = 1,	.glyph_index = 5},	/*Unicode: U+0021 (!)*/
  {.w_px = 3,	.glyph_index = 10},	/*Unicode: U+0022 (")*/
  {.w_px = 3,	.glyph_index = 15},	/*Unicode: U+0023 (#)*/
  {.w_px = 3,	.glyph_index = 20},	/*Unicode: U+0024 ($)*/
  {.w_px = 3,	.glyph_index = 25},	/*Unicode: U+0025 (%)*/
  {.w_px = 3,	.glyph_index = 30},	/*Unicode: U+0026 (&)*/
  {.w_px = 1,	.glyph_index = 35},	/*Unicode: U+0027 (')*/
  {.w_px = 2,	.glyph_index = 40},	/*Unicode: U+0028 (()*/
  {.w_px = 2,	.glyph_index = 45},	/*Unicode: U+0029 ())*/
  {.w_px = 3,	.glyph_index = 50},	/*Unicode: U+002a (*)*/
  {.w_px = 3,	.glyph_index = 55},	/*Unicode: U+002b (+)*/
  {.w_px = 2,	.glyph_index = 60},	/*Unicode: U+002c (,)*/
  {.w_px = 3,	.glyph_index = 65},	/*Unicode: U+002d (-)*/
  {.w_px = 1,	.glyph_index = 70},	/*Unicode: U+002e (.)*/
  {.w_px = 3,	.glyph_index = 75},	/*Unicode: U+002f (/)*/
  {.w_px = 3,	.glyph_index = 80},	/*Unicode: U+0030 (0)*/
  {.w_px = 3,	.glyph_index = 85},	/*Unicode: U+0031 (1)*/
  {.w_px = 3,	.glyph_index = 90},	/*Unicode: U+0032 (2)*/
  {.w_px = 3,	.glyph_index = 95},	/*Unicode: U+0033 (3)*/
  {.w_px = 3,	.glyph_index = 100},	/*Unicode: U+0034 (4)*/
  {.w_px = 3,	.glyph_index = 105},	/*Unicode: U+0035 (5)*/
  {.w_px = 3,	.glyph_index = 110},	/*Unicode: U+0036 (6)*/
  {.w_px = 3,	.glyph_index = 115},	/*Unicode: U+0037 (7)*/
  {.w_px = 3,	.glyph_index = 120},	/*Unicode: U+0038 (8)*/
  {.w_px = 3,	.glyph_index = 125},	/*Unicode: U+0039 (9)*/
  {.w_px = 1,	.glyph_index = 130},	/*Unicode: U+003a (:)*/
  {.w_px = 2,	.glyph_index = 135},	/*Unicode: U+003b (;)*/
  {.w_px = 3,	.glyph_index = 140},	/*Unicode: U+003c (<)*/
  {.w_px = 3,	.glyph_index = 145},	/*Unicode: U+003d (=)*/
  {.w_px = 3,	.glyph_index = 150},	/*Unicode: U+003e (>)*/
  {.w_px = 3,	.glyph_index = 155},	/*Unicode: U+003f (?)*/
  {.w_px = 3,	.glyph_index = 160},	/*Unicode: U+0040 (@)*/
  {.w_px = 3,	.glyph_index = 165},	/*Unicode: U+0041 (A)*/
  {.w_px = 3,	.glyph_index = 170},	/*Unicode: U+0042 (B)*/
  {.w_px = 3,	.glyph_index = 175},	/*Unicode: U+0043 (C)*/
  {.w_px = 3,	.glyph_index = 180},	/*Unicode: U+0044 (D)*/
  {.w_px = 3,	.glyph_index = 185},	/*Unicode: U+0045 (E)*/
  {.w_px = 3,	.glyph_index = 190},	/*Unicode: U+0046 (F)*/
  {.w_px = 3,	.glyph_index = 195},	/*Unicode: U+0047 (G)*/
  {.w_px = 3,	.glyph_index = 200},	/*Unicode: U+0048 (H)*/
  {.w_px = 3,	.glyph_index = 205},	/*Unicode: U+0049 (I)*/
  {.w_px = 3,	.glyph_index = 210},	/*Unicode: U+004a (J)*/
  {.w_px = 3,	.glyph_index = 215},	/*Unicode: U+004b (K)*/
  {.w_px = 3,	.glyph_index = 220},	/*Unicode: U+004c (L)*/
  {.w_px = 3,	.glyph_index = 225},	/*Unicode: U+004d (M)*/
  {.w_px = 3,	.glyph_index = 230},	/*Unicode: U+004e (N)*/
  {.w_px = 3,	.glyph_index = 235},	/*Unicode: U+004f (O)*/
  {.w_px = 3,	.glyph_index = 240},	/*Unicode: U+0050 (P)*/
  {.w_px = 3,	.glyph_index = 245},	/*Unicode: U+0051 (Q)*/
  {.w_px = 3,	.glyph_index = 250},	/*Unicode: U+0052 (R)*/
  {.w_px = 3,	.glyph_index = 255},	/*Unicode: U+0053 (S)*/
  {.w_px = 3,	.glyph_index = 260},	/*Unicode: U+0054 (T)*/
  {.w_px = 3,	.glyph_index = 265},	/*Unicode: U+0055 (U)*/
  {.w_px = 3,	.glyph_index = 270},	/*Unicode: U+0056 (V)*/
  {.w_px = 3,	.glyph_index = 275},	/*Unicode: U+0057 (W)*/
  {.w_px = 3,	.glyph_index = 280},	/*Unicode: U+0058 (X)*/
  {.w_px = 3,	.glyph_index = 285},	/*Unicode: U+0059 (Y)*/
  {.w_px = 3,	.glyph_index = 290},	/*Unicode: U+005a (Z)*/
  {.w_px = 2,	.glyph_index = 295},	/*Unicode: U+005b ([)*/
  {.w_px = 3,	.glyph_index = 300},	/*Unicode: U+005c (\)*/
  {.w_px = 2,	.glyph_index = 305},	/*Unicode: U+005d (])*/
  {.w_px = 3,	.glyph_index = 310},	/*Unicode: U+005e (^)*/
  {.w_px = 3,	.glyph_index = 315},	/*Unicode: U+005f (_)*/
};

typedef struct font font_t;
struct font
{
    int ascii_first;
    int ascii_last;
    int h_px;
    const uint8_t *glyph_bitmap;
    const glyph_descriptor_t *glyph_dsc;
};

const font_t font_3x5 =
{
    .ascii_first = 32,	/*First Ascii letter in this font*/
    .ascii_last = 95,	/*Last Ascii letter in this font*/
    .h_px = 5,				/*Font height in pixels*/
    .glyph_bitmap = glyph_bitmap_3x5_font,	/*Bitmap of glyphs*/
    .glyph_dsc = glyphs_3x5		/*Description of glyphs*/
};

static const glyph_descriptor_t* get_glyph_of_char(char ch)
{
    int glyph_idx = ch - font_3x5.ascii_first;
    if(glyph_idx > (font_3x5.ascii_last - font_3x5.ascii_first))
    {
        glyph_idx = ' ' - font_3x5.ascii_first;
    }
    return font_3x5.glyph_dsc + glyph_idx;
}

static void put_glyph(uint8_t *buf, int width, int height, uint8_t ch, int x, int y)
{
    const glyph_descriptor_t* gd = get_glyph_of_char(ch);
    const uint8_t *glyph_bm = &glyph_bitmap_3x5_font[gd->glyph_index];
    for(int py = 0; (py < font_3x5.h_px) && ((y + py) < height); py++)
    {
        for(int px = 0; (px < gd->w_px) && ((x + px) < width); px++)
        {
            if(((x + px) >= 0) && !!((0x80 >> px) & glyph_bm[py]))
            {
                buf[(width * (y + py)) + px + x] = 0xff;
            }
        }
    }
}

/**
 *
 */
void text_draw_puts(uint8_t *buf, int width, int height, const char *s, int x, int y)
{
    int xn = x;
    for(; *s; s++)
    {
        put_glyph(buf, width, height, *s, xn, y);
        const glyph_descriptor_t* gd = get_glyph_of_char(*s);
        xn += gd->w_px + 1;
    }
}

int text_get_width(const char *s)
{
    int xn = 0;
    for(; *s; s++)
    {
        const glyph_descriptor_t* gd = get_glyph_of_char(*s);
        xn += gd->w_px + 1;
    }

    if (xn > 0)
        xn--;

    return xn;
}

void text_scroller_init(text_scroller_t *ts, int y, int width, int height, int step_time_us,
                        const char *s)
{
    ts->x = width;
    ts->y = y;
    ts->screen_width = width;
    ts->screen_height = height;
    ts->step_time = step_time_us;
    ts->time_count = 0;
    ts->s = s;
    ts->str_width = text_get_width(s);
}

void text_scroller_draw(uint8_t *buf, text_scroller_t *ts)
{
    text_draw_puts(buf, ts->screen_width, ts->screen_height, ts->s, ts->x, ts->y);
}

void text_scroller_advance(text_scroller_t *ts, int dt_usec)
{
    ts->time_count += dt_usec;

    int steps = (ts->time_count / ts->step_time);
    ts->x -= steps;
    if((ts->x + ts->str_width) < 0)
    {
        ts->x = ts->screen_width;
    }

    ts->time_count -= (steps * ts->time_count);
}

void text_scroller_reset(text_scroller_t *ts)
{
    ts->x = ts->screen_width;
    ts->time_count = 0;
}
