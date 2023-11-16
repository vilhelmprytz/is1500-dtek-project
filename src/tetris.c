/* tetris.c

    (c) 2023
    Vilhelm Prytz
    Filip Dimitrijevic
*/

#include <stdint.h>
#include "display.h"

void menu()
{
    display_string(0, "TETRIS");
    display_string(2, "BTN3 - Highscores");
    display_string(3, "BTN4 - Start");
    display_update();
}

void game()
{
    display_string(0, "GAME");
    display_update();
}

void gameover()
{
    display_string(0, "GAME OVER");
    display_update();
}