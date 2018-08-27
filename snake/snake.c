#include <stdint.h>
#include "snake.h"

static void advance_lfsr(jsnake_game_t* gs)
{
    uint32_t bit = gs->lfsr & (uint32_t)1;
    gs->lfsr >>= 1;
    if(bit) {
        gs->lfsr ^= ((1U << 31) | (1 << 29) | (1 << 25) | (1 << 24));
    }
}

static void generate_new_target(jsnake_game_t* gs)
{
    /* do it twice for good measure */
    advance_lfsr(gs);
    advance_lfsr(gs);
    gs->targetx = gs->lfsr % gs->field_width;

    advance_lfsr(gs);
    advance_lfsr(gs);
    gs->targety = gs->lfsr % gs->field_height;
}

/**
 * Checks to see if the snake has gone out of bounds or if has bit itself.
 *
 * Sets the gameover flag appropriately
 */
static void update_gameover(jsnake_game_t* gs)
{
    /* check if out of bounds.  nb: points are unsigned, so we use 0xff */
    jsnake_point_t h = gs->snake[gs->headidx];
    if((h.x == 0xff) || (h.x >= gs->field_width) ||
       (h.y == 0xff) || (h.y >= gs->field_height))
    {
        gs->gameover = 1;
        return;
    }

    /* check if it bit itself. */
    uint32_t i;
    for(i = gs->tailidx; i != gs->headidx; i = ((i + 1) > MAX_SNAKE) ? 0 : (i + 1))
    {
        /* check bounds */
        if(i > MAX_SNAKE)
            i = 0;

        /* check collision */
        if((h.x == gs->snake[i].x) && (h.y == gs->snake[i].y))
        {
            gs->gameover = 1;
            break;
        }
    }
}

static snakedir_e ui_to_snakedir(jsnake_user_input_t *ui)
{
    if(ui->up)
        return SNAKEDIR_UP;
    if(ui->down)
        return SNAKEDIR_DOWN;
    if(ui->left)
        return SNAKEDIR_LEFT;
    if(ui->right)
        return SNAKEDIR_RIGHT;

    return SNAKEDIR_NONE;
}

static void step_snake(jsnake_game_t *gs)
{
    /* Advance snake's movement by first moving the tail forward, then */
    /* "generating" a new head */
    gs->prev_tail = gs->snake[gs->tailidx];
    gs->tailidx++;
    if(gs->tailidx >= MAX_SNAKE)
        gs->tailidx = 0;

    /* figure out where the next head position should be. */
    /* yes, there are better ways to do this, but */
    gs->new_head = gs->snake[gs->headidx];

    if(gs->snakedir_first == SNAKEDIR_NONE)
    {
        switch(gs->snakedir)
        {
            case SNAKEDIR_UP:
                gs->new_head.y++;
                break;

            case SNAKEDIR_DOWN:
                gs->new_head.y--;
                break;

            case SNAKEDIR_RIGHT:
                gs->new_head.x++;
                break;

            case SNAKEDIR_LEFT:
                gs->new_head.x--;
                break;
            default:
                break;
        }
    }
    else
    {
        switch(gs->snakedir_first)
        {
            case SNAKEDIR_UP:
                gs->new_head.y++;
                break;

            case SNAKEDIR_DOWN:
                gs->new_head.y--;
                break;

            case SNAKEDIR_RIGHT:
                gs->new_head.x++;
                break;

            case SNAKEDIR_LEFT:
                gs->new_head.x--;
                break;
            default:
                break;
        }
    }

    gs->headidx++;
    if(gs->headidx >= MAX_SNAKE)
        gs->headidx = 0;

    gs->snake[gs->headidx] = gs->new_head;
}

static void update_snakedir(jsnake_game_t *gs)
{
    if(gs->snakedir_second != SNAKEDIR_NONE)
    {
        gs->snakedir = gs->snakedir_second;
    }
    else if(gs->snakedir_first != SNAKEDIR_NONE)
    {
        gs->snakedir = gs->snakedir_first;
    }

    gs->snakedir_first = SNAKEDIR_NONE;
    gs->snakedir_second = SNAKEDIR_NONE;
}

/**
 *
 */
void jsnake_update_state(jsnake_game_t* gs, jsnake_user_input_t* ui, int32_t dt_us)
{
    if (ui_to_snakedir(ui) != SNAKEDIR_NONE) {
        if (gs->snakedir_first != SNAKEDIR_NONE)
            gs->snakedir_second = ui_to_snakedir(ui);
        else
            gs->snakedir_first = ui_to_snakedir(ui);
    }

    /* advance speed */
    /* XXX TODO won't handle moving 2 tiles in one timestep. */
    gs->move_timeout_us += dt_us;
    if(gs->move_timeout_us >= gs->us_per_step)
        gs->move_timeout_us -= gs->us_per_step;
    else
        return;

    step_snake(gs);
    update_snakedir(gs);

    /* check to see if the snake bit itself or went out of bounds */
    update_gameover(gs);

    /* check to see if the snake ate an apple */
    if((gs->new_head.x == gs->targetx) && (gs->new_head.y) == gs->targety)
    {
        /* =======generate new apple======= */
        generate_new_target(gs);

        /* =======grow snake tail======= */
        gs->snakelen++;
        if(gs->snakelen >= MAX_SNAKE)
            while(1);

        /* note the special case where the apple lies right where the snake's
           tail is when it is eaten.
           (x is head, o's are bodies)
           ooo
           o o
           o o
           xoo

           The tail will grow out and touch the head, but this does not kill the
           snake because gameover is not re-evaluated until the snake has moved
           again */
        if(gs->tailidx == 0)
            gs->tailidx = MAX_SNAKE - 1;
        else
            gs->tailidx--;
    }
}

void jsnake_game_init(jsnake_game_t* gs, uint32_t lfsr_seed, uint32_t us_per_step,
                      uint8_t width, uint8_t height)
{
    /* initialize basic state variables */
    gs->move_timeout_us = 0;
    gs->us_per_step = us_per_step;

    /* initialize snake struct */
    gs->headidx = 1;
    gs->tailidx = 0;
    gs->snake[1] = (jsnake_point_t){(width >> 1), (height >> 1)};
    gs->snake[0] = (jsnake_point_t){(width >> 1), (height >> 1) - 1};
    gs->snakelen = 2;
    gs->snakedir = SNAKEDIR_UP;

    /* we just assume that these are a sane number, say, 20 x 10. */
    gs->field_width = width;
    gs->field_height = height;

    gs->lfsr = lfsr_seed;
    gs->gameover = 0;

    /* generate first apple */
    generate_new_target(gs);
}
