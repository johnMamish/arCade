/**
 * TODOs:
 *
 */

#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>

#include "./tetris.h"

#define UP              'A'
#define DOWN            'B'
#define RIGHT           'C'
#define LEFT            'D'

#define WIDTH           24
#define HEIGHT          30

struct termios saved_attributes;

void reset_input_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode(void)
{
    struct termios tattr;
    char *name;

    //Save the terminal attributes so we can restore them later.
    tcgetattr(STDIN_FILENO, &saved_attributes);
    atexit(reset_input_mode);

    //Set the funny terminal modes.
    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO);
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void update_xy_demo(int left, int right, int up, int down, int* x, int* y, int* gravcount)
{
    //move stuff around
    if(left)
        *x = (*x == 0) ? (0) : (*x - 1);
    if(right)
        *x = (*x == WIDTH) ? (WIDTH) : (*x + 1);
    if(down)
        *y = (*y == HEIGHT) ? (HEIGHT) : (*y + 1);
    if(up)
        *y = (*y == 0) ? (0) : (*y - 1);

    if((*gravcount = *gravcount + 1) > 3)
    {
        *gravcount = 0;
        *y = (*y == HEIGHT) ? (HEIGHT) : (*y + 1);
    }
}

void draw_demo(int x, int y)
{
    printf("\033[2J");
    printf("\033[0;0H");

    printf("\033[%iB", y);
    printf("\033[%iC", x);
    printf("a");

    //left wall
    printf("\033[0;0H");
    for(int i = 0; i < HEIGHT + 1; i++)
        printf("|\033[B\033[D");

    //right wall
    printf("\033[0;%iH", WIDTH + 2);
    for(int i = 0; i < HEIGHT + 1; i++)
        printf("|\033[B\033[D");

    //floor
    printf("\033[%i;0H", HEIGHT + 2);
    for(int i = 0; i < WIDTH + 2; i++)
        printf("-");

    printf("\n");
}


void draw_tetris(jtetris_game_t* gs)
{
    //clear
    printf("\033[2J");

    //=======draw walls=======
    //left
    printf("\033[1;1H");
    for(int i = 0; i < (GAMESPACE_HEIGHT - 2 + 1); i++)
        printf("|\033[B\033[D");

    //right
    printf("\033[1;%iH", GAMESPACE_WIDTH + 2);
    for(int i = 0; i < (GAMESPACE_HEIGHT - 2 + 1); i++)
        printf("|\033[B\033[D");

    //floor
    printf("\033[%i;2H", GAMESPACE_HEIGHT - 2 + 1);
    for(int i = 0; i < GAMESPACE_WIDTH; i++)
        printf("-");

    //=======draw passive pieces=======
    int ix = 0;
    for(int y = GAMESPACE_HEIGHT - 2; y > 0; y--)
    {
        printf("\033[%i;2H", y);
        for(int x = 0; x < GAMESPACE_WIDTH; x++, ix++)
        {
            if(gs->active_board[ix])
                printf("x");
            else
                printf(" ");
        }
    }

    //=======draw ghost piece=======
    int rot_idx = gs->ghost_piece.rot_idx;
    int px = gs->ghost_piece.x;
    int py = gs->ghost_piece.y;
    int i;
    for(i = 0; i < 4; i++)
    {
        int occ_x = gs->ghost_piece.shape->occupancy_list[rot_idx][i].x + px;
        int occ_y = gs->ghost_piece.shape->occupancy_list[rot_idx][i].y + py;

        int print_x = occ_x + 2;
        int print_y = (GAMESPACE_HEIGHT - occ_y) - 2;
        if((print_x > 0) && (print_y > 0))
        {
            printf("\033[%i;%iH", print_y, print_x);
            printf("o");
        }
    }

    rot_idx = gs->active_piece.rot_idx;
    px = gs->active_piece.x;
    py = gs->active_piece.y;
    //======= active piece=======
    for(i = 0; i < 4; i++)
    {
        int occ_x = gs->active_piece.shape->occupancy_list[rot_idx][i].x + px;
        int occ_y = gs->active_piece.shape->occupancy_list[rot_idx][i].y + py;

        int print_x = occ_x + 2;
        int print_y = (GAMESPACE_HEIGHT - occ_y) - 2;
        if((print_x > 0) && (print_y > 0))
        {
            printf("\033[%i;%iH", print_y, print_x);
            printf("x");
        }
    }

    //=======Draw score and such=======
    printf("\033[5;13HLevel: %i", gs->level);
    printf("\033[6;13HRows: %i", gs->rows);
    printf("\033[7;13HScore: %i", gs->score);

    //flush stdout
    printf("\n");
}


//test to make sure that non-consecutive rows are properly cleared.
const char test_game_0[] =
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int main(int argc, char** argv)
{
    //echo off, blah blah
    set_input_mode();

    //state variables
    //int x = 0; int y = 0; int up, down, left, right;
    int up, down, left, right;
    jtetris_game_t game;
    jtetris_user_input_t ui;
    jtetris_game_init(&game, 0x31c3f32e);
    //memcpy(game.active_board, test_game_0, sizeof(test_game_0));

    int gravcount = 0;
    while(!game.gameover)
    {
        //gather user input
        up = down = left = right = 0;
        while(kbhit())
        {
            const char kbpfx[4] = "\033[";
            int key = 1;
            for(int i = 0; (i < 2) && (1 || kbhit()); i++)
            {
                int c = fgetc(stdin);
                if(kbpfx[i] != c)
                {
                    key = 0;
                    break;
                }
            }

            if(key && (1 || kbhit()))
                key = fgetc(stdin);

            switch(key)
            {
                case UP:
                    up = 1;
                    break;
                case DOWN:
                    down = 1;
                    break;
                case LEFT:
                    left = 1;
                    break;
                case RIGHT:
                    right = 1;
                    break;
            }
        }
        ui.rot = up;
        ui.left = left;
        ui.right = right;
        ui.down = 0;
        ui.insta_drop = down;

        //move stuff around
        //update_xy_demo(left, right, up, down, &x, &y, &gravcount);
        jtetris_update_state(&game, &ui, 55000);

        //draw
        //draw_demo(x, y);
        draw_tetris(&game);

        usleep(25000);
    }

    printf("\033[2J");
    printf("\033[1;1H");
    printf("GAME OVER\n\n\ncontrol+c to quit\n");

    while(1)
    {
        usleep(10000);
    }
}
