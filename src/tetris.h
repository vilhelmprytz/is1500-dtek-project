
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
    UP
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
    enum Direction rotation;
} Block;

Block currentBlock;
