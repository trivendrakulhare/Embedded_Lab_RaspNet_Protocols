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
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "UART_Init.h"
#include "CRC.h"
#include "PinConf.h"
#include "PhysicalLayer.h"
#include "NetworkLayer.h"
#define Delay 500

uint8_t *sendBuffer;
uint8_t *recieveBuffer;
uint8_t *tempBuffer;
uint8_t srcAddress;
volatile int preambleCounter = 0;
volatile int read_counter = 0;
volatile int send_counter = 0;
volatile int TotalByte = 0;
volatile int byteCounter = 0;
volatile int sendLock = 0;
volatile int recievedTotalByte = 6;
volatile int recievedByteCounter = 0;
volatile int flagToForward = 0;
int j = 0;

/**
* calling the frameInitialize function to get the send Buffer and initializng the recieve buffer and to get the source address
*/

void init()
{

	sendBuffer = frameInitialize();
	recieveBuffer = (uint8_t *)malloc(recievedTotalByte * sizeof(uint8_t));
	TotalByte = 8 + *(sendBuffer + 5);
	srcAddress = myAddress();
}

/**  
 *if total number of bytes doesn't match the current Byte Index then it will increment byte counter and reinitalize the send counter flag.
*/

void SendBuffer()
{
	if (sendLock != 1) ///Check if the  sendlock is set.
	{

		///Check to see if all the bits of a particular Byte is sent.
		if (send_counter <= 8)
		{
			if ((*(sendBuffer + byteCounter) & 0x80) != 0)
			{
				PORTB |= (1 << PORTB2);
				*(sendBuffer + byteCounter) = ((*(sendBuffer + byteCounter)) << 1);
			}

			else
			{
				PORTB &= ~(1 << PORTB2);
				*(sendBuffer + byteCounter) = ((*(sendBuffer + byteCounter)) << 1);
			}
			send_counter++;
		}

		if (send_counter == 8 && byteCounter != TotalByte)
		{ /// if all the bytes are not send then it will reset the bit                                                           counter and make increment in byte counter
			send_counter = 0;
			byteCounter++;
		}

		if (byteCounter == TotalByte)
		{ /// if all the bytes are recieved then it will set the lock as 1.

			free(sendBuffer);
			sendLock = 1;

			/// chechikng if data is there in the tempbuffer

			if (forwardFlag == 1)
			{
				sendBuffer = (uint8_t *)malloc((6 + (*(tempBuffer + 5))) * sizeof(uint8_t));
				int g;

				for (g = 0; g < (6 + (*(tempBuffer + 5))); g++)
				{
					*(sendBuffer + g) = *(tempBuffer + g);
				}

				free(tempBuffer);
				flagToForward = 0;
				byteCounter = 0;
				send_counter = 0;
				sendLock = 0;
			}
		}
	}
}

/**
    * if validation Value is 3 i.e. destination address doesn't match source address then will forward the packet. While doing this
    will implement two checks i.e. it should check if send buffer is free while forwarding and Incase if it is not free it will wait for it until send buffer get freed .
*/

/**
    * when all the bytes are recieved will perform the data validation by calling the validationProcess function.
*/

void RecieveBuffer()
{
	///Check to see if all bytes of packet is recieved.

	if (recievedByteCounter < recievedTotalByte)
	{
		if (read_counter < 8) ///Check to verify all the bits of particular bytes are recieved.
		{

			if (preambleCounter == 0)
			{
				if (PIND & (1 << PD2))
				{
					tempPreamble = tempPreamble | 0x01;
				}

				else
				{
					tempPreamble = tempPreamble & (~0x01);
				}

				if (tempPreamble == 0b01111110)
				{
					preambleCounter = 1;
					read_counter = 9;
				}

				if (preambleCounter == 0)
					tempPreamble = tempPreamble << 1;
			}

			///Keep checking bits until the preamble matches.
			else if (preambleCounter == 1)
			{
				if (PIND & (1 << PD2))
				{
					tempPreamble = tempPreamble | 0x01;
				}

				else
				{
					tempPreamble = tempPreamble & (~0x01);
				}

				if (read_counter < 7)
				{
					tempPreamble = tempPreamble << 1;
				}
				read_counter++;
			}
		}

		if (read_counter >= 8)
		{
			*(recieveFrame + j) = tempPreamble;
			tempPreamble = 0;
			j++;

			/// Reallocate the recieve buffer once it recieves the size of the data field
			if (recievedByteCounter == 5)
			{

				recievedTotalByte = recievedTotalByte + *(recieveFrame + 5) + 2;
				recieveFrame = (uint8_t *)realloc(recieveFrame, recievedTotalByte * sizeof(uint8_t));
			}

			recievedByteCounter++;
			read_counter = 0;

			/// if all the bytes of data is recived will do validation process
			if (recievedByteCounter == recievedTotalByte)
			{

				validationProcess(recieveFrame);
				int validationValue = validationProcess(recieveFrame);

				if (validationValue == 3) /// if message is to be broadcasted or forwarded then will check if send lock is free.
				{
					TotalByte = 8 + *(recieveFrame + 5);
					if (sendLock == 1)
					{

						sendBuffer = (uint8_t *)malloc(TotalByte * sizeof(uint8_t));
						int i;
						for (i = 0; i < TotalByte; i++)
						{
							*(sendBuffer + i) = *(recieveFrame + i);
						}

						free(recieveFrame);
						recieveFrame = (uint8_t *)malloc(recievedTotalByte * sizeof(uint8_t));
						send_counter = 0;
						byteCounter = 0;
						sendLock = 1;
						preambleCounter = 0;

					} /// if send lock is not free then will put it in the temporary buffer and once send lock is free will copy from temp buffer to Send buffer.
					else
					{
						tempBuffer = (uint8_t *)malloc(TotalByte * sizeof(uint8_t));
						int k;
						for (k = 0; k < TotalByte; k++)
						{
							*(tempBuffer + k) = *(recieveFrame + k);
						}

						free(recieveFrame);
						recieveFrame = (uint8_t *)malloc(recievedTotalByte * sizeof(uint8_t));
						flagToForward = 1;
						preambleCounter = 0;
						j = 0;
						recievedByteCounter = 0;
					}
				}
			}
		}
	}
}

/**
* fucntion to display the Entire string in minicom console to ask for Input
*/

void userInput()
{
	uart_putString("Please enter the message: ");
	int i = 0;
	while (1)
	{
		char in = uart_getchar();

		if (in == 0x0D || i >= 256)
			break;

		uart_putchar(in);
		inputStream[i] = in;
		i++;
	}
	uart_putchar('\n');
}
