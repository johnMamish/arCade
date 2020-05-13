/**
 * Code quality in this one is toxic sludge level
 */

#include <stdint.h>
#include <string.h>
#include "conway.h"

static uint8_t* get_row_ptr(const jconway_game_t* gs, int row)
{
    return gs->gamefield + (row * ((gs->width / 8) + 1));
}

static uint8_t get_cell_status(const jconway_game_t* gs, jconway_point_t pt)
{
    const uint8_t bitmask = (((uint8_t)1) << (pt.x % 8));
    const uint8_t* ptr = gs->gamefield + (pt.y * ((gs->width / 8) + 1)) + (pt.x / 8);
    return (*ptr & bitmask) ? (1) : (0);
}


static uint8_t get_cell_status_in_row_with_wraparound(const jconway_game_t* gs,
                                                      uint8_t* row,
                                                      uint8_t x)
{
    if (x == -1) {
        if (gs->toroid) {
            const uint8_t x = x + gs->width;
            const uint8_t bitmask = (((uint8_t)1) << (x % 8));
            const uint8_t* ptr = row + (x / 8);
            return (*ptr & bitmask) ? (1) : (0);
        } else {
            return 0;
        }
    } else if (x == gs->width) {
        if (gs->toroid) {
            return (*row & 0x01) ? (1) : (0);
        } else {
            return 0;
        }
    } else {
        const uint8_t bitmask = (((uint8_t)1) << (x % 8));
        const uint8_t* ptr = row + (x / 8);
        return (*ptr & bitmask) ? (1) : (0);
    }
}

/**
 * I resisted the temptation to use some unreadable bithacking in this...
 * premature optimization is bad, as they say.
 */
static int count_neighbors(const jconway_game_t* gs, jconway_point_t pt)
{
    int count = 0;

    /* above row */
    int d;
    for (d = -1; d <= 1; d++) {
        if (get_cell_status_in_row_with_wraparound(gs, gs->scratchpad, pt.x + d)) {
            count++;
        }
    }

    /* same row */
    /* prev cell */
    count += gs->prev_cell_prev_state;

    /* following cell */
    jconway_point_t following = { pt.x + 1, pt.y };
    if (following.x == gs->width) {
        if (gs->toroid) {
            following.x = 0;
            count += get_cell_status(gs, following);
        } else {
            count += 0;
        }
    }

    /* below row */
    if ((pt.y + 1) == gs->height) {
        for (d = -1; d <= 1; d++) {
            if (get_cell_status_in_row_with_wraparound(gs, gs->toroid_toprow_scratchpad, pt.x + d)) {
                count++;
            }
        }
    } else {
        for (d = -1; d <= 1; d++) {
            if (get_cell_status_in_row_with_wraparound(gs, get_row_ptr(gs, pt.y + 1), pt.x + d)) {
                count++;
            }
        }
    }

    return count;
}

void jconway_game_init(jconway_game_t* gs, uint8_t* gamefield, uint8_t* scratchpad,
                       uint8_t width, uint8_t height, uint8_t toroid)
{

    gs->gamefield = gamefield;
    const int field_size_bytes = (((width / 8) + 1) * height);
    memset(gs->gamefield, 0, field_size_bytes);

    gs->scratchpad = scratchpad;
    gs->toroid_toprow_scratchpad = scratchpad + ((width / 8) + 1);
    gs->scratchpad_current_row = scratchpad + 2 * ((width / 8) + 1);
    memset(gs->scratchpad, 0, 3 * ((width / 8) + 1));

    gs->prev_cell_prev_state = 0;
    gs->toroid_leftcell_state = 0;

    gs->width = width;
    gs->height = height;

    gs->toroid = toroid;
}

void jconway_step(jconway_game_t* gs)
{
    /* first row is special case depending on toroid */
    if (gs->toroid) {
        memcpy(gs->scratchpad, get_row_ptr(gs, gs->height - 1), (gs->width / 8) + 1);
        memcpy(gs->toroid_toprow_scratchpad, get_row_ptr(gs, 0), (gs->width / 8) + 1);
    } else {
        memset(gs->scratchpad, 0, ((gs->width / 8) + 1));
        memset(gs->toroid_toprow_scratchpad, 0, ((gs->width / 8) + 1));
    }

    int y;
    for (y = 0; y < (gs->height); y++) {
        /* initialize so that cell at x = -1 is handled right. */
        jconway_point_t pt;
        if (gs->toroid) {
            pt.x = gs->width - 1; pt.y = y;
            gs->prev_cell_prev_state = get_cell_status(gs, pt);
            pt.x = 0;
            gs->toroid_leftcell_state = get_cell_status(gs, pt);
        } else {
            gs->prev_cell_prev_state = 0;
            gs->toroid_leftcell_state = 0;
        }

        /* save current state of row */
        memcpy(gs->scratchpad_current_row, get_row_ptr(gs, y), ((gs->width / 8) + 1));

        uint8_t cell_bitmask = 0x01;
        pt.x = 0; pt.y = y;
        for (pt.x = 0; pt.x < (gs->width); pt.x++) {
            uint8_t cell_status = get_cell_status(gs, pt);
            uint8_t new_cell_status;
            int count = count_neighbors(gs, pt);

            if (cell_status) {
                new_cell_status = ((count == 2) || (count == 3)) ? (1) : (0);
            } else {
                new_cell_status = (count == 3) ? (1) : (0);
            }

            gs->prev_cell_prev_state = get_cell_status(gs, pt);

            if (new_cell_status) {
                *(get_row_ptr(gs, y) + (pt.x / 8)) |= cell_bitmask;
            }

            cell_bitmask = (cell_bitmask == 0x80) ? (0x01) : (cell_bitmask << 1);
        }

        /* copy saved state of current row to scratchpad */
        memcpy(gs->scratchpad, gs->scratchpad_current_row, ((gs->width / 8) + 1));
    }
}


void jconway_set_cell(jconway_game_t* gs, jconway_point_t* pt)
{
    const uint8_t bitmask = (((uint8_t)1) << (pt->x % 8));
    *(get_row_ptr(gs, pt->y) + (pt->x / 8)) |= bitmask;
}

void jconway_clear_cell(jconway_game_t* gs, jconway_point_t* pt)
{
    const uint8_t bitmask = ~(((uint8_t)1) << (pt->x % 8));
    *(get_row_ptr(gs, pt->y) + (pt->x / 8)) &= bitmask;
}

void jconway_toggle_cell(jconway_game_t* gs, jconway_point_t* pt)
{
    const uint8_t bitmask = (((uint8_t)1) << (pt->x % 8));
    *(get_row_ptr(gs, pt->y) + (pt->x / 8)) ^= bitmask;
}
