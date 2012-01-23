/*
 * tempSerialMgr.c
 *
 * Created: 12/30/2011 9:40:41 AM
 *  Author: matt poppe
 */ 
 
 
 // I am using this to test the RS485 signal out of the somfy port
 #include "board.h"
 
 #include "serial.h"
 #include "string.h"
 
 #include "conf_usart_serial.h"
 #include "DTG.h"
 #include "tempSerialMgr.h"
 #include "somfySerialMgr.h"
 
 #include "print_func.h"


uint8_t TempTxBuffer[128];
uint8_t TempRxBuffer[128];

usartDescriptor_t usart_temp = {

	USART_CHANNEL_USARTD1,			// Channel
	TEMP_SERIAL_BAUDRATE,			// baudrate
	TEMP_SERIAL_CHARLEN,			// character length
	TEMP_SERIAL_PAIRITY,			// parity
	TEMP_SERIAL_STOPBIT,			// stopbit
	NULL,							// RS485 transmit enable pin
	false,							// Echo Enabled
	&temp_txCBhandler,				// transmit callback
	&TempTxBuffer,					// actual buffer
	NULL,							// pointer to the buffer TODO this needs to be removed
	128,							// Actual size
	NULL,							// POW
	NULL,							// POR
	sizeof(TempTxBuffer),			// Transmit data size
	TX_IDLE,						// tx state
	&TempRxBuffer,					// rx buffer
	NULL,							// rx PoW
	NULL,							// rx PoR
	&temp_rxCBhandler,				// rx callback
	sizeof(TempRxBuffer),			// rx buffersize
};

void temp_txCBhandler(void)
{
	
}
void temp_rxCBhandler(uint8_t num)
{
	uint8_t readString[3];
	
	print_char_hex(num, readString);
	//usart_writeString(&usart_somfyDebug, readString);

}





