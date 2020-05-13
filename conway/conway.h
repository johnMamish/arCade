#include <stdint.h>

#ifndef CONWAY_H
#define CONWAY_H

/***************************************************************
 *                      defines                               **
 **************************************************************/
#define MAX_CONWAY 256

/***************************************************************
 *                      typedefs                              **
 **************************************************************/

typedef struct jconway_point
{
    uint8_t x, y;
} jconway_point_t;

/**
 * Current state of the game
 */
typedef struct jconway_game
{
    /** Pointer to field. Field should be statically allocated with size
        (((width / 8) + 1) * height).
        The field is bit-packed in the sense that each bit represents a single
        cell. This should be taken into consideration when rendering the field
        to a screen. */
    uint8_t* gamefield;

    /** A second array with size 3 * ((width / 8) + 1) needs to be
        allocated for scratchpad space. */
    uint8_t* scratchpad;
    uint8_t* toroid_toprow_scratchpad;
    uint8_t* scratchpad_current_row;
    uint8_t  prev_cell_prev_state;
    uint8_t  toroid_leftcell_state;

    /** Width and height of the field in cells. Max size is 256. */
    uint8_t width;
    uint8_t height;

    /** If this variable is set to 1, the grid is treated as a toroid.
        Otherwise, cells off the grid are treated as dead. If you want a
        buffer row of extra cells, you just need to size your grid accordingly
        and render the screen accordingly. */
    uint8_t toroid;
} jconway_game_t;


/***************************************************************
 *                       functions                            **
 **************************************************************/
/**
 * Initializes a new conway's game of life with an empty grid.
 *
 * @param[out]    gamestate  An uninitialized conway's game of life struct
 * @param[in]     gamefield  A statically allocated array of uint8_t with size
 *                           (((width / 8) + 1) * height).
 * @param[in]     scratchpad A statically allocated array of uint8_t with size
 *                           3 * ((width / 8) + 1)
 * @param[in]     width      Width of the field, in cells
 * @param[in]     height     height of the field, in cells
 * @param[in]     toroid     Set this parameter to 1 if the field should wrap
 *                           around.
 */
void jconway_game_init(jconway_game_t* gamestate, uint8_t* gamefield, uint8_t* scratchpad,
                       uint8_t width, uint8_t height, uint8_t toroid);

/**
 * Steps the given game of life struct by one step
 */
void jconway_step(jconway_game_t* gamestate);

/**
 * Forces a cell at the given xy position to the on state
 */
void jconway_set_cell(jconway_game_t* gamestate, jconway_point_t* pt);

/**
 * Forces a cell at the given xy position to the off state
 */
void jconway_clear_cell(jconway_game_t* gamestate, jconway_point_t* pt);

/**
 * Toggles a cell at the given xy position
 */
void jconway_toggle_cell(jconway_game_t* gamestate, jconway_point_t* pt);

#endif
