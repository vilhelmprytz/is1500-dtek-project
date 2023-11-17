#include <pic32mx.h>
#include <stdint.h>

void btninit(void)
{
    // Set bits 11 through 5 of Port D as inputs
    TRISD = TRISD | 0x00000FE0; // Clear the lower 8 bits to set them as outputs
}

int getbtns(void)
{
    volatile uint32_t *PORTD_ptr = PORTD;

    // Read the status of buttons BTN4, BTN3, BTN2 (bits 7-5 of Port D)
    int buttons = (*PORTD_ptr >> 5) & 0x00000007;

    // Return the 3 least significant bits containing the button status
    return buttons;
}
