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
    // just loop over and over again
    uint8_t pixels[512];
    int i;
    for (i = 0; i < 512; i++)
    {
        int j;
        for (j = 0; j < 8; j++)
            pixels[i] = j;
        delay(30);
    }

    display_test(*pixels);
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

typedef struct
{
    // Position of the block on the board
    int x, y;
    Block currentBlock;
} FallingBlock;

FallingBlock fallingBlock;

/*
When a block is generated, initialize it at the top of the board
*/

void generateBlock()
{
    // code logic from generating block ...
    fallingBlock.currentBlock = nextBlock;
    fallingBlock.x = BOARD_WIDTH... fallingBlock.y = 0; // start at the top of the board
}

/*
Initiate the falling
*/
void moveBlockAut
