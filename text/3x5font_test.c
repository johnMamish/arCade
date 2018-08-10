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

#include "./3x5font.h"

#define WIDTH           10
#define HEIGHT          21

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

void draw_buf(uint8_t* buf)
{
    int i;

    /* clear */
    printf("\033[2J");

    /* =======draw walls======= */
    /* top */
    printf("\033[1;1H");
    printf("|");
    for(i = 0; i < (WIDTH); i++)
        printf("-");
    printf("|");

    /* body */
    int x, y;
    for(y = 0; y < HEIGHT; y++)
    {
        printf("\033[%i;1H|", y + 2);
        for(x = 0; x < WIDTH; x++)
        {
            if(buf[(y * WIDTH) + x])
                printf("o");
            else
                printf(" ");
        }
        printf("|");
    }

    /* bottom */
    printf("\033[%i;1H", HEIGHT + 1);
    printf("|");
    for(i = 0; i < (WIDTH); i++)
        printf("-");
    printf("|\n");
}

int main(int argc, char** argv)
{
    uint8_t *buf = calloc(WIDTH * HEIGHT, sizeof(uint8_t));

    //const char* str = "HIGH SCORE:";
    text_scroller_t ts;
    text_scroller_init(&ts, 0, 10, 21, 100000, "HIGH SCORE:");

    while(1)
    {
        memset(buf, 0, WIDTH * HEIGHT);

        text_scroller_draw(buf, &ts);
        int mid = 5 - (text_get_width("54") / 2);
        text_draw_puts(buf, WIDTH, HEIGHT, "54", mid, 10);
        /* draw */
        draw_buf(buf);

        struct timespec req = {0, 10000000};
        nanosleep(&req, NULL);

        text_scroller_advance(&ts, 10000);
    }

    return 0;
}
