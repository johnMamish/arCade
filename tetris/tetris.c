/**
 * Contains basic game engine for a tetris game.
 *
 * API description:
 *     the only function you need to call is update_state().  You need to supply
 *     a previous state as well as user input to update_state().  update_state()
 *     will transform the previous state you give it appropriately.
 *     It is your responsibility to read the game board and render it
 *     appropriately, as drawing is implementation-defined.
 *
 * The grid is simply a 22 x 10 byte array.  a nonzero value in a byte
 * indicates that that spot is occupied.  This grid only holds pieces that have
 * already been "locked down".  The piece currently in play is not part of
 * "the grid".  This decision is made for computational efficiency and program
 * simplicity.
 *
 * The piece
 *
 *
 *
 * TODOs:
 *   x add all 7 pieces, make sure rotation is correct
 *   x generate new pieces randomly with 7 random-bag
 *   x clear field on complete row
 *   x levelling
 *   - find correct values for drop speed on different levels
 *   x insta-drop
 *   x add function stub for rng seeding
 *   x add ghost piece functionality
 *   - add insta-drop functionality
 *   - wall kicks
 *   - award points for "t-flip"
 *   - add "animation" to clearing rows
 *   - check out behaviour on official tetris.com game that seems to reset
 *     "gravity counter" when sliding a piece back and forth.
 */

#include "tetris.h"
#include <string.h>

////////////////////////////////////////////////////////////////
//                   constants                                //
////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////
//                   code                                     //
////////////////////////////////////////////////////////////////

/**
 * @brief Tells if the active piece is colliding with any other piece or with
 * the boundaries of the playing field.
 *
 * Returns nonzero for collision.
 */
static int check_collision(jtetris_game_t* gs, jtetris_piece_t* p)
{
     int failureflag = 0;

    //iterate over all of the "occupancy list" and check each cell.
    int rot_idx = p->rot_idx;
    int px = p->x;
    int py = p->y;
    int i;
    for(i = 0; i < 4; i++)
    {
        int occ_x = (int)((signed char)p->shape->occupancy_list[rot_idx][i].x) + px;
        int occ_y = (int)((signed char)p->shape->occupancy_list[rot_idx][i].y) + py;
        //check for obvious "out-of-bounds" overlap
        if((occ_x >= GAMESPACE_WIDTH) || (occ_x < 0))
            failureflag = 1;
        if(occ_y == -1)
            failureflag = 1;

        //check and see if that square in the field is already occupied.
        if(gs->active_board[occ_x + (occ_y * GAMESPACE_WIDTH)] != 0)
            failureflag = 1;
    }

    return failureflag;
}

/**
 * @brief Tries to shift the active piece by the given dx and dy.
 *
 * If the piece cannot be moved because moving it would result in a collsion
 * with the field boundaries or with other pieces, then the piece is not moved
 * and shift_if_able() returns a nonzero value.  Otherwise, shift_if_able()
 * returns zero.
 *
 * @param[in,out] gs        A pointer to the current tetris game.  The active
 *                piece of gs will be moved in the direction of dx and dy if
 *                possible.
 * @param[in]     dx        Distance to move active piece left (negative) or
 *                right (positive).
 * @param[in]     dy        Distance to move active piece down (negative)
 *
 * @return        returns zero if the piece was moved successfully
 *                returns nonzero if a collision occurred preventing the piece
 *                from moving.
 */
static int shift_if_able(jtetris_game_t* gs, int dx, int dy)
{
    //adjust position of active piece and check overlap
    gs->active_piece.x += dx;
    gs->active_piece.y += dy;
    int overlap = check_collision(gs, &gs->active_piece);

    //reset the piece to its previous position if we overlapped
    if(overlap)
    {
        gs->active_piece.x -= dx;
        gs->active_piece.y -= dy;
    }

    return overlap;
}

/**
 * @brief Similar functionality to shift_if_able(), but with rotation instead.
 *
 */
static int rotate_if_able(jtetris_game_t* gs)
{
    //adjust rotation index of active piece and check overlap
    if(++gs->active_piece.rot_idx == 4)
        gs->active_piece.rot_idx = 0;

    int overlap = check_collision(gs, &gs->active_piece);

    if(overlap)
    {
        if(--gs->active_piece.rot_idx == -1)
            gs->active_piece.rot_idx = 3;
    }

    return overlap;
}

/**
 * This array gives the amount of points to award for clearing rows, according
 * to the original NES tetris game.
 * If n rows are cleared, score_table[n] * (level + 1) points should be awarded.
 */
static const int score_table[] = {0, 40, 100, 300, 1200};

/**
 * @brief clears out rows and updates score and level accordingly.
 *
 * XXX could use some moderately clever optimization.
 */
static void clear_rows_update_score(jtetris_game_t* gs)
{
    int rows_cleared = 0;

    //keep it simple, iterate over all rows from bottom to top
    for(int y = 0; y < GAMESPACE_HEIGHT; y++)
    {
        int row_full = 1;
        int x;
        for(x = 0; x < GAMESPACE_WIDTH; x++)
        {
            if(gs->active_board[x + (y * GAMESPACE_WIDTH)] == 0)
            {
                row_full = 0;
                break;
            }
        }

        if(row_full)
        {
            //shift everything above the current row down.
            int i;
            memmove(gs->active_board + (y * GAMESPACE_WIDTH),
                    gs->active_board + ((y + 1) * GAMESPACE_WIDTH),
                    (GAMESPACE_HEIGHT - y - 1) * GAMESPACE_WIDTH);

            //clear the top row, just to be rigorous.
            int x2;
            for(x2 = 0; x2 < GAMESPACE_WIDTH; x2++)
            {
                gs->active_board[x2 + ((GAMESPACE_HEIGHT - 1) * GAMESPACE_WIDTH)] = 0;
            }
            y--;
            rows_cleared++;
        }
    }

    //update score.
    gs->score += score_table[rows_cleared] * (gs->level + 1);
    gs->rows += rows_cleared;
    gs->row_accum += rows_cleared;
    if(gs->row_accum >= 10)
    {
        gs->level++;
        gs->row_accum -= 10;
    }
}

/**
 * @brief finds the correct x and y positions for the "ghost piece"
 */
static void calculate_ghost_piece(jtetris_game_t* gs)
{
    //copy all info except y position
    gs->ghost_piece.shape = gs->active_piece.shape;
    gs->ghost_piece.rot_idx = gs->active_piece.rot_idx;
    gs->ghost_piece.x = gs->active_piece.x;

    //calculate y position for ghost piece
    jtetris_piece_t* gp = &gs->ghost_piece;
    for(gp->y = gs->active_piece.y; gp->y >= 0; gp->y--)
    {
        if(check_collision(gs, gp))
        {
            gp->y++;
            break;
        }
    }
    if(gp->y == -1)
        gp->y = 0;
}

/**
 * @brief "deactivates" the active piece
 *
 * In our model of the Tetris game, the playing area consists of active pieces
 * and a large area of inactive pieces.  While a lot of information is kept
 * about the active pieces, the inactive pieces are all one stupid lump at the
 * bottom of the board.
 *
 * This routine will take the active piece and put it in the big lump.
 */
static void lock_out_active_piece(jtetris_game_t* gs)
{
    int i;
    int rot_idx = gs->active_piece.rot_idx;
    int px = gs->active_piece.x;
    int py = gs->active_piece.y;
    for(i = 0; i < 4; i++)
    {
        int occ_x = gs->active_piece.shape->occupancy_list[rot_idx][i].x + px;
        int occ_y = gs->active_piece.shape->occupancy_list[rot_idx][i].y + py;

        gs->active_board[occ_x + (occ_y * GAMESPACE_WIDTH)] = 1;
    }
}

static void advance_lfsr(jtetris_game_t* gs)
{
    int bit = gs->lfsr & 1;
    gs->lfsr >>= 1;
    if(bit) {
        gs->lfsr ^= ((1U << 31) | (1 << 29) | (1 << 25) | (1 << 24));
    }
}

static void generate_7_random(jtetris_game_t* gs, int index)
{
    int deck[7] = {0, 1, 2, 3, 4, 5, 6};

    int i;
    for(i = 7; i != 0; i--)
    {
        //generate a new random number
        advance_lfsr(gs);
        int rand = gs->lfsr % i;

        //advance to the "rand-th" spot in "deck" that isn't "crossed-out"
        int j = 0;
        while(rand)
        {
            if(deck[j] != -1)
            {
                rand--;
                j++;
            }
            while(deck[j] == -1)
            {
                j++;
            }
        }
        while(deck[j] == -1)
        {
            j++;
        }

        gs->random_bag[index++] = deck[j];
        deck[j] = -1;
    }
}

/**
 * @brief updates random number generation and puts a new piece in "active
 * piece".
 *
 * If the newly generated piece collides, then this function returns a nonzero
 * value.
 */
static int generate_new_piece(jtetris_game_t* gs)
{
    //set up the new piece
    gs->active_piece.x = JTETRIS_INITIAL_X;
    gs->active_piece.y = JTETRIS_INITIAL_Y;
    gs->active_piece.shape = &shapes[gs->random_bag[gs->rand_idx++]];
    gs->active_piece.rot_idx = 0;

    //check to see if we need to "refill" the "random bag"
    if(gs->rand_idx == 7)
        generate_7_random(gs, 0);
    if(gs->rand_idx == 14)
    {
        gs->rand_idx = 0;
        generate_7_random(gs, 7);
    }

    //check to see if the new piece collides.
    return check_collision(gs, &gs->active_piece);
}


/**
 * This array gives the number of microseconds to wait between piece movement
 * for different levels.
 */
static const int gravity_levels[] = {500000, 400000, 300000, 166000, 125000, 100000};

/**
 * @brief Updates a given gamestate appropriately given user input and dt.
 *
 * @param[in,out] gs        A pointer to a valid jtetris_gamestate_t.  Once this
 *                function runs, state will be updated appropriately.
 * @param[in]     uinput    A pointer to a valid jtetris_user_input_t struct.
 *                See the declaration for jtetris_user_input_t for more
 *                information about how to fill the fields, as it may be
 *                implementation defined.
 * @param[in]     dt_usec   The number of microseconds since
 *                jtetris_update_state() was last called.
 */
void jtetris_update_state(jtetris_game_t* gs,
                          jtetris_user_input_t* uinput,
                          int dt_usec)
{
    //Corner case: if both left and right are pressed, they should cancel out.
    if(uinput->left && uinput->right)
        uinput->left = (uinput->right = 0);

    //Try to apply the user's input for left, right, and rotate.
    if(uinput->insta_drop)
    {
        uinput->left = uinput->right = uinput->rot = 0;
        memcpy(&gs->active_piece, &gs->ghost_piece, sizeof(gs->ghost_piece));
    }

    if(uinput->left)
        shift_if_able(gs, -1, 0);

    if(uinput->right)
        shift_if_able(gs, 1, 0);

    if(uinput->rot)
        rotate_if_able(gs);

    //apply gravity.  If the piece overlaps after the application of gravity,
    //"lock" the previous position, check for row clearance, and generate a
    //new piece.
    gs->gravity_count += dt_usec;
    int gravtime = gravity_levels[sizeof(gravity_levels) / (sizeof(gravity_levels[0]))];
    if(gs->level < (sizeof(gravity_levels) / (sizeof(gravity_levels[0]))))
        gravtime = gravity_levels[gs->level];

    while(gs->gravity_count > gravtime)
    {
        gs->gravity_count -= gravtime;
        if(shift_if_able(gs, 0, -1) != 0)
        {
            lock_out_active_piece(gs);
            clear_rows_update_score(gs);
            gs->gameover = generate_new_piece(gs);
        }
    }

    //XXX todo here: calculate ghost piece
    calculate_ghost_piece(gs);
}

void jtetris_game_init(jtetris_game_t* gs, unsigned int rng_seed)
{
    //empty active board
    for(int i = 0; i < sizeof(gs->active_board); i++)
        gs->active_board[i] = 0;

    gs->gravity_count = 0;
    gs->rows = 0;
    gs->row_accum = 0;
    gs->level = 0;
    gs->score = 0;
    gs->gameover = 0;

    //init rng
    gs->lfsr = rng_seed;

    //fill up random piece "bag"
    generate_7_random(gs, 0);
    generate_7_random(gs, 7);
    gs->rand_idx = 0;

    //XXX todo generate new active piece properly

    generate_new_piece(gs);
}
