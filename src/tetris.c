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

#define GAME_HEIGHT 120
#define GAME_WIDTH 30
#define BLOCK_SIZE 3
#define MAX_HIGH_SCORES 5 // maybe less for high score

int current_score;
int btn;
int intended_action;
bool is_occupied;

// intended_action
// 0 - do nothing
// 1 - move left
// 2 - move right
// 3 - rotate

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

void draw_block(int x, int y, int oledstate, bool check, bool *is_occupied)
{
    int loop;
    for (loop = 0; loop < BLOCK_SIZE; loop++)
    {
        int innerloop;
        for (innerloop = 0; innerloop < BLOCK_SIZE; innerloop++)
        {
            if (check)
            {
                if (display[y + loop][x + innerloop] == 1)
                {

                    *is_occupied = true;
                }
            }
            else
            {
                display[y + loop][x + innerloop] = oledstate;
            }
        }
    }
}

void draw_shape(int x, int y, int oledstate, bool check, bool *is_occupied)
{
    switch (currentBlock.shape)
    {
    // I
    case I:
        switch (currentBlock.rotation)
        {
        case UP:
        case DOWN:
            draw_block(x, y, oledstate, check, is_occupied);
            draw_block(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_block(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            draw_block(x + BLOCK_SIZE * 3, y, oledstate, check, is_occupied);
            return;
        case LEFT:
        case RIGHT:
            draw_block(x, y, oledstate, check, is_occupied);
            draw_block(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_block(x, y + BLOCK_SIZE * 2, oledstate, check, is_occupied);
            draw_block(x, y + BLOCK_SIZE * 3, oledstate, check, is_occupied);
            return;
        }
        return;

    case J:
        switch (currentBlock.rotation)
        {
        case UP:
        case DOWN:
            draw_block(x, y, oledstate, check, is_occupied);
            draw_block(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_block(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            draw_block(x + BLOCK_SIZE * 2, y + BLOCK_SIZE, oledstate, check, is_occupied);
            return;
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

bool check_will_not_be_out_of_bounds(int x, int y, enum Direction direction)
{
    is_occupied = false;

    if (direction == DOWN)
    {
        // draw_shape with check mode
        draw_shape(x + 1, y, 1, true, &is_occupied);
        if (is_occupied == true)
        {
            return false;
        }
    }

    if (direction == LEFT)
    {
        // draw_shape with check mode
        draw_shape(x, y + BLOCK_SIZE, 1, true, &is_occupied);
        if (is_occupied == true)
        {
            return false;
        }
    }

    if (direction == RIGHT)
    {
        // draw_shape with check mode
        draw_shape(x, y - BLOCK_SIZE, 1, true, &is_occupied);
        if (is_occupied == true)
        {
            return false;
        }
    }

    if (direction == ROTATE)
    {
        enum Direction old_rotation = currentBlock.rotation;
        currentBlock.rotation = (currentBlock.rotation + 1) % 4;
        draw_shape(x, y, 1, true, &is_occupied);
        // return back
        currentBlock.rotation = old_rotation;
        if (is_occupied == true)
        {
            return false;
        }
    }

    return true;
}

void tetris_game_isr(void)
{
    // BTN2, rotation
    if (((btn) & 0x00000001) == 1)
    {
        intended_action = 3;
    }

    // BTN4, turn left
    if (btn >> 2 == 1)
    {
        intended_action = 1;
    }

    // BTN4, turn left
    if (((btn >> 1) & 0x00000001) == 1)
    {
        intended_action = 2;
    }
}

void game(enum GameState *state)
{
    // get status of buttons
    btn = getbtns();

    // just slow things down
    delay(100000);

    // rotate shape
    if (intended_action == 3)
    {
        intended_action = 0;
        if (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, ROTATE))
        {
            currentBlock.rotation = (currentBlock.rotation + 1) % 4;
        }
    }

    // turn LEFT
    if (intended_action == 1)
    {
        intended_action = 0;
        if (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, LEFT))
        {
            // draw_shape(currentBlock.x, currentBlock.y, 0, false, &is_occupied);
            currentBlock.y = currentBlock.y + BLOCK_SIZE;
        }
    }

    // turn RIGHT
    if (intended_action == 2)
    {
        intended_action = 0;
        if (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, RIGHT))
        {
            // draw_shape(currentBlock.x, currentBlock.y, 0, false, &is_occupied);
            currentBlock.y = currentBlock.y - BLOCK_SIZE;
        }
    }

    // tetris is 20x10 blocks
    // one block is 3x3 pixels

    // now collision
    if (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, DOWN) == false)
    {
        // check closest whole block
        // draw it
        draw_shape(currentBlock.x, currentBlock.y, 1, false, &is_occupied);

        // now clear rows
        check_full_rows();

        // new block
        currentBlock.x = 1;
        currentBlock.y = 1;

        // check also if new block is stuck
        if (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, DOWN) == false)
        {
            // game over
            *state = GAMEOVER;
        }
    }

    // no collision, move the shape down
    if (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, DOWN) == true)
    {
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

    draw_shape(currentBlock.x, currentBlock.y, 1, false, &is_occupied);

    display_change();

    // undraw temporarily
    draw_shape(currentBlock.x, currentBlock.y, 0, false, &is_occupied);
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
