/*
* @author: Trivendra Kulhare
* Date: 10/01/2019
* Embedded Lab
*/

#include <stdio.h>
#include "UART_Init.h"
#include "CRC.h"

/**
    * if the recieved buff destination address is '0', then will Broadcast the Message and there is a check to see if
    message came back to source then will Complete the Broadcast.
*/

/**
    * if the recieved buff destination address is not equal to source address, then forward the same.
*/

int validationProcess(uint8_t *recievedBuff)
{
    /// Check to see if Destination address is same as my address, then perform the CRC Check

    if (*(recievedBuff + 6) == srcAddress)

    {

        uint8_t recievedMessageSize = *(recievedBuff + 5);

        uint8_t *crcTempBuffer = (uint8_t *)malloc(recievedMessageSize * sizeof(uint8_t));

        memcpy(crcTempBuffer, (recievedBuff + 6), recievedMessageSize + 2); /// adding source and destination address in the                                                                        payload for CRC calculation

        uint32_t crcTemp = crc32b((uint8_t *)crcTempBuffer, recievedMessageSize);

        uint8_t *recievedCRC = (uint8_t *)malloc(recievedMessageSize * sizeof(uint8_t));

        /// storing the recieved CRC value in a variable
        *(recievedCRC) = *(recievedBuff + 1);
        *(recievedCRC + 1) = *(recievedBuff + 2);
        *(recievedCRC + 2) = *(recievedBuff + 3);
        *(recievedCRC + 3) = *(recievedBuff + 4);

        /// storing the current CRC value in temprory variable.

        *(crcTempBuffer) = crcTemp >> 24;
        *(crcTempBuffer + 1) = crcTemp >> 16;
        *(crcTempBuffer + 2) = crcTemp >> 8;
        *(crcTempBuffer + 3) = crcTemp;

        /// check to see if recieved packet CRC bytes are same as the CRC which we calculated in the code.

        if ((*(recievedCRC + 1) == *(crcTempBuffer)) &&
            (*(recievedCRC + 2) == *(crcTempBuffer + 1)) &&
            (*(recievedCRC + 3) == *(crcTempBuffer + 2)) &&
            (*(recievedCRC + 4) == *(crcTempBuffer + 3)))
        {
            uart_putString('CRC PASS');
            uart_putchar('\n');
            uart_putString('Data:');
            int m;

            /// printing the data value which we recieved in the message.

            for (m = 0; m < recievedMessageSize; m++)
            {
                uart_putchar(*(recievedBuff[8 + m]));
            }
            return 1;
        }
        else
        {
            uart_putString('CRC FAIL');
            uart_putchar('\n');
            return 2;
        }
    }

    /// check to see if the message is to be broadcasted.
    else if (*(recievedBuff + 6) == 0)
    {

        if (*(recievedBuff + 7) == srcAddress) /// if it matches my address then it means broadcast is completed.
        {
            uart_putString('BROADCAST COMPLETE');
            uart_putchar('\n');
            return 0;
        }
        else
        { /// else will forward the message if it contain 0 in destionation field and forward the same.

            uart_putString('BROADCASTING MESSAGE');
            uart_putchar('\n');
            return 3;
        }
    }

    /// check to see if recieved message address doesn't match my address then forward the message.

    else if (*(recievedBuff + 6) != srcAddress)
    {

        uart_putString('FORWARDING');
        uart_putchar('\n');
        return 3;
    }
}