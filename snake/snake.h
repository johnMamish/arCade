#include <stdint.h>


////////////////////////////////////////////////////////////////
//                        defines                             //
////////////////////////////////////////////////////////////////
#define MAX_SNAKE 256

////////////////////////////////////////////////////////////////
//                      typedefs                              //
////////////////////////////////////////////////////////////////

typedef enum snakedir
{
    SNAKEDIR_UP, SNAKEDIR_DOWN, SNAKEDIR_LEFT, SNAKEDIR_RIGHT;
} snakedir_e;

typedef struct jsnake_point
{
    uint8_t x, y;
} jsnake_point_t;

/**
 * Current state of the game
 */
typedef struct jsnake_game
{
    ///keeps track of how long since the snake has moved
    int move_timeout_us;

    ///target x and y positions.
    uint8_t targetx, targety;

    ///not the most efficient representation, but it's easy to use
    jsnake_point_t snake[MAX_SNAKE];
    uint32_t headpos, tailpos;
    uint32_t snakelen;
    snakedir_e snakedir;

    //These can be used to speed up drawing routines.  Instead of iterating
    //through the entire snake, you can just turn off prev_tail and turn on
    //new_head.
    jsnake_point_t prev_tail, new_head;

    uint8_t field_width;
    uint8_t field_height;

    //random point generation
    unsigned int lfsr;

    uint8_t gameover;
} jsnake_game_t;

/**
 *
 */
typedef struct jsnake_user_input
{
    char up, down, left, right;
} jsnake_user_input_t;


////////////////////////////////////////////////////////////////
//                      functions                             //
////////////////////////////////////////////////////////////////
void jsnake_update_state(jsnake_game_t*, jsnake_user_input_t*, int);
void jsnake_game_init(jsnake_game_t*, unsigned int);
