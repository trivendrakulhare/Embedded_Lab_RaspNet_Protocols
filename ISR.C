/*
* @author: Trivendra Kulhare
* Date: 10/01/2019
* Embedded Lab
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "Timer.h"

/** 
*Interrupt Serivce routine for the timer and it will toggle PORTB in every clock cycle. 
*/

ISR(TIMER1_COMPA_vect)
{
    PORTB ^= (1 << PORTB1); /// toggle B1 at every clock cycle.
}

/** 
* Interrupt Service Routine  for Sending data at every clock cycle. It will send data bitwise from the Send Buffer 
*/

ISR(PCINT0_vect)
{
    SendBuffer(); // on every clock change trigger SendBuffer.
}

/** 
*Interrupt Service Routine  for Recieving data at every clock cycle. It will recieve data bitwise from the Recieve Buffer 
*/

ISR(PCINT2_vect)
{
    RecieveBuffer();
}