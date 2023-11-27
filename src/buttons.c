#include <pic32mx.h>
#include <stdint.h>

void btninit(void)
{
    // Set bits 11 through 5 of Port D as inputs
    TRISD = TRISD | 0x00000FE0; // Clear the lower 8 bits to set them as outputs
    TRISF = TRISF | 0x00000002;
}

int getbtns(void)
{
    volatile uint32_t *PORTD_ptr = PORTD;
    volatile uint32_t *PORTF_ptr = PORTF;

// Read the status of buttons BTN4, BTN3, BTN2 (bits 7-5 of Port D)
    int buttonsD = (*PORTD_ptr >> 5) & 0x00000007;

    // Read the status of button BTN1 (assuming it's bit 1 of Port F)
    int buttonF = (*PORTF_ptr >> 1) & 0x00000001;

    // Combine the button statuses
    int buttons = (buttonF << 3) | buttonsD;

    // Return the combined button status
    return buttons;
}