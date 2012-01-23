/*
 * messages_callbacks.c
 *
 * Created: 12/15/2011 12:51:06 PM
 *  Author: matt poppe
 */ 

#include <board.h>
#include "messages_callbacks.h"
#include "PCserialMgr.h"

#include "somfy.h"


PROGMEM_DECLARE(const char, cmd_query[]) = "QUERY";
PROGMEM_DECLARE(const char, cmd_send[]) = "SEND";
PROGMEM_DECLARE(const char, cmd_up[]) = "UP";
PROGMEM_DECLARE(const char, cmd_down[]) = "DOWN";
PROGMEM_DECLARE(const char, cmd_stop[]) = "STOP";
PROGMEM_DECLARE(const char, REPLY_STATUS_OK[]) = "OK\r\n";


PROGMEM_STRING_T commands[] = {
	cmd_query,
	cmd_send,
	cmd_up,
	cmd_down,
	cmd_stop,
};

messageCB_t cmdCallBacks[] = {
	cmd_QueryHandler, //query
	cmd_sendHandler, //send
	cmd_upHandler, //up
	cmd_downHandler, //down
	PCserial_statusCallBack, //stop
};

void cmd_QueryHandler(uint8_t *data)
{
	somfyAddr_t addr;
	uint8_t device[3], x = 0;
	uint8_t *ptr = &addr.firstByte;
	control_move_t move;
	
	// get all three bytes of the address
	for(uint8_t y = 0; y < 3; y++)
	{
		while(*data == ' ')
		{
			data++;
		}
		
		//convert the string Byte to an Interger Byte;
		*ptr = atoi(data);
		if(*ptr > 99 )
		{
			data++;
		}//if the string is greater than 2 digits long		
		
		//increment data the size of a byte
		data += 2;
		ptr++;
	}		
	//populate the move data
	move.moveTo = move_to_up_limit;
	move.value = 100;
	
	//execute move
	somfy_reqMotorPos(addr);
}

void cmd_sendHandler(uint8_t *data)
{
	somfyAddr_t addr;
	uint8_t device[3], x = 0;
	uint8_t *ptr = &addr.firstByte;
	control_move_t move;
	
	// get all three bytes of the address
	for(uint8_t y = 0; y < 3; y++)
	{
		while(*data == ' ')
		{
			data++;
		}
		
		//convert the string Byte to an Interger Byte;
		*ptr = atoi(data);
		if(*ptr > 99 )
		{
			data++;
		}//if the string is greater than 2 digits long		
		
		//increment data the size of a byte
		data += 2;
		ptr++;
	}		
	//populate the move data
	move.moveTo = move_to_up_limit;
	move.value = 100;
	
	//execute move
	somfy_control_moveAbsPos(addr, &move);
}

void cmd_upHandler(uint8_t *data)
{
	somfyAddr_t addr;
	uint8_t device[3], x = 0;
	uint8_t *ptr = &addr.firstByte;
	control_move_t move;
	
	// get all three bytes of the address
	for(uint8_t y = 0; y < 3; y++)
	{
		while(*data == ' ')
		{
			data++;
		}
		
		//convert the string Byte to an Interger Byte;
		*ptr = atoi(data);
		if(*ptr > 99 )
		{
			data++;
		}//if the string is greater than 2 digits long		
		
		//increment data the size of a byte
		data += 2;
		ptr++;
	}		
	//populate the move data
	move.moveTo = move_to_up_limit;
	move.value = 100;
	
	//execute move
	somfy_control_moveAbsPos(addr, &move);
}

void cmd_downHandler(uint8_t *data)
{
	somfyAddr_t addr;
	uint8_t device[3], x = 0;
	uint8_t *ptr = &addr.firstByte;
	control_move_t move;
	
	//cycle thru string to get all 3 bytes
	for(uint8_t y = 0; y < 3; y++)
	{
		while(*data == ' ')
		{
			data++;
		}
		
		//convert the string Byte to an Interger Byte;(must be a decimal value)
		*ptr = atoi(data);
		if(*ptr > 99 )
		{
			data++;
		}//if the string is greater than 2 digits long		
		//increment data the size of a byte
		data += 2;
		ptr++;
	}	
	
	//populate move data
	move.moveTo = move_to_down_limit;
	move.value = 100;
	
	//execute move
	somfy_control_moveAbsPos(addr, &move);
}