#include <stdio.h>
#include "UART_Init.h"

/**
* to calculate the CRC value of payload.
*/

uint32_t crc32b(uint8_t *message, int Psize)
{
	uint32_t polynomial = 0x04C11DB7; ///Take the polynomial to a 32 bit variable.

	uint32_t crc1 = 0; ///Initially crc will be null.

	int n = 0; ///Variable to check crc is calculated for all characters in the message.

	while ((message[n] != 0) || (Psize != 0)) ///When message has a character or Psize is not zero.
	{
		unsigned char b = message[n];

		crc1 ^= ((uint32_t)b << 24); ///Perform XOR operation of crc with character in the message converted to 32 bit character.
		int m;
		for (m = 0; m < 8; m++) ///Run the loop 8 times.
		{
			if ((crc1 & 0x80000000) != 0) ///If MSB of crc is 1, right shift crc by 1 and XOR with polynomial.
			{
				crc1 = (uint32_t)((crc1 << 1) ^ polynomial);
			}

			else ///If MSB of crc is 0, just right shift crc by 1.
			{
				crc1 <<= 1;
			}
		}

		n = n + 1; ///Increment n by 1 to take the next character of the message.
		Psize--;
	}

	return crc1; ///Return the final computed 32 bit crc.
}