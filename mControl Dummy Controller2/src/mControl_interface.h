/*
 * USBinterface.h
 *
 * Created: 12/6/2011 1:47:29 PM
 *  Author: matt poppe
 */ 


#ifndef USBINTERFACE_H_
#define USBINTERFACE_H_







typedef struct{
	uint8_t size;
	PROGMEM_STRING_T *strings;
}commands_t;

typedef struct{
	uint8_t size;
	char string[];
}string_t;

void usb_init(void);
uint8_t usb_getOutOfProgMem(char *buffer, PROGMEM_STRING_T str);
void usb_tx_prgmem_buffer(PROGMEM_STRING_T str);
void usb_matchString(void);
void usb_incoming_data_notify(void);
#endif /* USBINTERFACE_H_ */