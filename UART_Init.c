#ifndef F_CPU
#define F_CPU 12000000UL
#endif
#ifndef BAUD
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)
#endif
#include <util/setbaud.h>

/**
* UART Configuration, Source from AppleSini website mentioned in the Lab task sheet
*/

void uart_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /// 8-bit data
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /// Enable RX and TX
}

/**
*function to put the character to MINICOM
*/

void uart_putchar(char c)
{
    loop_until_bit_is_set(UCSR0A, UDRE0); /// Wait until data register empty.
    UDR0 = c;
}

/**
*function to get the character to MINICOM 
*/

char uart_getchar(void)
{
    loop_until_bit_is_set(UCSR0A, RXC0); /// Wait until data exists.
    return UDR0;
}

/**
* custom function to get the character from user and put it in a Char Array named as string 
*/

void uart_putString(char *string)
{
    int index;
    for (index = 0; index < strlen(string); index++)
    {
        uart_putchar(string[index]);
    }
}