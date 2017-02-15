#include "snake.h"

static void advance_lfsr(jsnake_game_t* gs)
{
    int bit = gs->lfsr & 1;
    gs->lfsr >>= 1;
    if(bit) {
        gs->lfsr ^= ((1U << 31) | (1 << 29) | (1 << 25) | (1 << 24));
    }
}

static void generate_new_target(jsnake_game_t* gs)
{
    //do it twice for good measure
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
    //check if out of bounds.  nb: points are unsigned, so we use 0xff
    jsnake_point_t h = gs->snake[gs->headidx];
    if((h.x == 0xff) || (h.x >= gs->field_width) ||
       (h.y == 0xff) || (h.y >= gs->field_height))
    {
        gs->gameover = 1;
        return;
    }

    //check if it bit itself.
    uint32_t i;
    for(i = gs->tailidx; i != gs->headidx; i++)
    {
        //check bounds
        if(i > MAX_SNAKE)
            i = 0;

        //check collision
        if((h.x == gs->snake[i].x) && (h.y == gs->snake[i].y))
        {
            gs->gameover = 1;
            break;
        }
    }
}

/**
 *
 */
void jsnake_update_state(jsnake_game_t* gs, jsnake_user_input_t* ui, int dt_us)
{
    //jenky.  oh well.
    if(ui->up)
        gs->snakedir = SNAKEDIR_UP;
    if(ui->down)
        gs->snakedir = SNAKEDIR_DOWN;
    if(ui->left)
        gs->snakedir = SNAKEDIR_LEFT;
    if(ui->right)
        gs->snakedir = SNAKEDIR_RIGHT;

    //Advance snake's movement by first moving the tail forward, then
    //"generating" a new head
    gs->prev_tail = gs->snake[gs->tailidx];
    gs->tailidx++;
    if(gs->tailidx >= MAX_SNAKE)
        gs->tailidx = 0;

    //figure out where the next head position should be.
    //yes, there are better ways to do this, but
    gs->new_head = gs->snake[gs->headidx];
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
    }

    gs->headidx++;
    if(gs->headidx >= MAX_SNAKE)
        gs->headidx = 0;

    gs->snake[gs->headidx] = gs->newhead;

    //check to see if the snake bit itself or went out of bounds
    update_gameover(gs);

    //check to see if the snake ate an apple
    if((gs->newhead.x == gs->targetx) && (gs->newhead.y) == gs->targety)
    {
        //=======generate new apple=======
        generate_new_target(gs);

        //=======grow snake tail=======
        gs->snakelen++;
        if(gs->snakelen >= MAX_SNAKE)
            while(1);

        //note the special case where the apple lies right where the snake's
        //tail is when it is eaten.
        // (x is head, o's are bodies)
        // ooo
        // o o
        // o o
        // xoo
        //
        //The tail will grow out and touch the head, but this does not kill the
        //snake because gameover is not re-evaluated until the snake has moved
        //again
        if(gs->tailidx == 0)
            gs->tailidx = MAX_SNAKE - 1;
        else
            gs->tailidx--;
    }
}
