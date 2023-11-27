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
#define MAX_HIGH_SCORES 5 // maybe less for high score

int current_score;
int btn;

void delay(int num)
{
    int i;
    for (i = num; i > 0; i--)
        ;
}

void menu(enum GameState *state)
{
    display_string(0, "TETRIS");
    display_string(1, "");
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

void check_full_rows()
{
    // check if row is full
    int row;
    for (row = 1; row < GAME_HEIGHT + 1; row++)
    {
        // check all pixels in this row
        int pix;
        bool row_full = true;

        for (pix = 1; pix < GAME_WIDTH + 1; pix++)
        {
            if (display[pix][row] == 0)
            {
                row_full = false;
                break;
            }
        }

        // if row is full, remove row
        if (row_full == true)
        {
            // remove row
            int pix;
            for (pix = 1; pix < GAME_WIDTH + 1; pix++)
            {
                display[pix][row] = 0;
            }
        }

        // if row is removed, move all rows above down
        if (row_full == true)
        {
            int row_above;
            for (row_above = row; row_above > 1; row_above--)
            {
                int pix;
                for (pix = 1; pix < GAME_WIDTH + 1; pix++)
                {
                    display[pix][row_above] = display[pix][row_above - 1];
                }
            }
        }

        // if row is full, increment score
        if (row_full == true)
        {
            current_score++;
            display[1 + current_score][120] = 1;
        }
    }
}

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
        // top, to the left
        if (display[y + BLOCK_SIZE][x] == 1)
        {
            return false;
        }

        // bottom, to the left
        if (display[y + BLOCK_SIZE][x + BLOCK_SIZE - 1] == 1)
        {
            return false;
        }
    }

    // if (direction == RIGHT)
    if (direction == RIGHT)
    {
        // top, to the right
        if (display[y - 1][x] == 1)
        {
            return false;
        }
        // bottom, to the right
        if (display[y - 1][x + BLOCK_SIZE - 1] == 1)
        {
            return false;
        }
    }

    return true;
}

void tetris_game_isr(void)
{
    // BTN2 reset
    if (((btn) & 0x00000001) == 1)
    {
        display_clear();
    }

    // try
    // BTN4 check
    if (btn >> 2 == 1)
    {
        if (check_will_be_out_of_bounds(currentBlock.y, currentBlock.y, LEFT) != false)
        {
            draw_block(currentBlock.x, currentBlock.y, 0);
            currentBlock.y = currentBlock.y + BLOCK_SIZE;
        }
    }

    // BTN3 check
    if (((btn >> 1) & 0x00000001) == 1)
    {

        if (check_will_be_out_of_bounds(currentBlock.x, currentBlock.y, RIGHT) != false)
        {
            draw_block(currentBlock.x, currentBlock.y, 0);
            currentBlock.y = currentBlock.y - BLOCK_SIZE;
        }
    }

    // tetris is 20x10 blocks
    // one block is 3x3 pixels

    // now collision
    if (check_will_be_out_of_bounds(currentBlock.x, currentBlock.y, DOWN) == false)
    {
        // check closest whole block
        // draw it
        draw_block(currentBlock.x, currentBlock.y, 1);

        // now clear rows
        check_full_rows();

        // new block
        currentBlock.x = 1;
        currentBlock.y = 1;

        // check also if new block is stuck
        if (check_will_be_out_of_bounds(currentBlock.x, currentBlock.y, DOWN) == false)
        {
            // game over
            state = GAMEOVER;
        }
    }
}

void game(enum GameState *state)
{
    // get status of buttons
    btn = getbtns();

    delay(100000);

    if (check_will_be_out_of_bounds(currentBlock.x, currentBlock.y, DOWN) != false)
    {
        draw_block(currentBlock.x, currentBlock.y, 0);
        currentBlock.x = currentBlock.x + 1;
    }

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

    draw_block(currentBlock.x, currentBlock.y, 1);

    display_change();

    draw_block(currentBlock.x, currentBlock.y, 0);
}

void gameover(enum GameState *state)
{
    display_string(0, "GAME OVER");
    display_string(1, itoaconv(current_score));
    display_string(2, "Press BTN4 for");
    display_string(3, "menu");
    display_update();
}

/*
High score structure
*/

typedef struct
{
    int score;
    // Add more fields if needed (e.g., player name, date)
} HighScore;

/*
Global high score array
*/

HighScore highScores[MAX_HIGH_SCORES];

/*
Initialize high scores
*/

void initializeHighScores()
{
    int i;
    for (i = 0; i < MAX_HIGH_SCORES; i++)
    {
        highScores[i].score = 0; // Initialize to zero or any default value
    }
}

/*
Updating high scores
*/

void updateHighScore(int newScore)
{
    int i;
    for (i = 0; i < MAX_HIGH_SCORES; i++)
    {
        if (newScore > highScores[i].score)
        {
            // Shift lower scores down
            int j;
            for (j = MAX_HIGH_SCORES - 1; j > i; j--)
            {
                highScores[j] = highScores[j - 1];
            }
            // Insert new high score
            highScores[i].score = newScore;
            break;
        }
    }
}

/*
Display high score
*/

void highscore(enum GameState *state)
{
    display_string(0, "Highscore List");
    char scoreLine[32];

    int i;
    for (i = 0; i < MAX_HIGH_SCORES; i++)
    {
        // sprintf(scoreLine, "%d. Score: %d", i + 1, highScores[i].score);
        display_string(i + 1, scoreLine); // Adjust the line number as needed
    }

    display_update();
}
