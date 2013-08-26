/*
 * somfySerialMgr.c
 *
 * Created: 10/23/2011 7:22:58 AM
 *  Author: matt
 */ 

#include "board.h"
#include "serial.h"
#include "ioport.h"

#include "conf_usart_serial.h"
#include "masterSerialMgr.h"
#include "somfySerialMgr.h"

#include "print_func.h"

#include "somfy.h"
#include "DTG.h"
#include "string.h"

uint8_t nibble_to_ascii[16] = "0123456789ABCDEF";

uint8_t somfyTxDevBuffer[128];
uint8_t somfyRxDevBuffer[128];

static uint8_t somfyTxDebugBuffer[256];
static uint8_t somfyRxDebugBuffer[256];


static uint8_t packet_tx_cnt_left;
static uint8_t packets_waiting_to_transfer;

usartDescriptor_t usart_somfyDevice = {

	USART_CHANNEL_USARTC0,			// Channel
	SOMFY_DEVICE_DEFAULT_BAUDRATE,	// baudrate
	SOMFY_DEVICE_DEFAULT_CHSIZE,	// character length
	SOMFY_DEVICE_DEFAULT_PAIRITY,	// parity
	SOMFY_DEVICE_DEFAULT_STOPBIT,	// stopbit
	SOMFY_DEVICE_RS485_ENABLE_PIN,	// RS485 transmit enable pin
	false,							// Echo Enabled
	&somfyDeviceTxCB,				// transmit callback
	somfyTxDevBuffer,				// actual buffer
	NULL,							// pointer to the buffer TODO this needs to be removed
	128,							// Actual size
	NULL,							// POW
	NULL,							// POR
	sizeof(somfyTxDevBuffer),		// Transmit data size
	TX_IDLE,						// tx state
	somfyRxDevBuffer,				// rx buffer
	NULL,							// rx PoW
	NULL,							// rx PoR
	&somfyDeviceRxCB,				// rx callback
	128,							// rx buffersize
};

usartDescriptor_t usart_somfyDebug = {
	USART_CHANNEL_USARTC1,			// Channel
	SOMFY_DEBUG_DEFAULT_BAUDRATE,	// baudrate
	SOMFY_DEBUG_DEFAULT_CHSIZE,		// character length
	SOMFY_DEBUG_DEFAULT_PAIRITY,	// parity
	SOMFY_DEBUG_DEFAULT_STOPBIT,	// stopbit
	0,								// RS485 transmit enable pin
	false,							// Echo Enabled
	&somfyDeviceTxCB,				// transmit callback
	somfyTxDebugBuffer,				// actual buffer
	NULL,							// pointer to the buffer TODO this needs to be removed
	256,							// Actual size
	NULL,							// POW
	NULL,							// POR
	sizeof(somfyTxDebugBuffer),		// Transmit data size
	TX_IDLE,						// tx state
	somfyRxDebugBuffer,				// rx buffer
	NULL,							// rx PoW
	NULL,							// rx PoR
	&somfyDeviceRxCB,				// rx callback
	sizeof(somfyRxDebugBuffer), 	// rx buffersize
	
};	

void somfy_serial_init(void)
{
	
	
	


}

void somfy_serial_Tx(somfy_packet_t *pkt)
{
	packet_tx_cnt_left = pkt->packet_size;
	packets_waiting_to_transfer++;
	
	usart_writeByteArray(&usart_somfyDevice, pkt->packet, pkt->packet_size);
}

void somfy_debug_TxPgmMem(PROGMEM_STRING_T str)
{
	cli();
	while(PROGMEM_READ_BYTE(str))
	{
		usart_writeByte(&usart_somfyDebug, PROGMEM_READ_BYTE(str++));
	}
	sei();
}


void somfy_display_packet_Tx(somfy_packet_t *pkt)
{
	uint8_t i, temp, *ptr, x = 0;
	size_t dataSentSize = 0;
	
	uint8_t buffer[100];
	
	ptr = pkt->packet;
	
	dataSentSize += usart_writeString(&usart_somfyDebug, "<outgoing packet:>");
	/* This goes through the packet an converts all the values to ASCII
	 */
	for (i = 0; i < pkt->packet_size; i++)
	{
		//get the next byte in the packet
		temp = *ptr++;
		// Upper nibble
		buffer[x++] = nibble_to_ascii[(temp & 0xF0) >> 4];
		// Lower nibble
		buffer[x++] = nibble_to_ascii[temp & 0x0F];
		buffer[x++] = ' ';
	}	
	buffer[x++] = '\r';
	buffer[x++] = '\n';
	buffer[x++] = '\0';
	
	// Write the first packet
	dataSentSize += usart_writeString(&usart_somfyDebug, buffer);
	
	// Start the second packet
	dataSentSize +=usart_writeString(&usart_somfyDebug, "<outgoing packet:inverted>");
	
	// Reset the pointer to the front of the packet
	*ptr = pkt->packet;
	x = 0;
	// Run through the packet but this time invert the values
	for (i = 0; i < pkt->packet_size; i++)
	{
		//get the next byte in the packet and invert it
		temp = ~(pkt->packet[i]);
		
		// Upper nibble
		buffer[x++] = nibble_to_ascii[(temp & 0xF0) >> 4];
		// Lower nibble
		buffer[x++] = nibble_to_ascii[temp & 0x0F];
		buffer[x++] = ' ';
	}
	
	buffer[x++] = '\r';
	buffer[x++] = '\n';
	buffer[x++] = '\0';

		
	dataSentSize += usart_writeString(&usart_somfyDebug, buffer);

}



void somfyDeviceTxCB(uint8_t num)
{
	num = num;
	
	packet_tx_cnt_left--;
	
	if(packet_tx_cnt_left == 0)
	{
		packets_waiting_to_transfer--;
		
		//TODO: I need to add delay of 100ms using the using the TX_HOLD then resetting the hold to TX_IDLE
	}
}	

void somfyDeviceRxCB(uint8_t num)
{
	somfy_Rx_data(num);
/*
	uint8_t buffer[3];
	
	// Get the string of the byte
	print_char_hex(num, buffer);
	
	//print the byte on the debugger
	usart_writeString(&usart_somfyDebug, buffer);
	
	// Add a space after the byte
	usart_writeByte(&usart_somfyDebug, ' ');*/
	
}
