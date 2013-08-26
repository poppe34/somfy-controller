/*
 * masterSerialMgr.h
 *
 * Created: 1/3/2012 3:37:50 PM
 *  Author: mpoppe
 */ 


#ifndef MASTERSERIALMGR_H_
#define MASTERSERIALMGR_H_

#define MASTER_SERIAL_BUFFER_SIZE 256
#define USART_CHANNEL_USARTC0		0x08A0
#define USART_CHANNEL_USARTC1		0x08B0
#define USART_CHANNEL_USARTD0		0x09A0
#define USART_CHANNEL_USARTD1		0x09B0
#define USART_CHANNEL_USARTE0		0x0AA0
#define USART_CHANNEL_USARTE1		0x0AB0
#define USART_CHANNEL_USARTF0		0x0BA0
#define USART_CHANNEL_USARTF1		0x0BB0

#define ADD_TO_BUFFER(buffer,data)		*buffer->txPoW++ = data; 
									

/********************************************************************************************************************************
			TYPEDEFs
********************************************************************************************************************************/
typedef enum tx_state
{
	TX_IDLE,
	TX_ACTIVE,
	TX_HOLD,
}tx_serial_state_t;

/*

TODO: ADD and echo bool value

*/
typedef struct usartDescrpitor 
{
	//! Channel the descriptor uses
	uint16_t channel;

	//! Set baud rate of the USART (unused in slave modes).
	uint32_t baudrate;

	//! Number of bits to transmit as a character (5 to 9).
	USART_CHSIZE_t charlength;

	//! Parity type: USART_PMODE_DISABLED_gc, USART_PMODE_EVEN_gc,
	//! USART_PMODE_ODD_gc.
	USART_PMODE_t paritytype;

	//! Number of stop bits between two characters:
	//! true: 2 stop bits
	//! false: 1 stop bit
	bool stopbits;
	
	//! RS485 transmit enable
	uint8_t RS485portPin;
	
	//! Is echo enabled
	Bool echoEnabled;
	
	//!callback after a transfer is complete
	void ( *txCallBack )( uint8_t );
	
	//! start of the Tx buffer
	uint8_t *txBuffer;
	
	//! Pointer in the the transmit pointer
	uint8_t *txBufferPtr;
	
	//! Size left in the buffer
	uint8_t txActualBufferSize;
	
	//! Point were next transmitted byte is written into buffer
	uint8_t *txPoW;
	
	//! Point in the buffer that is next to be transmitted
	uint8_t *txPoR;
	
	//!  Size of the data needed to be transfered
	uint16_t txDataSize;
	
	//! Current state of the usart transmit
	//! TX_IDLE
	//! TX_ACTIVE
	tx_serial_state_t txState;
	
	//! Rx buffer NOTE: If no receive buffer is given then the data will be reported just one
	//! at a time
	uint8_t *rxBuffer;
	
	//! Point where to write next byte in the buffer
	uint8_t *rxPoW;
	
	//! Next unread byte in the buffer 
	uint8_t *rxPoR;
	
	//! Callback after a transfer is complete
	void ( *rxCallBack )(uint8_t);
	
	//!  Size of the string that has been received
	uint16_t rxDataSize;
}usartDescriptor_t;

enum usart_index
{
#ifdef CONF_BOARD_ENABLE_USARTC0
	USARTC0_ARRAY_INDEX,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTC1
	USARTC1_ARRAY_INDEX,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTD0
	USARTD0_ARRAY_INDEX,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTD1
	USARTD1_ARRAY_INDEX,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTE0
	USARTE0_ARRAY_INDEX,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTE1
	USARTE1_ARRAY_INDEX,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTF0
	USARTF0_ARRAY_INDEX,
#endif	
#ifdef CONF_BOARD_ENABLE_USARTF1
	USARTF1_ARRAY_INDEX,
#endif
	USART_FILLER_DUMMY,	
};

typedef struct 
{
	USART_t *port;
	enum usart_index indx; 
}usart_info_t;
/**
 * \defgroup usart_group USART module (USART)
 *
 * This is a driver for configuring, enablig, disabling and use of the on-chip
 * USART.
 *
 * \section dependencies Dependencies
 *
 * The USART module depends on the following modules:
 *  - \ref sysclk_group for peripheral clock control.
 *  - \ref port_driver_group for peripheral io port control.
 *
 * @{
 */
void masterSerial_init(void);

/**
 * \defgroup usart_group USART module (USART)
 *
 * This is a driver for configuring, enabling, disabling and use of the on-chip
 * USART.
 *
 * \section dependencies Dependencies
 *
 * The USART module depends on the following modules:
 *  - \ref sysclk_group for peripheral clock control.
 *  - \ref port_driver_group for peripheral io port control.
 *
 * @{
 */
uint8_t usart_init( usartDescriptor_t *desc );
uint8_t usart_openPort( usartDescriptor_t *descr );
uint8_t usart_task(void);
size_t usart_getSizeofData(usartDescriptor_t *desc, Bool getTx);
size_t usart_writeString( usartDescriptor_t *desc, uint8_t *str );
uint8_t usart_isPortOpen(usartDescriptor_t *desc);
void usart_writeByte(usartDescriptor_t *desc, uint8_t num);
size_t usart_writeByteArray(usartDescriptor_t *desc, uint8_t *str, size_t len);
uint8_t usart_catchUpRead(usartDescriptor_t *desc);
void usart_terminateRxWithNull(usartDescriptor_t *desc);
uint8_t *usart_readBytes(usartDescriptor_t *desc, uint8_t cnt);
#endif /* MASTERSERIALMGR_H_ */