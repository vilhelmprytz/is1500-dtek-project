/* tetris.c

    (c) 2023
    Vilhelm Prytz
    Filip Dimitrijevic
*/

#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "buttons.h"
#include "tetris.h"

#define GAME_HEIGHT 100
#define GAME_WIDTH 30
#define BLOCK_SIZE 5

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

void draw_block(int x, int y, int oledstate)
{
    int loop;
    for (loop = 0; loop < BLOCK_SIZE; loop++)
    {
        int innerloop;
        for (innerloop = 0; innerloop < BLOCK_SIZE; innerloop++)
        {
            display[y + loop][x + innerloop] = oledstate;
        }
    }
}

// 2d array to keep track of blocks
int x, y;
x = 1;
y = 4;

bool check_will_be_out_of_bounds(int x, int y, enum Direction direction)
{
    // if (direction == DOWN)
    if (direction == DOWN)
    {
        // directly under
        if (display[y][x + BLOCK_SIZE] == 1)
        {
            return false;
        }
        // to the left
        if (display[y + BLOCK_SIZE - 1][x + BLOCK_SIZE] == 1)
        {
            return false;
        }
    }

    // if (direction == LEFT)
    if (direction == LEFT)
    {
        if (display[y + BLOCK_SIZE][x + BLOCK_SIZE] == 1)
        {
            return false;
        }
    }

    // if (direction == RIGHT)
    if (direction == RIGHT)
    {
        if (display[y - 1][x + BLOCK_SIZE] == 1)
        {
            return false;
        }
    }

    return true;
}

void game(enum GameState *state)
{
    // display_clear();
    delay(100000);

    // get status of buttons
    int btn = getbtns();

    // draw boundaries
    int col;
    for (col = 0; col < GAME_HEIGHT + 2; col++)
    {
        display[0][col] = 1;
        display[GAME_WIDTH + 1][col] = 1;
    }

    int row;
    for (row = 1; row < GAME_WIDTH + 1; row++)
    {
        display[row][0] = 1;
        display[row][GAME_HEIGHT + 1] = 1;
    }

    // tetris is 20x10 blocks
    // one block is 3x3 pixels
    if (check_will_be_out_of_bounds(x, y, DOWN) == false)
    {
        // draw it
        draw_block(x, y, 1);

        // new block
        x = 1;
        y = 4;
    }
    if (check_will_be_out_of_bounds(x, y, DOWN) != false)
    {
        x = x + 1;
    }

    // try
    // BTN4 check
    if (btn >> 2 == 1)
    {
        if (check_will_be_out_of_bounds(x, y, LEFT) != false)
        {
            y = y + 1;
        }
    }

    // BTN3 check
    if (((btn >> 1) & 0x00000001) == 1)
    {
        if (check_will_be_out_of_bounds(x, y, RIGHT) != false)
        {
            y = y - 1;
        }
    }

    draw_block(x, y, 1);

    display_change();

    draw_block(x, y, 0);
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
