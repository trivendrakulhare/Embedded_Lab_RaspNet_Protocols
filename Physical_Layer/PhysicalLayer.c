/*
* @author: Trivendra Kulhare
* Date: 10/01/2019
* Embedded Lab
*/

#include <stdio.h>
#include "CRC.h"

unsigned char *buffer = "test";
uint8_t tempRecieveBuffer;
uint8_t preamble = 0x7E;
uint8_t *frame;
uint8_t *payload;
uint8_t destinationAddress = 0x02;
uint8_t sourceAddress = 0x01;
uint8_t myAddress = 0x01;
uint8_t tempPreamble;
int buffSize;

/**
 * Assigning Send buffer according to the specification i.e. 
        1st byte PREAMBLE, Next Four Byte CRC, 5th Byte is Size of data, 6th Byte is DESTINATION ADDRESS
        7th Byte is SOURCE ADDRESS, and Next Bytes is for the Data 
*/

uint8_t *frameInitialize()
{

  /// getting the length of buffer to initialize the frame.

  buffSize = strlen(buffer);

  /// Dynamically Aloocating the frame size for send buffer

  frame = (uint8_t *)malloc((8 + buffSize) * sizeof(uint8_t));

  /// Payload includes Source, Destination and data

  payload = (uint8_t *)malloc((2 + buffSize) * sizeof(uint8_t));

  memcpy(payload, &destinationAddress, 1);
  memcpy(payload + 1, &sourceAddress, 1);
  memcpy(payload + 2, (uint8_t *)buffer, buffSize);

  /// passing these valuues to the function to get the CRC value for the Payload

  uint32_t crc = crc32b(payload, buffSize + 2);

  /// making frame Byte by Byte.

  memcpy(frame, &preamble, 1);
  *(frame + 1) = crc >> 24;
  *(frame + 2) = crc >> 16;
  *(frame + 3) = crc >> 8;
  *(frame + 4) = crc;
  memcpy(frame + 5, &buffSize, 1);
  memcpy(frame + 6, &destinationAddress, buffSize);
  memcpy(frame + 7, &sourceAddress, buffSize);
  memcpy(frame + 8, (uint8_t *)buffer, buffSize);

  return frame;
}

/**
* to return the source address 
*/

uint8_t myAddress()
{

  return sourceAddress;
}