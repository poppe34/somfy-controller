/*
 * somfySerialMgr.h
 *
 * Created: 10/26/2011 8:15:40 AM
 *  Author: matt poppe
 */ 


#ifndef SOMFYSERIALMGR_H_
#define SOMFYSERIALMGR_H_

#include "masterSerialMgr.h"
#include "somfyRS485.h"

extern usartDescriptor_t usart_somfyDevice;
extern usartDescriptor_t usart_somfyDebug;

void somfy_serial_init(void);
void somfy_serial_Tx(somfy_packet_t *pkt);
void somfy_debug_TxPgmMem(PROGMEM_STRING_T str);
void somfy_debug_Tx(const char *str, uint8_t len);
void somfy_display_packet_Tx(somfy_packet_t *pkt);
void somfyDeviceTxCB(uint8_t num);
void somfyDeviceRxCB(uint8_t num);

#endif /* SOMFYSERIALMGR_H_ */