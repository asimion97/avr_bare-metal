#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Set Output Port
    DDRB |= (1 << PB5);

    while (1) {
        // ON LED
        PORTB |= (1 << PB5);
        _delay_ms(500);

        // OFF LED
        PORTB &= ~(1 << PB5);
        _delay_ms(500);
    }

    return 0;
}

