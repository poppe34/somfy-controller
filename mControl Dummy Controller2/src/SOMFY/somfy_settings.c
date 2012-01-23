/*
 * somfy_setting.c
 *
 * Created: 10/21/2011 11:39:09 AM
 *  Author: matt poppe
 */ 

#include "board.h"

#include "somfy.h"

PROGMEM_DECLARE(const char, set_motor_limits_dbg[]) = "SET_MOTOR_LIMITS\r\n";
PROGMEM_DECLARE(const char, set_factory_defaults_dbg[]) = "SET_FACTORY_DEFAULT\r\n";
PROGMEM_DECLARE(const char, set_motor_dir_dbg[]) = "SET_MOTOR_DIRECTION\r\n";
PROGMEM_DECLARE(const char, set_roll_speed_dbg[]) = "SET_MOTOR_ROLLING_SPEED\r\n";
PROGMEM_DECLARE(const char, set_motor_ip_dbg[]) = "SET_MOTOR_IP\r\n";
PROGMEM_DECLARE(const char, set_DCT_lock_dbg[]) = "SET_DCT_LOCK\r\n";


void send_factory_default(somfyAddr_t addr, enum default_list list)
{
	somfy_packet_t pkt;
	pkt.msg = set_to_factory_settings;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;
	
	//set the size of the packet being sent
	pkt.packet_size = ksetFactoryDefaultSize;
	
	*(pkt.data) = list;
	pkt.data_size = 1;
	
	somfy_Tx_pkt(&pkt);	

#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(set_factory_defaults_dbg);
#endif
#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
#endif	
}

void send_motor_limits(somfyAddr_t addr,struct set_motor_limits *limits)
{
	somfy_packet_t pkt;
	pkt.msg = set_limits;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;
	
	//set the size of the packet being sent
	pkt.packet_size = ksetMotorLimitsSize;
	
	pkt.data[0] = limits->type;
	pkt.data[1] = limits->dir;
	
	pkt.data[2] = (limits->pos_pulseCount & 0xff);
	pkt.data[3] = ((limits->pos_pulseCount>> 8) & 0xff);
	
	pkt.data_size = 4;
	
	somfy_Tx_pkt(&pkt);	

#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(set_motor_limits_dbg);
#endif
#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
#endif	
}


void send_motor_direction(somfyAddr_t addr,enum rotation_dir dir)
{
	somfy_packet_t pkt;
	pkt.msg = set_motor_rotation;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;
	
	//set the size of the packet being sent
	pkt.packet_size = ksetMotorDirectSize;
	
	pkt.data[0] = dir;
	
	pkt.data_size = 1;
	
	somfy_Tx_pkt(&pkt);	

#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(set_motor_dir_dbg);
#endif
#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
#endif	
}

void send_motor_speed(somfyAddr_t addr, struct rolling_speed *speed)
{
	somfy_packet_t pkt;
	pkt.msg = set_rolling_speed;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;
	
	//set the size of the packet being sent
	pkt.packet_size = ksetMotorSpeedSize;
	
	pkt.data[0] = speed->up;
	pkt.data[1] = speed->down;
	pkt.data[2] = speed->slow;
	
	pkt.data_size = 3;
	
	somfy_Tx_pkt(&pkt);	

#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(set_roll_speed_dbg);
#endif
#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
#endif	
}
void send_DCT_lock(somfyAddr_t addr, struct DCT_lock *dct)
{
	somfy_packet_t pkt;
	pkt.msg = set_dry_contact_lock;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;
	
	//set the size of the packet being sent
	pkt.packet_size = ksetDCTlockSize;
	
	pkt.data[0] = dct->status;
	pkt.data[1] = dct->num;
	pkt.data[2] = dct->priority;
	
	pkt.data_size = 3;
	
	somfy_Tx_pkt(&pkt);	

#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(set_DCT_lock_dbg);
#endif
#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
#endif	
}

void send_IP(somfyAddr_t addr, struct set_IP *ip)
{
		somfy_packet_t pkt;
	pkt.msg = set_dry_contact_lock;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;
	
	//set the size of the packet being sent
	pkt.packet_size = ksetDCTlockSize;
	
	pkt.data[0] = ip->action;
	pkt.data[1] = ip->IP_num;
	
	pkt.data[2] = ip->pos_count & 0xff;
	pkt.data[3] = (ip->pos_count >> 8) & 0xff;
	
	pkt.data_size = 4;
	
	somfy_Tx_pkt(&pkt);	

#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(set_DCT_lock_dbg);
#endif
#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
#endif	
}