/* tetris.c

    (c) 2023
    Vilhelm Prytz
    Filip Dimitrijevic
*/

#include <stdint.h>
#include "display.h"
#include "buttons.h"
#include "tetris.h"

void delay(int num)
{
    int i;
    for (i = num; i > 0; i--)
        ;
}

void menu(enum GameState *state)
{
    // get status of buttons
    int btn = getbtns();

    // BTN4 check
    if (btn >> 2 == 1)
    {
        *state = GAME;
    }

    // BTN3 check
    if (((btn >> 1) & 0x00000001) == 1)
    {
        *state = HIGHSCORE;
    }

    display_string(0, "TETRIS");
    display_string(2, "BTN3 - Highscores");
    display_string(3, "BTN4 - Start");
    display_update();
}

void game(enum GameState *state)
{
    display_clear();

    // set stuff
    int col;
    for (col = 0; col < 128; col++)
    {
        display[0][col] = 1;
        display[31][col] = 1;
    }

    int row;
    for (row = 1; row < 31; row++)
    {
        display[row][0] = 1;
        display[row][127] = 1;
    }

    // try
    display[4][4] = 1;
    display[4][5] = 1;
    display[5][4] = 1;
    display[5][5] = 1;

    display_change();
}

void gameover(enum GameState *state)
{
    display_string(0, "GAME OVER");
    display_update();
}

void highscore(enum GameState *state)
{
    display_string(0, "Highscore List");
    display_string(2, "");
    display_string(3, "");
    display_update();
}

/*
Implementing falling or movement of blocks
*/

// typedef struct
// {
//     // Position of the block on the board
//     int x, y;
//     Block currentBlock;
// } FallingBlock;

// FallingBlock fallingBlock;

// /*
// When a block is generated, initialize it at the top of the board
// */

// void generateBlock()
// {
//     // code logic from generating block ...
//     fallingBlock.currentBlock = nextBlock;
//     fallingBlock.x = BOARD_WIDTH... fallingBlock.y = 0; // start at the top of the board
// }

// /*
// Initiate the falling
// */
// void moveBlockAut
