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

#include "./snake.h"

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

void draw_snake(jsnake_game_t* gs)
{
    int32_t i;
    uint32_t ui;

    /* clear */
    printf("\033[2J");

    /* =======draw walls======= */
    /* left */
    printf("\033[1;1H");
    for(i = 0; i < (gs->field_height + 3); i++)
        printf("|\033[B\033[D");

    /* right */
    printf("\033[1;%iH", gs->field_width + 2);
    for(i = 0; i < (gs->field_height + 3); i++)
        printf("|\033[B\033[D");

    /* floor */
    printf("\033[%i;2H", gs->field_height + 3);
    for(i = 0; i < (gs->field_width); i++)
        printf("-");

    /* ceiling */
    printf("\033[1;2H");
    for(i = 0; i < (gs->field_width); i++)
        printf("-");

    /* ======= draw snake ======= */
    ui = gs->tailidx;
    do
    {
        printf("\033[%i;%iH", (gs->field_height - gs->snake[ui].y) + 2,
               gs->snake[ui].x + 2);
        printf("o");

        ui++;
        if(ui >= MAX_SNAKE)
            ui = 0;
    } while(ui != gs->headidx);
    printf("\033[%i;%iH", (gs->field_height - gs->snake[ui].y) + 2,
           gs->snake[ui].x + 2);
    printf("o");

    /* ======= draw apple ======= */
    printf("\033[%i;%iH", (gs->field_height - gs->targety) + 2,
           gs->targetx + 2);
    printf("x");

    /* flush stdout */
    printf("\n");
}

int main(int argc, char** argv)
{
    /* echo off, blah blah */
    set_input_mode();

    /* state variables */
    /* int x = 0; int y = 0; int up, down, left, right; */
    int up, down, left, right;
    jsnake_game_t game;
    jsnake_user_input_t ui;
    jsnake_game_init(&game, 0x31c3f32e, 20, 20);

    while(!game.gameover)
    {
        /* gather user input */
        up = down = left = right = 0;
        while(kbhit())
        {
            const char kbpfx[4] = "\033[";
            int key = 1;
            int i;
            for(i = 0; (i < 2) && (1 || kbhit()); i++)
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
        ui.up = up;
        ui.left = left;
        ui.right = right;
        ui.down = down;

        /* move stuff around */
        jsnake_update_state(&game, &ui, 55000);

        /* draw */
        draw_snake(&game);

        struct timespec req = {0, 55000000};
        nanosleep(&req, NULL);
    }

    printf("\033[2J");
    printf("\033[1;1H");
    printf("GAME OVER\n\n\ncontrol+c to quit\n");

    while(1)
    {
        struct timespec req = {0, 10000000};
        nanosleep(&req, NULL);
    }
}
