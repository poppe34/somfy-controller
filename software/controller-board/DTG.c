/*
 * DTG.c
 * Device to GUI interface
 * Created: 12/15/2011 12:35:22 PM
 *  Author: matt poppe
 */ 
#include "board.h"
#include "string.h"

#include "serial.h"

#include "messages_callbacks.h"
#include "DTG.h"
#include "somfy.h"

#include <stdio.h>



uint8_t getOutOfProgMem(uint8_t *buffer, PROGMEM_STRING_T str)
{
	char holderChar;
	uint8_t cnt = 0;
	//this gets the command out of program memory and puts it in RAM
	holderChar = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)str);
	//repeat through the string until we hit a zero
	while(holderChar)
	{
		*buffer++ = holderChar;
		str++;
		cnt++;
		holderChar = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)str);
	}
	*buffer = '\0';
	return cnt;
}

void matchString(uint8_t *msg)
{
	uint8_t strHolder[40];
	bool match;
	messageStatus_t status = transmit_NoResponce;
	uint8_t cnt;
	uint8_t size = 5; //sizeof(*cmdCallBacks) / sizeof(messageCB_t);
	
	
	//Go through all the options available and look for a match
	for(uint8_t x=0; x<size; x++)
	{
		match = false;
		cnt = getOutOfProgMem(strHolder, commands[x]);


		// compare if the two strings are the same 0 is returned by memcmp
		match = memcmp(msg, strHolder, cnt); 

		//execute the callback for the matched string
		if(!match)
		{
			msg += (strlen(strHolder) + 1);	
			status = transmit_OK;
			if(cmdCallBacks[x])
			{
				(cmdCallBacks[x])(msg);
			}				
		}
		
	}
	//Go through all keywords and execute keywords
}

void Tx_prgmem_buffer(PROGMEM_STRING_T str)
{
	char temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)str);
	
	while(temp_char)
	{
		temp_char = PROGMEM_READ_BYTE((uint8_t PROGMEM_PTR_T)str++);
		
	}
}

usart_serial_write_packet(USART_t *usart,const uint8_t *data, size_t len);

void Tx_packet_usart(USART_t *usart, const uint8_t *data, size_t len)
{
	
}