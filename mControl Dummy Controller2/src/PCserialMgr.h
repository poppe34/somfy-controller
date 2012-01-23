/*
 * serial_interface.h
 *
 * Created: 12/15/2011 3:36:56 PM
 *  Author: matt poppe
 */ 


#ifndef PCSERIAL_MGR_H_
#define PCSERIAL_MGR_H_

#include "masterSerialMgr.h"
#include "somfyRS485.h"
#include "DTG.h"

extern usartDescriptor_t usart_PCinter;

void PCserialInterface_init(void);
void PCserial_txCBhandler(void);
void PCserial_rxCBhandler(uint8_t num);
void PCserial_statusCallBack(uint8_t *data);

#endif /* SERIAL_INTERFACE_H_ */