/**
 *
 */


////////////////////////////////////////////////////////////////
//                      defines                               //
////////////////////////////////////////////////////////////////

#define GAMESPACE_WIDTH 10
#define GAMESPACE_HEIGHT 22

#define JTETRIS_INITIAL_X 4
#define JTETRIS_INITIAL_Y 21

////////////////////////////////////////////////////////////////
//                      typedefs                              //
////////////////////////////////////////////////////////////////

/**
 * This is used to describe a particular shape, for instance, a straight line,
 * an L, or a backwards L.
 * The shape is described by a list of occupied x, y coordinates.  Because each
 * shape can be rotated 4 ways, there are 4 lists for each shape.
 *
 * The occupancy list should list positions in clockwise order.  Entry n+1
 * should be a clockwise rotation of entry n.
 */
struct jtetris_shape
{
    struct {
        signed char x, y;
    } occupancy_list [4][4];
};

typedef struct jtetris_shape jtetris_shape_t;

const static jtetris_shape_t foo = { .occupancy_list = {{{1, 1}, {1, 1}, {1, 1}, {1, 1}}, {{1, 1}, {1, 1}, {1, 1}, {1, 1}}, {{1, 1}, {1, 1}, {1, 1}, {1, 1}}, {{1, 1}, {1, 1}, {1, 1}, {1, 1}}} };


const static jtetris_shape_t shapes [7] =
{
    //I-shape
    //
    //0123456789
    //   XXXX
    //
    // -1012
    //0 ****      **X*      ****      *X**
    //-1XXXX      **X*      ****      *X**
    //-2****      **X*      XXXX      *X**
    //-3****      **X*      ****      *X**
    {
        {{{-1, -1}, {0, -1}, {1, -1}, {2, -1}},
         {{1, 0}, {1, -1}, {1, -2}, {1, -3}},
         {{-1, -2}, {0, -2}, {1, -2}, {2, -2}},
         {{0, 0}, {0, -1}, {0, -2}, {0, -3}}}
    },

    //Reverse-L
    //
    //0123456789
    //   X
    //   XXX
    //
    // -1012
    //0 X***      *XX*      ****      *X**
    //-1XXX*      *X**      XXX*      *X**
    //-2****      *X**      **X*      XX**
    //-3****      ****      ****      ****
    {
        {{{-1, 0}, {-1, -1}, {0, -1}, {1, -1}},
         {{0, 0}, {1, 0}, {0, -1}, {0, -2}},
         {{-1, -1}, {0, -1}, {1, -1}, {1, -2}},
         {{0, 0}, {0, -1}, {-1, -2}, {0, -2}}}
    },

    //L-shape
    //
    //0123456789
    //     X
    //   XXX
    //
    // -1012
    //0 **X*      *X**      ****      XX**
    //-1XXX*      *X**      XXX*      *X**
    //-2****      *XX*      X***      *X**
    //-3****      ****      ****      ****
    {
        {{{1, 0}, {-1, -1}, {0, -1}, {1, -1}},
         {{0, 0}, {0, -1}, {0, -2}, {1, -2}},
         {{-1, -1}, {0, -1}, {1, -1}, {-1, -2}},
         {{-1, 0}, {0, 0}, {0, -1}, {0, -2}}}
    },

    //square
    //
    //0123456789
    //    XX
    //    XX
    //
    // *XX*
    // *XX*
    // ****
    {
        .occupancy_list =
        {{{0, 0}, {1, 0}, {0, -1}, {1, -1}},
         {{0, 0}, {1, 0}, {0, -1}, {1, -1}},
         {{0, 0}, {1, 0}, {0, -1}, {1, -1}},
         {{0, 0}, {1, 0}, {0, -1}, {1, -1}}}
    },

    //S-shape
    //
    //0123456789
    //    XX
    //   XX
    //
    //  -1012
    //0  *XX*      *X**      ****      X***
    //-1 XX**      *XX*      *XX*      XX**
    //-2 ****      **X*      XX**      *X**
    //-3 ****      ****      ****      ****
    {
        {{{0, 0}, {1, 0}, {-1, -1}, {0, -1}},
         {{0, 0}, {0, -1}, {1, -1}, {1, -2}},
         {{0, -1}, {1, -1}, {-1, -2}, {0, -2}},
         {{-1, 0}, {-1, -1}, {0, -1}, {0, -2}}}
    },


    //T-shape
    // -101
    // 0*X*       *X*       ***         *X*
    //-1XXX       *XX       XXX         XX*
    //-2***       *X*       *X*         *X*
    {
        {{{0, 0}, {-1, -1}, {0, -1}, {1, -1}},
        {{0, 0}, {0, -1}, {1, -1}, {0, -2}},
        {{-1, -1}, {0, -1}, {1, -1}, {0, -2}},
        {{0, 0}, {-1, -1}, {0, -1}, {0, -2}}}
    },

    //Z-shape
    //
    //0123456789
    //   XX
    //    XX
    //
    //  -1012
    //0  XX**      **X*      ****      *X**
    //-1 *XX*      *XX*      XX**      XX**
    //-2 ****      *X**      *XX*      X***
    //-3 ****      ****      ****      ****
    {
        {{{-1, 0}, {0, 0}, {0, -1}, {1, -1}},
         {{1, 0}, {0, -1}, {1, -1}, {0, -2}},
         {{-1, -1}, {0, -1}, {0, -2}, {1, -2}},
         {{0, 0}, {-1, -1}, {0, -1}, {-1, -2}}}
    },
};

/**
 * This structure groups together user input that the state update should be
 * interested in.  I thought about making this a char with bitfields, but what's
 * the point?  It's a little more readable this way.
 *
 * The rot, left, and right buttons should be edge-triggered, but down should
 * "directly" reflect the state of the button.
 */
struct jtetris_user_input
{
    char rot, left, right, down, insta_drop;
};

typedef struct jtetris_user_input jtetris_user_input_t;

/**
 *
 */
struct jtetris_piece
{
    int x, y;
    const jtetris_shape_t* shape;
    int rot_idx;
};

typedef struct jtetris_piece jtetris_piece_t;


/**
 *
 */
struct jtetris_game
{
    ///An x, y position on the board is indexed by (x + (y * 10)).  Higher x
    ///positions are further right, and higher y positions are further up.
    char active_board[22*10];
    jtetris_piece_t active_piece;
    jtetris_piece_t ghost_piece;

    int gravity_count;

    ///level starts at 0.
    int level;
    int row_accum;
    int rows;
    int score;
    int gameover;

    //random piece generation
    unsigned int lfsr;          //XXX Make sure to seed.
    int random_bag[14];
    int rand_idx;
};

typedef struct jtetris_game jtetris_game_t;


////////////////////////////////////////////////////////////////
//                      functions                             //
////////////////////////////////////////////////////////////////
void jtetris_update_state(jtetris_game_t*, jtetris_user_input_t*, int);
void jtetris_game_init(jtetris_game_t*, unsigned int);
char jtetris_get_cell(jtetris_game_t*, int, int);
