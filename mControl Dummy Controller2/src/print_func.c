/*
 * print_func.c
 *
 * Created: 1/3/2012 3:27:47 PM
 *  Author: mpoppe
 */ 
#include "board.h"

static const char HEX_DIGITS[16] = "0123456789ABCDEF";
 
void print_char_hex(uint8_t num, uint8_t *buffer)
{
	uint8_t x;
	
	buffer[2] = '\0';
	buffer[1] =  HEX_DIGITS[num & 0xf];
	num >>= 4;
	buffer[0] = HEX_DIGITS[num & 0xf];
	
}

 string_itemize(uint8_t *buffer, uint8_t itemizedChar)
{
	//used to get the count of the itemized char
	uint8_t cnt = 0;
	
	while(*buffer != NULL)
	{
		if(*buffer == itemizedChar)
		{
			cnt++;
		}
		buffer++;
	}
	
	uint8_t testie[cnt];
}

void print(USART_t *usart, uint8_t *num)
{
	
}