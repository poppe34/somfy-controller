/*
 * DTG.h
 * Device to GUI interface
 * Created: 12/15/2011 12:36:13 PM
 *  Author: matt poppe
 */ 


#ifndef DTG_H_
#define DTG_H_

#include "messages_callbacks.h"


#define transmitString()

//Serial buffer data for Temp
extern uint8_t tempSerialBuf[];
extern uint8_t *tempSerialPtr;
extern uint8_t tempSerialCnt;

//Serial buffer data for somfy device
extern uint8_t somfyDevSerialBuf[];
extern uint8_t *somfyDevSerialPtr;
extern uint8_t somfyDevSerialCnt;

//Serial buffer data for somfy debug
extern uint8_t somfyDebugSerialBuf[];
extern uint8_t *somfyDebugSerialPtr;
extern uint8_t somfyDebugSerialCnt;

//Serial buffer data for PC interface
extern uint8_t PCSerialBuf[];
extern uint8_t *PCSerialPtr;
extern uint8_t PCSerialCnt;

typedef enum{
	transmit_OK = 0x00,
	transmit_NoResponce = 0x80,
}messageStatus_t;


typedef void (*messageStatusCB_t)(messageStatus_t status);

typedef struct{
	uint8_t len;
	uint8_t *data;
	messageStatusCB_t cb;
}message_t;

typedef void (*messageCB_t)(uint8_t *);

//extern cmdResps[];
extern PROGMEM_STRING_T commands[];
extern messageCB_t cmdCallBacks[];


void matchString(uint8_t *msg);
uint8_t getOutOfProgMem(uint8_t *buffer, PROGMEM_STRING_T str);
void Tx_prgmem_buffer(PROGMEM_STRING_T str);
void Tx_packet_usart(USART_t *usart, const uint8_t *data, size_t len);
#endif /* DTG_H_ */