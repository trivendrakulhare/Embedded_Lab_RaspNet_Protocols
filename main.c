/*
* @author: Trivendra Kulhare
* Date: 10/01/2019
* Embedded Lab
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART_Init.h"
#include "ISR.h"
#include "communication.h"
#include "PinConf.h"
#include "PhysicalLayer.h"

/**
* calling the UART configuration, init functions to initialize the Buffers, to initialize the pin configuration
  and UserInput function to put message in the minicom to display.   
*/

void main()
{

	uart_init();
	init();
	pinConfiguration();
	userInput();

	sei();
	/// enable interrupts

	for (;;)
	{
	}
}