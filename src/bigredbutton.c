#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bigredbutton.h"
#include "usb_keyboard.h"

volatile int typing = 0;

/* Entry Point */
int main()
{
    initialize();
 
    for(;;)
    {
    }
 
    return 0;
}
 
/* Initialize the keyboard and the microcontroller */
void initialize()
{

    typing = 0;

    usb_init();

    while(!usb_configured()) 
    {
        /* wait */        
    } 
 
    /* Configure pins and interrupts */
    DDRB &= ~(1<<PB0); /* Set PB0 as input */
    PORTB |= (1<<PB0); /* Activate PULL UP resistor */ 
 
    PCMSK0 |= (1 << PCINT0); /* Enable PCINT0 */
    PCICR |= (1 << PCIE0); /* Activate interrupt on enabled PCINT7-0 */

    /* Enable interrupts */
    sei ();
}

/* Interrupt Handler */
ISR(PCINT0_vect)
{
    cli();

    if (0 == typing && 1 == (PINB & (1 << PB0)))
    {
        int i = 0;
        typing = 1;

        unsigned int buildString[] = { KEY_B, KEY_U, KEY_I, KEY_L, KEY_D, KEY_SPACE, 
            KEY_MINUS, KEY_C, KEY_SPACE, KEY_SLASH, KEY_P, KEY_A, KEY_R, KEY_E, KEY_N, KEY_T, KEY_ENTER};
        
        do 
        {
            usb_keyboard_press(buildString[i], 0);
            usb_keyboard_send();
            _delay_ms(10);
        }
        while (KEY_ENTER != buildString[i++]);

        typing = 0;
    }

    sei();      
}