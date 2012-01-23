/*
 * serial_interface.c
 *
 * Created: 12/15/2011 2:29:18 PM
 *  Author: matt poppe
 */ 
#include "board.h"

#include "asf.h"

#include "masterSerialMgr.h"
#include "messages_callbacks.h"
#include "DTG.h"

#include "conf_usart_serial.h"
#include "PCserialMgr.h"
#include "somfySerialMgr.h"
#include "conf_somfyRS485.h"

uint8_t PCinterTxBuffer[128];
uint8_t PCinterRxBuffer[128];

static Bool newMsgFlag;

usartDescriptor_t usart_PCinter = {

	USART_CHANNEL_USARTD0,			// Channel
	PC_INTERFACE_SERIAL_BAUDRATE,	// baudrate
	PC_INTERFACE_SERIAL_CHARLEN,	// character length
	PC_INTERFACE_SERIAL_PAIRITY,	// parity
	PC_INTERFACE_SERIAL_STOPBIT,	// stopbit
	NULL,							// RS485 transmit enable pin
	true,							// Echo Enabled
	&PCserial_txCBhandler,			// transmit callback
	&PCinterTxBuffer,				// actual buffer
	NULL,							// pointer to the buffer TODO this needs to be removed
	128,							// Actual size
	NULL,							// POW
	NULL,							// POR
	sizeof(PCinterTxBuffer),		// Transmit data size
	TX_IDLE,						// tx state
	&PCinterRxBuffer,				// rx buffer
	NULL,							// rx PoW
	NULL,							// rx PoR
	&PCserial_rxCBhandler,			// rx callback
	128,							// rx buffersize
};

void PCserial_txCBhandler(void)
{
	
}
void PCserial_rxCBhandler(uint8_t num)
{
	uint8_t cnt;
	
	if(num == 2)
	{
		cli();
		usart_PCinter.rxBuffer = PCinterRxBuffer;
		usart_PCinter.rxPoW = PCinterRxBuffer;
		usart_PCinter.rxPoR = PCinterRxBuffer;
		sei();
	}

	if(num == 13)
	{	
			
		
		//add a zero to the buffer to terminate the string
		usart_terminateRxWithNull(&usart_PCinter);
		
		newMsgFlag = true;
#ifdef DEBUG_FROM_PC
		//write to the debug 
		usart_writeString(&usart_somfyDebug, "<incoming MSG>");
		usart_writeString(&usart_somfyDebug, &usart_PCinter.rxPoR);
		usart_writeByte(&usart_somfyDebug, '\r');
		usart_writeByte(&usart_somfyDebug, '\n');
#endif
				
		matchString((usart_readBytes(&usart_PCinter, 0)));
	}	

}

void PCserial_statusCallBack(uint8_t *data)
{

}

void PCserial_task(void)
{
	if(newMsgFlag)
	{
		//lower the flag
		newMsgFlag = false;
		
		somfy_Rx_data(PCinterRxBuffer);
	}
}