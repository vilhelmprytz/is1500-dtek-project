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
#include "random.h"

#define GAME_HEIGHT 60
#define GAME_WIDTH 30
#define BLOCK_SIZE 3
#define MAX_HIGH_SCORES 3 // maybe less for high score

#define BASE_DELAY 250000 // Base delay when score is 0
#define MIN_DELAY 2500    // Minimum delay when score is 450 or more
#define TARGET_SCORE 450  // Score at which minimum delay is reached

int current_score;
int btn;
int intended_action;
bool is_occupied;

char initials[3] = "AAA";

int highScores[3];
char highScoreInitials[3][3] = {"AAA", "AAA", "AAA"};

// intended_action
// 0 - do nothing
// 1 - move left
// 2 - move right
// 3 - rotate
// 4 - fast drop

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

void draw_square(int x, int y, int oledstate, bool check, bool *is_occupied)
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

void draw_shape(int x, int y, int oledstate, bool check, bool *is_occupied, enum Shape shape)
{
    switch (shape)
    {
    // I
    case I:
        switch (currentBlock.rotation)
        {
        case UP:
        case DOWN:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 3, y, oledstate, check, is_occupied);
            return;
        case LEFT:
        case RIGHT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE * 2, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE * 3, oledstate, check, is_occupied);
            return;
        }
        return;

    // J
    case J:
        switch (currentBlock.rotation)
        {
        case UP:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y + BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        case DOWN:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x, y - BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            return;
        case LEFT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE * 2, oledstate, check, is_occupied);
            draw_square(x - BLOCK_SIZE, y + BLOCK_SIZE * 2, oledstate, check, is_occupied);
            return;
        case RIGHT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE * 2, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            return;
        }
        return;

    // L
    case L:
        switch (currentBlock.rotation)
        {
        case UP:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y - BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        case DOWN:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            return;
        case LEFT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y + BLOCK_SIZE * 2, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE * 2, oledstate, check, is_occupied);
            return;
        case RIGHT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x - BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE * 2, oledstate, check, is_occupied);
            return;
        }
        return;

    // O
    case O:
        switch (currentBlock.rotation)
        {
        case UP:
        case DOWN:
        case LEFT:
        case RIGHT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        }
        return;

    // S
    case S:
        switch (currentBlock.rotation)
        {
        case UP:
        case DOWN:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x, y - BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y + BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        case LEFT:
        case RIGHT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y - BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y - BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        }
        return;

    // T
    case T:
        switch (currentBlock.rotation)
        {
        case UP:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x, y - BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            return;
        case DOWN:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y - BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y + BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        case LEFT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y + BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        case RIGHT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y - BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        }
        return;

    // Z
    case Z:
        switch (currentBlock.rotation)
        {
        case UP:
        case DOWN:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y - BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        case LEFT:
        case RIGHT:
            draw_square(x, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE, y + BLOCK_SIZE, oledstate, check, is_occupied);
            draw_square(x + BLOCK_SIZE * 2, y + BLOCK_SIZE, oledstate, check, is_occupied);
            return;
        }
        return;
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

            // draw new score
            draw_score();
        }
    }
}

bool check_will_not_be_out_of_bounds(int x, int y, enum Direction direction)
{
    is_occupied = false;

    if (direction == DOWN)
    {
        // draw_shape with check mode
        draw_shape(x + 1, y, 1, true, &is_occupied, currentBlock.shape);
        if (is_occupied == true)
        {
            return false;
        }
    }

    if (direction == LEFT)
    {
        // draw_shape with check mode
        draw_shape(x, y + BLOCK_SIZE, 1, true, &is_occupied, currentBlock.shape);
        if (is_occupied == true)
        {
            return false;
        }
    }

    if (direction == RIGHT)
    {
        // draw_shape with check mode
        draw_shape(x, y - BLOCK_SIZE, 1, true, &is_occupied, currentBlock.shape);
        if (is_occupied == true)
        {
            return false;
        }
    }

    if (direction == ROTATE)
    {
        enum Direction old_rotation = currentBlock.rotation;
        currentBlock.rotation = (currentBlock.rotation + 1) % 4;
        draw_shape(x, y, 1, true, &is_occupied, currentBlock.shape);
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
    if (btn & 0x00000008) // BTN1 for fast drop
    {
        intended_action = 4;
    }
    else if (btn & 0x00000001) // BTN2, rotation
    {
        intended_action = 3;
    }
    else if (btn & 0x00000004) // BTN4, turn left
    {
        intended_action = 1;
    }
    else if (btn & 0x00000002) // BTN3, turn right
    {
        intended_action = 2;
    }
}

void draw_int(int x, int y, int num)
{
    int i;
    int j;

    for (i = 0; i < 7; i++)
    {
        for (j = 0; j < 5; j++)
        {
            display[x + j][y + i] = num_font[num * 35 + i * 5 + j];
        }
    }
}

void draw_score()
{
    draw_int(26, 70, current_score / 100);
    draw_int(20, 70, (current_score / 10) % 10);
    draw_int(14, 70, current_score % 10);
}

unsigned long calculate_delay()
{
    if (current_score >= TARGET_SCORE)
    {
        return MIN_DELAY;
    }
    else
    {
        // calculate delay based on current score
        // this formula linearly decreases the delay value as the score increases
        return BASE_DELAY - (current_score * (BASE_DELAY - MIN_DELAY) / TARGET_SCORE);
    }
}

void game(enum GameState *state)
{
    // get status of buttons
    btn = getbtns();

    // just slow things down
    delay(calculate_delay());

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
            currentBlock.y = currentBlock.y + BLOCK_SIZE;
        }
    }

    // turn RIGHT
    if (intended_action == 2)
    {
        intended_action = 0;
        if (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, RIGHT))
        {
            currentBlock.y = currentBlock.y - BLOCK_SIZE;
        }
    }

    // fast drop
    if (intended_action == 4)
    {
        intended_action = 0; // reset intended action
        while (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, DOWN))
        {
            currentBlock.x += 1; // Move the block down
        }
    }

    // tetris is 20x10 blocks
    // one block is 3x3 pixels

    // now collision
    if (check_will_not_be_out_of_bounds(currentBlock.x, currentBlock.y, DOWN) == false)
    {
        // check closest whole block
        // draw it
        draw_shape(currentBlock.x, currentBlock.y, 1, false, &is_occupied, currentBlock.shape);

        // now clear rows
        check_full_rows();

        // new block
        currentBlock.x = 4;
        currentBlock.y = ((30 / 3) / 2) * 3 + 1;

        currentBlock.shape = currentBlock.nextShape;
        currentBlock.nextShape = rand() % 7;

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

    // draw next shape below game area
    draw_shape(85, 22, 1, false, &is_occupied, currentBlock.nextShape);

    // draw shape
    draw_shape(currentBlock.x, currentBlock.y, 1, false, &is_occupied, currentBlock.shape);

    display_change();

    // undraw temporarily, both current shape and next shape
    draw_shape(currentBlock.x, currentBlock.y, 0, false, &is_occupied, currentBlock.shape);
    draw_shape(85, 22, 0, false, &is_occupied, currentBlock.nextShape);
}

void gameover(enum GameState *state)
{
    display_string(0, "GAME OVER");
    display_string(1, itoaconv(current_score));
    display_string(2, initials);
    display_string(3, "BTN4 - Menu");
    display_update();
}

/*
Updating high scores
*/

void updateHighScore(int newScore)
{
    int i;
    for (i = 0; i < MAX_HIGH_SCORES; i++)
    {
        if (newScore > highScores[i])
        {
            // Shift lower scores down
            int j;
            for (j = MAX_HIGH_SCORES - 1; j > i; j--)
            {
                highScores[j] = highScores[j - 1];
                // Copy initials, without using strcpy
                int k;
                for (k = 0; k < 3; k++)
                {
                    highScoreInitials[j][k] = highScoreInitials[j - 1][k];
                }
            }
            // Insert new high score
            highScores[i] = newScore;
            // Insert new high socre initials, without using strcpy
            int k;
            for (k = 0; k < 3; k++)
            {
                highScoreInitials[i][k] = initials[k];
            }
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
        int k;
        for (k = 0; k < 3; k++)
        {
            scoreLine[k] = highScoreInitials[i][k];
        }

        scoreLine[3] = *" ";

        scoreLine[4] = *itoaconv(highScores[i] / 100);
        scoreLine[5] = *itoaconv((highScores[i] / 10) % 10);
        scoreLine[6] = *itoaconv(highScores[i] % 10);

        display_string(i + 1, scoreLine); // Adjust the line number as needed
    }

    display_update();
}
