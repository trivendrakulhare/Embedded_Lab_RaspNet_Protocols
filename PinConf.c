/*
* @author: Trivendra Kulhare
* Date: 10/01/2019
* Embedded Lab
*/

#ifndef F_CPU
#define F_CPU 12000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>

/**
*Initializing the pins of port D and port B in which B1 and D3 is used for clock and B2 and D2 is used for Data.
*/

void pinConfiguration()
{

	//calculate time frequency
	OCR1A = 0x07A0;

	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << CS12) | (1 << CS10);

	DDRB |= _BV(PB1) | _BV(PB2);
	DDRC |= _BV(PC2) | _BV(PC3);

	PCMSK0 |= (1 << PCINT1);
	PCICR |= (1 << PCIE0);

	PCMSK2 |= (1 << PCINT19);
	PCICR |= (1 << PCIE2);
}