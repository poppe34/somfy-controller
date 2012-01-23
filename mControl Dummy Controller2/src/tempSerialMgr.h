/*
 * tempSerialMgr.h
 *
 * Created: 1/10/2012 1:41:19 PM
 *  Author: mpoppe
 */ 


#ifndef TEMPSERIALMGR_H_
#define TEMPSERIALMGR_H_

#include "masterSerialMgr.h"
#include "somfyRS485.h"
#include "DTG.h"

extern usartDescriptor_t usart_temp;

void temp_txCBhandler(void);
void temp_rxCBhandler(uint8_t num);




#endif /* TEMPSERIALMGR_H_ */