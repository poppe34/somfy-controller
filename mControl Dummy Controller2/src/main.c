/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * AVR Software Framework (ASF).
 */
#include <asf.h>
#include <conf_usart_serial.h>

#include "masterSerialMgr.h"
#include "somfySerialMgr.h"
#include "PCserialMgr.h"
#include "tempSerialMgr.h"

#include "somfy.h"

void tempSerial_init(void);


int main (void)
{
	sysclk_init();
	
	board_init();
	pmic_init();
	sei();
	pmic_enable_level(PMIC_LVL_LOW);
	
	usart_openPort(&usart_somfyDevice);
	usart_openPort(&usart_somfyDebug);
	usart_openPort(&usart_PCinter);
	usart_openPort(&usart_temp);

	somfyMotor_defaults_init();
	somfyAddr_t dest = {0x06, 0x0f, 0x0d};
//	somfy_control_wink(dest);
	somfy_reqMotorPos(dest);
	// usart run loop start
	return usart_task();
	
}