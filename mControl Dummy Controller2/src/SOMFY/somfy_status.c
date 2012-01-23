/*
 * somfy_status.c
 *
 * Created: 10/28/2011 12:49:21 PM
 *  Author: matt poppe
 */ 
#include "board.h"
#include "somfy.h"

#include <stdio.h>

#include "somfySerialMgr.h"
#include "PCserialMgr.h"

static somfy_motor_t motors[knumOfMotors];
volatile status_waiting_flags_t flags;

PROGMEM_DECLARE(const char, rx_motor_position[]) = "POST_MOTOR_POSITION\r\n";
PROGMEM_DECLARE(const char, rx_motor_status[]) = "POST_MOTOR_STATUS\r\n";
PROGMEM_DECLARE(const char, rx_motor_limits[]) = "POST_MOTOR_LIMITS\r\n";
PROGMEM_DECLARE(const char, rx_motor_rotation[]) = "POST_MOTOR_DIRECTION\r\n";
PROGMEM_DECLARE(const char, rx_motor_speed[]) = "POST_MOTOR_ROLLING_SPEED\r\n";
PROGMEM_DECLARE(const char, rx_motor_ip[]) = "POST_MOTOR_IP\r\n";
PROGMEM_DECLARE(const char, rx_dct_lock[]) = "POST_DCT_LOCK\r\n";
PROGMEM_DECLARE(const char, rx_fact_default[]) = "POST_FACTORY_DEFAULT\r\n";

PROGMEM_DECLARE(const char, tx_motor_position[]) = "GET_MOTOR_POSITION\r\n";
PROGMEM_DECLARE(const char, tx_motor_status[]) = "GET_MOTOR_STATUS\r\n";
PROGMEM_DECLARE(const char, tx_motor_limits[]) = "GET_MOTOR_LIMITS\r\n";
PROGMEM_DECLARE(const char, tx_motor_rotation[]) = "GET_MOTOR_DIRECTION\r\n";
PROGMEM_DECLARE(const char, tx_motor_speed[]) = "GET_MOTOR_ROLLING_SPEED\r\n";
PROGMEM_DECLARE(const char, tx_motor_ip[]) = "GET_MOTOR_IP\r\n";
PROGMEM_DECLARE(const char, tx_dct_lock[]) = "GET_DCT_LOCK\r\n";
PROGMEM_DECLARE(const char, tx_fact_default[]) = "GET_FACTORY_DEFAULT\r\n";

PROGMEM_DECLARE(const char, err_no_dev[]) = "status update was ran without a valid Addr\r\n";
PROGMEM_DECLARE(const char, err_unknown_status[]) = "packet returned an unknown status packet type\r\n";
	
void somfyMotor_defaults_init(void)
{

		motors[0].addr.firstByte = 0x06;
		motors[0].addr.secondByte = 0x0f;
		motors[0].addr.thirdByte = 0x0d;
		
		motors[1].addr.firstByte = 0x06;
		motors[1].addr.secondByte = 0x0f;
		motors[1].addr.thirdByte = 0x34;

		motors[2].addr.firstByte = 0x55;
		motors[2].addr.secondByte = 0x0f;
		motors[2].addr.thirdByte = 0x0d;
}

void somfy_getFullStatus(somfyAddr_t addr)
{
	/*somfy_reqMotorPos(addr);
	while(!(flags)){;}
	somfy_reqMotorStatus(addr);
	while(!(flags)){;}
	somfy_reqMotorLimits(addr);
	while(!(flags)){;}
	somfy_reqMotorDirection(addr);
	while(!(flags)){;}
	somfy_reqMotorSpeed(addr);
	while(!(flags)){;}
	somfy_reqMotorDCTlock(addr);
	while(!(flags)){;}*/
}

void somfy_reqMotorPos(somfyAddr_t addr)
{
	somfy_statusReq(addr, get_position, 0);
	
	flags.position_flag = true;
	
	//Misc Debugging features
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(tx_motor_position);
	#endif

}

void somfy_reqMotorStatus(somfyAddr_t addr)
{
	somfy_statusReq(addr, get_status, 0);

	flags.status_flag = true;
	
	//Misc Debugging features
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(tx_motor_status);
	#endif

}

void somfy_reqMotorLimits(somfyAddr_t addr)
{
	somfy_statusReq(addr, get_limits, 0);

	flags.limits_flag = true;
	
	//Misc Debugging features
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(tx_motor_limits);
	#endif
}

void somfy_reqMotorDirection(somfyAddr_t addr)
{
	somfy_statusReq(addr, get_direction, 0);
	
	flags.rotation_flag = true;
	
	//Misc Debugging features
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(tx_motor_rotation);
	#endif
}

void somfy_reqMotorSpeed(somfyAddr_t addr)
{
	somfy_statusReq(addr, get_speed, 0);
	
	flags.speed_flag = true;
	
	//Misc Debugging features
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(tx_motor_speed);
	#endif
}

void somfy_reqMotorIP(somfyAddr_t addr, uint8_t ip)
{
	somfy_statusReq(addr, get_intermediate_pos, ip);
	
	flags.ip_flag = true;
	
	//Misc Debugging features
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(tx_motor_ip);
	#endif
}

void somfy_reqMotorDCTlock(somfyAddr_t addr)
{
	somfy_statusReq(addr, get_dry_contact_lock, 1);
	
	flags.lock_flag = true;
	
	//Misc Debugging features
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(tx_dct_lock);
	#endif
}

void somfy_reqMotorFactDefaults(somfyAddr_t addr, uint8_t data)
{
	somfy_statusReq(addr, get_factory_default_status, data);
	
	flags.position_flag = true;
	
	//Misc Debugging features
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(tx_motor_position);
	#endif
}

void somfy_statusReq(somfyAddr_t addr, enum status_request req, uint8_t data)
{
	somfy_packet_t pkt;
	pkt.packet_size = 0;
	//set packet type
	pkt.msg = req;
	
	//set destination addr
	pkt.dest = addr;
	
	//set source addr
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
		
	//set the size of the packet being sent
	pkt.data_size = 0;
	
	if((req == get_intermediate_pos)||(req == get_dry_contact_lock) || (req == get_factory_default_status))
	{
		pkt.data[0] = data;
		pkt.data_size++;
	}
	//send to the device
	somfy_Tx_pkt(&pkt);	

	//Misc Debugging features

	#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
	#endif	
}

void somfy_Rx_statusUpdate(somfy_packet_t *pkt)
{

	uint8_t x, i;
	somfy_motor_t *dev = NULL;
	uint8_t string[128];

	
	for(x=0; x<knumOfMotors; x++)
	{
		if((motors[x].addr.firstByte == pkt->src.firstByte) && (motors[x].addr.secondByte == pkt->src.secondByte) && (motors[x].addr.thirdByte == pkt->src.thirdByte))
		{
			dev = &motors[x];
		}
	}
	
	if(!dev)
	{
		#ifdef DEBUG_ERR
		somfy_debug_TxPgmMem(err_no_dev);
		#endif
		return;
	}
	
	switch (pkt->msg)
	{
	case status_position:
		#ifdef DEBUG_MSG
		somfy_debug_TxPgmMem(rx_motor_position);
		#endif
		
		dev->pulsesFromUpLimit = *((motor_pulses_t *)(&pkt->data[0]));
		dev->percentFromUpLimit = pkt->data[2];
		dev->currentIP = pkt->data[3];
		
		reportBackStatus(dev->percentFromUpLimit, &pkt->src);
		break;
	
	case status_motor_status:
		#ifdef DEBUG_MSG
		somfy_debug_TxPgmMem(rx_motor_status);
		#endif
		break;
	
	case status_limits:
	//Gather the limits
		dev->downLimit = *((motor_pulses_t *)(&pkt->data[2]));
		dev->upLimit = *((motor_pulses_t *)(&pkt->data[0]));
	
		#ifdef DEBUG_MSG
		somfy_debug_TxPgmMem(rx_motor_limits);
		#endif
		
		//build the string to report back the limits
		sprintf(string, "LIMITS %i %i %i %i %i", pkt->src.firstByte, pkt->src.secondByte, pkt->src.thirdByte, dev->upLimit, dev->downLimit);
		
		#ifdef DEBUG_STATUS
		usart_writeString(&usart_somfyDebug, string);
		#endif
		
		//send the string to the PC interface
		usart_writeString(&usart_PCinter, string);
		break;
		
	case status_direction:
		
		dev->MLD = pkt->data[0];
		
		#ifdef DEBUG_MSG
		somfy_debug_TxPgmMem(rx_motor_rotation);
		#endif
		
		//build the string to report back the limits
		sprintf(string, "DIRECTION %i %i %i %i", pkt->src.firstByte, pkt->src.secondByte, pkt->src.thirdByte, dev->MLD);
		
		#ifdef DEBUG_STATUS
		usart_writeString(&usart_somfyDebug, string);
		#endif
		
		//send the string to the PC interface
		usart_writeString(&usart_PCinter, string);
		break;
		
		break;
		
	case status_speed:
		dev->upSpeed = pkt->data[0];
		dev->downSpeed = pkt->data[1];
		dev->slowSpeed = pkt->data[2];
		
		#ifdef DEBUG_MSG
		somfy_debug_TxPgmMem(rx_motor_speed);
		#endif
		
		//build the string to report back the limits
		sprintf(string, "DIRECTION %i %i %i %i %i %i", pkt->src.firstByte, pkt->src.secondByte, pkt->src.thirdByte, dev->upSpeed, dev->downSpeed, dev->slowSpeed);
		
		#ifdef DEBUG_STATUS
		usart_writeString(&usart_somfyDebug, string);
		#endif
		
		//send the string to the PC interface
		usart_writeString(&usart_PCinter, string);
		
		break;
		
	case status_intermediate_pos:
		i = pkt->data[0];
		dev->ip[i].pulses =  *((motor_pulses_t *)(&pkt->data[1]));
		dev->ip[i].percent_down =  pkt->data[3];
	
		#ifdef DEBUG_MSG
		somfy_debug_TxPgmMem(rx_motor_position);
		#endif
	

		//build the string to report back the limits
		sprintf(string, "DIRECTION %i %i %i %i %i %i", pkt->src.firstByte, pkt->src.secondByte, pkt->src.thirdByte, i, dev->ip[i].pulses, dev->ip[i].percent_down);
		
		#ifdef DEBUG_STATUS
		usart_writeString(&usart_somfyDebug, string);
		#endif
		
		//send the string to the PC interface
		usart_writeString(&usart_PCinter, string);
		
		break;
	
	
		break;
		
	case status_dry_contact_lock:
		
		#ifdef DEBUG_MSG
		somfy_debug_TxPgmMem(rx_dct_lock);
		#endif
		
		//build the string to report back the limits
		sprintf(string, "DRYCONTACTS %i %i %i %i %i %i", pkt->src.firstByte, pkt->src.secondByte, pkt->src.thirdByte, i, pkt->data[0], pkt->data[4]);
		
		#ifdef DEBUG_STATUS
		usart_writeString(&usart_somfyDebug, string);
		#endif
		
		//send the string to the PC interface
		usart_writeString(&usart_PCinter, string);
		break;
		
	case status_factory_default:
		#ifdef DEBUG_MSG
		somfy_debug_TxPgmMem(rx_fact_default);
		#endif
		
		//build the string to report back the limits
		sprintf(string, "DRYCONTACTS %i %i %i %i %i %i", pkt->src.firstByte, pkt->src.secondByte, pkt->src.thirdByte, i, pkt->data[0], pkt->data[1]);
		
		#ifdef DEBUG_STATUS
		usart_writeString(&usart_somfyDebug, string);
		#endif
		
		//send the string to the PC interface
		usart_writeString(&usart_PCinter, string);
		break;
		break;
		
	default:
		#ifdef DEBUG_ERR
		somfy_debug_TxPgmMem(err_unknown_status);	
		#endif	
		break;

		
	
	}
}