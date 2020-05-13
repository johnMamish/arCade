/**
 * TODOs:
 *
 */

#define _POSIX_C_SOURCE       199309L

#include <stdint.h>
#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>

#include "./conway.h"

#define UP              'A'
#define DOWN            'B'
#define RIGHT           'C'
#define LEFT            'D'

#define SPACE          ' '

#define NEXT           'n'

#define WIDTH           35
#define HEIGHT          35

struct termios saved_attributes;

void reset_input_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode(void)
{
    struct termios tattr;

    /* Save the terminal attributes so we can restore them later. */
    tcgetattr(STDIN_FILENO, &saved_attributes);
    atexit(reset_input_mode);

    /* Set the funny terminal modes. */
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
    FD_SET(STDIN_FILENO, &fds); /* STDIN_FILENO is 0 */
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void draw_conway(jconway_game_t* gs, jconway_point_t* cursor)
{
    /* clear */
    printf("\033[2J");

    printf("arrow keys to move cursor, space to toggle cell, \'n\' to advance sim\n\n\n");

    int x, y;
    for (y = 0; y < gs->height; y++) {
        uint8_t bitmask = 0x01;
        for (x = 0; x < gs->width; x++) {
            uint8_t cell_state = *(gs->gamefield +
                                   (((gs->width / 8) + 1) * y) +
                                   (x / 8));

            if ((x == cursor->x) && (y == cursor->y)) {
                if (cell_state & bitmask) {
                    printf("\033[0;31m" "█" "\033[0m");
                } else {
                    printf("\033[0;31m" "░" "\033[0m");
                }
            } else {
                if (cell_state & bitmask) {
                    printf("█");
                } else {
                    printf("░");
                }
            }

            bitmask = (bitmask == 0x80) ? (0x01) : (bitmask << 1);
        }
        printf("\r\n");
    }

    /* flush stdout */
    printf("\n");
}

int main(int argc, char** argv)
{
    /* echo off, blah blah */
    set_input_mode();

    /* state variables */
    /* int x = 0; int y = 0; int up, down, left, right; */
    int up, down, left, right, space, next;
    jconway_game_t game;
    jconway_point_t cursor;
    static uint8_t gamefield[((WIDTH / 8) + 1) * HEIGHT];
    static uint8_t scratchpad[((WIDTH / 8) + 1) * 3];
    jconway_game_init(&game, gamefield, scratchpad, WIDTH, HEIGHT, 1);

    while(1)
    {
        /* gather user input */
        up = down = left = right = space = next = 0;
        while(kbhit())
        {
            const char kbpfx[4] = "\033[";
            int key = 1;
            int i;
            int c;
            for(i = 0; (i < 2) && (1 || kbhit()); i++)
            {
                c = fgetc(stdin);
                if(kbpfx[i] != c)
                {
                    key = 0;
                    break;
                }
            }

            if(key && (1 || kbhit())) {
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
            } else {
                switch(c) {
                    case NEXT:
                        next = 1;
                        break;
                    case SPACE:
                        space = 1;
                        break;
                }
            }
        }

        /* move stuff around */
        if (up) {
            cursor.y = (cursor.y == 0) ? (HEIGHT - 1) : cursor.y - 1;
        }
        if (down) {
            cursor.y = (cursor.y == (HEIGHT - 1)) ? (0) : cursor.y + 1;
        }
        if (left) {
            cursor.x = (cursor.x == 0) ? (WIDTH - 1) : cursor.x - 1;
        }
        if (right) {
            cursor.x = (cursor.x == (WIDTH - 1)) ? (0) : cursor.x + 1;
        }

        if (space) {
            jconway_toggle_cell(&game, &cursor);
        }

        if (next) {
            jconway_step(&game);
        }

        /* draw */
        draw_conway(&game, &cursor);

        struct timespec req = {0, 10000000};
        nanosleep(&req, NULL);
    }
}
