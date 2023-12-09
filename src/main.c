/* main.c, copied from mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING

    New changes written by Vilhelm Prytz and Filip Dimitrijevic
    (C) Vilhelm Prytz 2023
    (C) Filip Dimitrijevic
*/

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "display.h" /* Declatations for these labs */
#include "tetris.h"
#include "random.h"

enum GameState state;

int btn;

void user_isr(void)
{
    if (state == MENU)
    {
        // BTN4 check
        if (btn >> 2 == 1)
        {
            current_score = 0;
            currentBlock.x = 4;
            currentBlock.y = ((30 / 3) / 2) * 3 + 1;
            currentBlock.shape = rand() % 7;
            currentBlock.nextShape = rand() % 7;
            currentBlock.rotation = UP;

            state = GAME;
        }

        // BTN3 check
        if (((btn >> 1) & 0x00000001) == 1)
        {
            state = HIGHSCORE;
        }
    }

    if (state == GAME)
    {
        tetris_game_isr();
    }

    if (state == GAMEOVER || state == HIGHSCORE)
    {
        // BTN4 check
        if (btn >> 2 == 1)
        {
            display_clear();
            state = MENU;
        }
    }

    // Acknowledge the Timer 2 interrupt by clearing the interrupt flag
    IFSCLR(0) = 0x00000100;
}

void timerinit(void)
{
    // TCS = 0, internal peripheral clock
    // T32 = 0, 16-bit timer
    // TCKPS = 7, 1:256 prescaling (TCKPS)
    // TGATE = 0, gated time accumulation disabled
    // SDL = 0, continue operation when device enters idle mode
    // ON = 1, module is enabled
    T2CON = (T2CON & 0xFFFF0000) | 0x00008070;

    // Clear the timer register
    TMR2 = 0x0;

    // Load the period register
    // Assuming 80 MHz
    // 80 000 000 / 256 = 312 500 (1 second is 312 500 in timer)
    // 10 timeout per second, 100 ms => 312 500 / 10 = 31 250
    PR2 = 0x7A12; // 31 250

    // Set priority level = 3,
    // then subpriority level = 1
    IPCSET(2) = 0x0000000C;
    IPCSET(2) = 0x00000001;

    // Clear the timer interrupt status flag
    IFSCLR(0) = 0x00000100;

    // Enable timer interrupts
    IECSET(0) = 0x00000100;

    // Start the timer
    T2CONSET = 0x8000;

    enable_interrupt();
}

int main(void)
{
    /*
  This will set the peripheral bus clock to the same frequency
  as the sysclock. That means 80 MHz, when the microcontroller
  is running at 80 MHz. Changed 2017, as recommended by Axel.
*/
    SYSKEY = 0xAA996655; /* Unlock OSCCON, step 1 */
    SYSKEY = 0x556699AA; /* Unlock OSCCON, step 2 */
    while (OSCCON & (1 << 21))
        ;                 /* Wait until PBDIV ready */
    OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
    while (OSCCON & (1 << 21))
        ;         /* Wait until PBDIV ready */
    SYSKEY = 0x0; /* Lock OSCCON */

    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up input pins */
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    /* SPI2STAT bit SPIROV = 0; */
    SPI2STATCLR = 0x40;
    /* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
    /* SPI2CON bit MSTEN = 1; */
    SPI2CONSET = 0x20;
    /* SPI2CON bit ON = 1; */
    SPI2CONSET = 0x8000;

    // init display
    display_init();

    // init buttons
    btninit();

    // init timer interrupt
    timerinit();

    // seed our pseudo random function
    srand(72390347);

    // keep track of state
    state = MENU;
    while (1)
    {
        btn = getbtns();

        if (state == MENU)
        {
            menu(&state);
        }
        else if (state == GAME)
        {
            game(&state);
        }
        else if (state == GAMEOVER)
        {
            gameover(&state);
        }
        else if (state == HIGHSCORE)
        {
            highscore(&state);
        }
        else
        {
            display_string(0, "fatal error, something broke");
            display_update();
        }
    }
    return 0;
}