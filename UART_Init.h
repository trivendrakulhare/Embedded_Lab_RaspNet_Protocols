/*
* @author: Trivendra Kulhare
* Date: 10/01/2019
* Embedded Lab
*/

/** 
* function call to initialize all the UART configuration 
*/

void uart_init(void);

/** 
* function call to get the character from the MINICOM 
*/

char uart_getchar(void);

/** 
* function call to get the Array of all the character 
*/

void uart_putString(char *string);