#include <avr/io.h>

void CHIP_Init()
{
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;
    DDRE = 0xFF;    
    DDRF = 0xFF;   
    DDRG = 0xFF;

	PORTD = 0x00;
	PORTE = 0x08;
	PORTF = 0x00;

	PORTC |= 0x20;
	PORTB = 0xFF;
	PORTC = 0x02;

	PORTG |= 0x18;
	PORTB = 0x00;
	PORTG = 0x00;
}
