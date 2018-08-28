#include "text.h"

///////////////////////////////////////////////////
//     Structs for internal use                  //
///////////////////////////////////////////////////

typedef struct glyph_descriptor glyph_descriptor_t;
struct glyph_descriptor
{
    uint32_t w_px;
    uint32_t glyph_index;
};

/**
 * This struct stores minimal meta-information for an extremely simple font.
 * a jmfont may not specify glyphs for all 256 ascii characters, but for simplicity and
 * compactness, the characters for which a font is specified must be contiguous.
 *
 * A font consists primarily of two arrays: an array of uint8_t containing glyphs, and an
 * array of glyph descriptors, which provide index information and width information for
 * each glyph.
 */
struct jmfont
{
    int ascii_first;       ///< ascii code of first character in font, inclusive.
    int ascii_last;        ///< ascii code of last character in font, inclusive.
    int h_px;              ///< height of font, in pixels

    const uint8_t *glyph_bitmap;            ///< pointer to the start of the glyphs
    const glyph_descriptor_t *glyph_dsc;    ///< pointer to array of glyph descriptors
};

/**
 * Given a character, returns the corresponding glyph descriptor in the font.
 *
 * If the character isn't in the fontset, the first glyph in the fontset is returned.
 */
static const glyph_descriptor_t* get_glyph_of_char(const jmfont_t *f, char ch)
{
    int glyph_idx = ch - f->ascii_first;
    if(glyph_idx > (f->ascii_last - f->ascii_first))
    {
        glyph_idx = 0;
    }
    return f->glyph_dsc + glyph_idx;
}

/**
 * Given a screen descriptor, a buffer, and coordinates, this function draws the glyph corresponding
 * to the given character on the buffer.
 *
 * wonkiness is because I'm trying to avoid multiplication.
 */
static void put_glyph(uint8_t *buf, uint8_t ch, const jmfont_screen_descriptor_t *sd,
                      const jmfont_t *font, int x, int y)
{
    int bufsize = sd->screen_width * sd->screen_height;
    const glyph_descriptor_t* gd = get_glyph_of_char(font, ch);
    const uint8_t *glyph_bm = &font->glyph_bitmap[gd->glyph_index];

    int ydelta = (sd->flipped_vertical) ? sd->screen_width : -sd->screen_width;
    int xdelta = (sd->flipped_horizontal) ? -1 : 1;

    int buf_x, buf_y;
    buf_y = (sd->flipped_vertical) ? (sd->screen_height - 1 - y) : (y);
    buf_y *= sd->screen_width;

    for(int pixy = font->h_px - 1; pixy >= 0; pixy--)
    {
        buf_x = (sd->flipped_horizontal) ? (sd->screen_width - 1 - x) : x;

        for(int pixx = 0; (pixx < gd->w_px); pixx++)
        {
            int idx = buf_x + buf_y;
            if((idx >= 0) && (idx < bufsize) && (buf_x >= 0) && (buf_x < sd->screen_width)
               && !!((0x80 >> pixx) & glyph_bm[pixy]))
            {
                buf[buf_x + buf_y] = 0xff;
            }
            buf_x += xdelta;
        }
        buf_y += ydelta;
    }
}

/**
 * Given a string, draws it to the buffer using the given font.
 */
static void text_draw_puts(uint8_t *buf, const char *s, const jmfont_screen_descriptor_t *sd,
                           const jmfont_t *font, int x, int y)
{
    int xn = x;
    for(; *s; s++)
    {
        put_glyph(buf, *s, sd, font, xn, y);
        const glyph_descriptor_t* gd = get_glyph_of_char(font, *s);
        xn += gd->w_px + 1;
    }
}

static int text_get_width(const jmfont_t *font, const char *s)
{
    int xn = 0;
    for(; *s; s++)
    {
        const glyph_descriptor_t* gd = get_glyph_of_char(font, *s);
        xn += gd->w_px + 1;
    }

    if (xn > 0)
        xn--;

    return xn;
}

void text_scroller_init(text_scroller_t *ts, const jmfont_screen_descriptor_t* sd,
                        const jmfont_t *font, int y, int step_time_us, const char *s)
{
    ts->jsd = *sd;
    ts->font = font;
    ts->x = sd->screen_width;
    ts->y = y;
    ts->step_time = step_time_us;
    ts->time_count = 0;
    ts->s = s;
    ts->str_width = text_get_width(font, s);
}

void text_scroller_draw(uint8_t *buf, text_scroller_t *ts)
{
    text_draw_puts(buf, ts->s, &ts->jsd, ts->font, ts->x, ts->y);
}

void text_scroller_advance(text_scroller_t *ts, int dt_usec)
{
    ts->time_count += dt_usec;

    while (ts->time_count >= ts->step_time) {
        ts->x--;
        ts->time_count -= ts->step_time;
    }

    if((ts->x + ts->str_width) < 0)
    {
        ts->x = ts->jsd.screen_width;
    }
}

void text_scroller_reset(text_scroller_t *ts)
{
    ts->x = ts->jsd.screen_width;
    ts->time_count = 0;
}


///////////////////////////
// FONT DECLARATIONS
///////////////////////////
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

const jmfont_t jmtext_font_3x5 =
{
    .ascii_first = 32,	/*First Ascii letter in this font*/
    .ascii_last = 95,	/*Last Ascii letter in this font*/
    .h_px = 5,				/*Font height in pixels*/
    .glyph_bitmap = glyph_bitmap_3x5_font,	/*Bitmap of glyphs*/
    .glyph_dsc = glyphs_3x5		/*Description of glyphs*/
};
