/* tetris.c

    (c) 2023
    Vilhelm Prytz
    Filip Dimitrijevic
*/

#include <stdint.h>
#include "display.h"
#include "buttons.h"
#include "tetris.h"

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
    display_string(0, "GAME");
    display_string(2, "");
    display_string(3, "");
    display_update();
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
