/*
 * masterSerialMgr.c
 *
 * Created: 1/3/2012 3:35:23 PM
 *  Author: mpoppe
 */ 

#include <stdio.h>
#include "ioport.h"
#include "board.h"

#include "serial.h"
#include "usart.h"
#include "masterSerialMgr.h"


#include "conf_usart_serial.h"
#include "string.h"
#include "util/delay.h"

#if NUM_OF_USART_CHANNELS == 0
	#error 'USART is not allowed'
#endif


void usart_txHandler(uint8_t i);
void usart_print(usartDescriptor_t *desc);
usart_info_t *usart_getPort(uint16_t desc);

/*TODO: ADD a if def to all the following*/
usart_info_t CHANNEL_USARTC0 = {&USARTC0, USARTC0_ARRAY_INDEX};
usart_info_t CHANNEL_USARTC1 = {&USARTC1, USARTC1_ARRAY_INDEX};
usart_info_t CHANNEL_USARTD0 = {&USARTD0, USARTD0_ARRAY_INDEX};
usart_info_t CHANNEL_USARTD1 = {&USARTD1, USARTD1_ARRAY_INDEX};
//usart_info_t CHANNEL_USARTE0 = {&USARTE0, USARTE0_ARRAY_INDEX};
//usart_info_t CHANNEL_USARTE1 = {&USARTE1, USARTE1_ARRAY_INDEX};
//usart_info_t CHANNEL_USARTF0 = {&USARTF0, USARTF0_ARRAY_INDEX};
//usart_info_t CHANNEL_USARTF1 = {&USARTF1, USARTF1_ARRAY_INDEX};
	
//*********************
// \BRIEF This is the holders of the used USART channels
//***********************
usartDescriptor_t *usartDescirptors[NUM_OF_USART_CHANNELS] = {
#ifdef CONF_BOARD_ENABLE_USARTC0
	NULL,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTC1
	NULL,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTD0
	NULL,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTD1
	NULL,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTE0
	NULL,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTE1
	NULL,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTF0
	NULL,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTF1
	NULL,
#endif	
};



/*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
uint8_t usart_openPort( usartDescriptor_t *descr )
{
	//Make sure we receive a valid descriptor
	if( descr == NULL )
		return 0;// no descriptor sent
	
	//Get the descriptor number	
	uint8_t x = (uint8_t)((usart_getPort( descr->channel ))->indx);
	
	if(x == USART_FILLER_DUMMY)
		return 0; //unknown descriptor channel
	if(usartDescirptors[ x ] != NULL)
		return 0;// channel currently open
	
	//init the usart port and return true if successful	
	if(!usart_init(descr)){return 0;}
	
	//Set the interrupt level to low
	usart_set_rx_interrupt_level((usart_getPort(descr->channel)->port), USART_RXCINTLVL_LO_gc);
	usart_set_tx_interrupt_level((usart_getPort(descr->channel)->port), USART_TXCINTLVL_LO_gc);
	//put the opened device in the descriptors list	
	usartDescirptors[ x ] = descr;
	
	sei();
	
	return 1;

}

/*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
uint8_t usart_init( usartDescriptor_t *desc )
{
	//Need to add the different possible modes that I can have in SPI
	usart_rs232_options_t options;
	USART_t *port = NULL;
	
	options.baudrate = desc->baudrate;
	options.charlength = desc->charlength;
	options.paritytype = desc->paritytype;
	options.stopbits = desc->stopbits;
	
	port = (usart_getPort(desc->channel))->port;
	
	//! If there is a supplied Tx buffer point the point of write and reads	
	if(desc->txBuffer)
	{
		desc->txPoW = desc->txBuffer;
		desc->txPoR = desc->txBuffer;
	}

	//! If there is a supplied Rx buffer point the point of write and reads	
	if(desc->txBuffer)
	{
		desc->rxPoW = desc->rxBuffer;
		desc->rxPoR = desc->rxBuffer;
	}

	
	usart_init_rs232(port, &options);
	
	return 1;
}


size_t usart_getSizeofData(usartDescriptor_t *desc, Bool getTx)
{
	size_t bufferLen;
	
	if(!desc){return 0;}
		
	if(getTx)
	{
		// use the distance between PoR and PoW to give the size left of the buffer
		bufferLen = desc->txPoR - desc->txPoW;
	
		//if the POR is less than POW add the size of the buffer to get the actual size of the buffer
		if(bufferLen >= desc->txDataSize || bufferLen == 0)
		{
			bufferLen += desc->txDataSize;
		}
	}	
	else
	{
				// use the distance between PoR and PoW to give the size left of the buffer
		bufferLen = desc->rxPoR - desc->rxPoW;
	
		//if the POR is less than POW add the size of the buffer to get the actual size of the buffer
		if(bufferLen >= desc->rxDataSize || bufferLen == 0)
		{
			bufferLen += desc->rxDataSize;
		}
	}	
}
/*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
uint8_t usart_task(void)
{
	usartDescriptor_t *currentDesc;
	do 
	{
	
		for(uint8_t x = 0; x < NUM_OF_USART_CHANNELS; x++)
		{
			currentDesc = usartDescirptors[x];
			//Check for open ports
			if( currentDesc != NULL )
			{
				if (currentDesc->txPoW != currentDesc->txPoR)
				{
					if( currentDesc->txState == TX_IDLE)	
					{
						usart_print(currentDesc);
					}
				}
			
			}
		}
		
	}while(1);
	
	return 0;
}
 
 /*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
size_t usart_writeString( usartDescriptor_t *desc, uint8_t *str )
{
	return(usart_writeByteArray(desc, str, (strlen(str))));
}

void usart_writeByte(usartDescriptor_t *desc, uint8_t num)
{
	if(!usart_isPortOpen(desc)){return;}
	
	ADD_TO_BUFFER(desc, num);
	
	if(desc->txPoW > (desc->txBuffer + desc->txDataSize))
	{
		desc->txPoW = desc->txBuffer;
	}
}

size_t usart_writeByteArray(usartDescriptor_t *desc, uint8_t *str, size_t len)
{
	size_t sizeOfString, bufferLen, sizeToEdge;
	
	if(!usart_isPortOpen(desc)){return 0;}
	
	//get the overall size of the string in question
	sizeOfString = len;
	
	bufferLen = usart_getSizeofData(desc, true);
	
	//if there is not enough room in the buffer for the string exit without adding it to the buffer
	if(bufferLen < len){return 0;}
	
	//check to see if adding the string to the buffer will cause our PoW to go out of bounds with the buffer
	sizeToEdge = (desc->txBuffer + desc->txDataSize) - desc->txPoW;
	
/*	if we go out of bounds in our buffer break the memcpy to two parts
		1. to fill to the end of the buffer
		2. to fill from the beginning again*/
	if(sizeToEdge < len)
	{
		//fill to the edge
		memcpy(desc->txPoW, str, sizeToEdge);
		desc->txPoW = desc->txBuffer;//reset the pointer to the front of the buffer
		str += sizeToEdge;// move the pointer of the string to the uncopied data
		sizeOfString = len - sizeToEdge;// adjust the size of the string to leftover length
	}		
	//copy rest/all of the data to the buffer
	memcpy(desc->txPoW, str, sizeOfString);
	
	//move the PoW to end of the newly added string
	desc->txPoW += sizeOfString;

	return len;
}

uint8_t *usart_readBytes(usartDescriptor_t *desc, uint8_t cnt)
{
	uint8_t *oldPoR = desc->rxPoR;
	if((cnt > (usart_getSizeofData(desc, false)) || cnt == 0))
	{
		desc->rxPoR = desc->rxPoW;
	}
	else
	{
		desc->rxPoR += cnt;
	}
	return oldPoR;
}
/*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
uint8_t usart_isPortOpen(usartDescriptor_t *desc)
{
	usart_info_t *info = usart_getPort(desc->channel);

	//Validate the channel exists
	if(!info){return 0;}
	//Validate that channel is open
	if(!(usartDescirptors[info->indx])){return 0;}
		
	return 1;
}

/*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
void usart_print(usartDescriptor_t *desc)
{
	if(!desc)
	return;
	
	USART_t *port;
	
	
	if(desc->txPoR != desc->txPoW)
	{
		//hold the interrupt so we don't get in a case where we say we active when we are not
		cli();
		port = ((usart_info_t *)usart_getPort(desc->channel))->port;
		
		desc->txState = TX_ACTIVE;
		
		if(desc->RS485portPin)
		{
			ioport_set_pin_high(desc->RS485portPin);
		}
		
		
		//Send the Char
		usart_putchar(port, *desc->txPoR);
		
	
		//startup interrupts again
		sei();
	}
}
/*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
void usart_terminateRxWithNull(usartDescriptor_t *desc)
{
	*desc->rxPoW++ = '\0';
}

/*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
usart_info_t *usart_getPort(uint16_t chan)
{
	switch( chan )
	{
#ifdef CONF_BOARD_ENABLE_USARTC0
		case( USART_CHANNEL_USARTC0 ):
			return(&CHANNEL_USARTC0);
		break;
#endif
#ifdef CONF_BOARD_ENABLE_USARTC1
		case ( USART_CHANNEL_USARTC1 ):
			return(&CHANNEL_USARTC1);
		break;
#endif
#ifdef CONF_BOARD_ENABLE_USARTD0
		case( USART_CHANNEL_USARTD0 ):
			return(&CHANNEL_USARTD0);
		break;
#endif
#ifdef CONF_BOARD_ENABLE_USARTD1
		case ( USART_CHANNEL_USARTD1 ):
			return(&CHANNEL_USARTD1);
		break;
#endif
#ifdef CONF_BOARD_ENABLE_USARTE0
		case( USART_CHANNEL_USARTE0 ):
			return( &CHANNEL_USARTE0 );
		break;
#endif
#ifdef CONF_BOARD_ENABLE_USARTE1
		case ( USART_CHANNEL_USARTE1 ):
			return( &CHANNEL_USARTE1 );
		break;
#endif
#ifdef CONF_BOARD_ENABLE_USARTF0
		case( USART_CHANNEL_USARTF0 ):
			return( &CHANNEL_USARTF0 );
		break;
#endif
#ifdef CONF_BOARD_ENABLE_USARTF1
		case ( USART_CHANNEL_USARTF1 ):
			return( &CHANNEL_USARTF1 );
		break;
#endif
		default:
		return NULL;
	}		
}

/*
##########################################################################
*
*   brief:	This function will make some sanity check then initialize then 
*			open the serial port
*
*   param: Descriptor of the port that is going to be opened
*
*
##########################################################################
*/
void usart_txHandler(uint8_t i)
{
	usartDescriptor_t *desc = usartDescirptors[i];
	//TODO I need to add some kind of reset if the data size needs to be reset to zero
	// right now I will just have buffers of size 128
	
	//If the POR is outside the buffer reset it
	if((desc->txPoR - desc->txBuffer) >= desc->txDataSize)
	{
		desc->txPoR = desc->txBuffer;
	}
	
	if(desc->RS485portPin)
	{
		ioport_toggle_pin(desc->RS485portPin);
	}
	//execute the callback for a transmit done
	if(desc->txCallBack)
	{
		desc->txCallBack(*desc->txPoR);
	}
	
	//increment transmit read
	desc->txPoR++;
	
	//set the state of transmit back to ideal
	desc->txState = TX_IDLE;
}

uint8_t usart_readByte(usartDescriptor_t *desc)
{
	return (*desc->rxPoR++);
}

uint8_t usart_readBuffer(usartDescriptor_t *desc, uint8_t *buf)
{
	
}

uint8_t usart_catchUpRead(usartDescriptor_t *desc)
{
	desc->rxPoR = desc->rxPoW;
}

/*
TODO: I need to create protection in this handler to keep POW hitting the POR This will
prevent the buffer from overfilling*/

uint8_t usart_rxHandler(uint8_t i)
{
	uint8_t value;
	USART_t *port;
	usartDescriptor_t *currentDesc = usartDescirptors[i];
	
	//find the port that has received data and fetch the data
	port = usart_getPort(currentDesc->channel)->port;
	value = usart_getchar(port);
	
	//Echo value if enabled
	if(currentDesc->echoEnabled)
	{
		usart_writeByte(currentDesc, value);
	}
	
	//! If there is a receive buffer then add the value to the buffer
	//! if there is no buffer then just report back the read char
	if(currentDesc->rxBuffer)
	{
		//add the read bit into the buffer
		*currentDesc->rxPoW++ = value;
	
		//check if we are at the end of the buffer.  If we are at the end reset the pointer to the beginning
		if((currentDesc->rxPoW -  currentDesc->rxBuffer) >= currentDesc->rxDataSize)
		{
			currentDesc->rxPoW = currentDesc->rxBuffer;
		}
	}

	//execute the callback
	if(currentDesc->rxCallBack)
	{
		currentDesc->rxCallBack(value);
	}
}

#ifdef CONF_BOARD_ENABLE_USARTC0
ISR(USARTC0_TXC_vect)
{
	usart_txHandler(USARTC0_ARRAY_INDEX);
}

ISR(USARTC0_RXC_vect)
{
	usart_rxHandler(USARTC0_ARRAY_INDEX);	
}
#endif

#ifdef CONF_BOARD_ENABLE_USARTC1
ISR(USARTC1_TXC_vect)
{
	usart_txHandler(USARTC1_ARRAY_INDEX);
}

ISR(USARTC1_RXC_vect)
{
	usart_rxHandler(USARTC1_ARRAY_INDEX);
}
#endif

#ifdef CONF_BOARD_ENABLE_USARTD0
ISR(USARTD0_TXC_vect)
{
	usart_txHandler(USARTD0_ARRAY_INDEX);
}

ISR(USARTD0_RXC_vect)
{
	usart_rxHandler(USARTD0_ARRAY_INDEX);
}
#endif

#ifdef CONF_BOARD_ENABLE_USARTD1
ISR(USARTD1_TXC_vect)
{
	usart_txHandler(USARTD1_ARRAY_INDEX);
}

ISR(USARTD1_RXC_vect)
{
	usart_rxHandler(USARTD1_ARRAY_INDEX);	
}
#endif

#ifdef CONF_BOARD_ENABLE_USARTE0
ISR(USARTE0_TXC_vect)
{
	usart_txHandler(USARTE0_ARRAY_INDEX);
}

ISR(USARTE0_RXC_vect)
{
	usart_rxHandler(USARTE0_ARRAY_INDEX);
}
#endif

#ifdef CONF_BOARD_ENABLE_USARTE1
ISR(USARTE1_TXC_vect)
{
	usart_txHandler(USARTE1_ARRAY_INDEX);
}

ISR(USARTE1_RXC_vect)
{
	usart_rxHandler(USARTE1_ARRAY_INDEX);
}
#endif

#ifdef CONF_BOARD_ENABLE_USARTF0
ISR(USARTF0_TXC_vect)
{
	usart_txHandler(USARTF0_ARRAY_INDEX);
}

ISR(USARTF0_RXC_vect)
{
	usart_rxHandler(USARTF0_ARRAY_INDEX);
}
#endif

#ifdef CONF_BOARD_ENABLE_USARTF1
ISR(USARTF0_TXC_vect)
{
	usart_txHandler(USARTF1_ARRAY_INDEX);
}

ISR(USARTF0_RXC_vect)
{
	usart_rxHandler(USARTF1_ARRAY_INDEX);
}
#endif
