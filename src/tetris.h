
enum GameState
{
    MENU,
    GAME,
    GAMEOVER,
    HIGHSCORE,
};
enum GameState state;

enum Direction
{
    LEFT,
    RIGHT,
    DOWN,
    UP,
    ROTATE
};

enum Shape
{
    // X X X X
    I,

    // X
    // X X X
    J,

    //     X
    // X X X
    L,

    // X X
    // X X
    O,

    //   X X
    // X X
    S,

    //   X
    // X X X
    T,

    // X X
    //   X X
    Z,
};

typedef struct
{
    int x;
    int y;
    enum Shape shape;
    enum Shape nextShape;
    enum Direction rotation;
} Block;

Block currentBlock;
int current_score;
char initials[3];

extern const uint8_t const num_font[7 * 5 * 10];

void updateHighScore(int newScore);
void draw_score();
