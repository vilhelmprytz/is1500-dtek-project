/*
Define block structures and game board
*/
#define BOARD_WIDTH ...
#define BOARD_HEIGHT ...

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
    Shape shape;
} Block;
