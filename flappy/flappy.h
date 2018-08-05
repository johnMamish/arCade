#include <stdint.h>

#ifndef FLAPPY_H
#define FLAPPY_H

/***************************************************************
 *                      typedefs                              **
 **************************************************************/

typedef struct jsnake_point
{
    uint8_t x, y;
} jsnake_point_t;

/**
 * Current state of the game
 */
typedef struct jflappy_game jflappy_game_t;
typedef struct jflappy_game
{
    /// Keeps track of current time; gives x position of bird
    uint64_t utime;

    int32_t y_q15_16;
    int32_t dy_q15_16;
    int32_t ddy_q15_

    uint8_t pipe_spacing_px;
    uint8_t field_width_px;
    uint8_t field_height_px;

    /* random point generation */
    uint32_t lfsr;

    uint8_t gameover;
};

/**
 *
 */
typedef struct jflappy_user_input
{
    char up, down, left, right;
} jsnake_user_input_t;


/***************************************************************
 *                       functions                            **
 **************************************************************/
void jsnake_update_state(jsnake_game_t*, jsnake_user_input_t*, int32_t);
void jsnake_game_init(jsnake_game_t*, uint32_t, uint8_t, uint8_t);

#endif
