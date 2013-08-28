/*
 * somfy_control.c
 *
 * Created: 11/2/2011 3:20:34 PM
 *  Author: Matt Poppe
 */ 
#include "board.h"

#include "somfy.h"



PROGMEM_DECLARE(const char, ctrl_move_momentary_dbg[]) = "CTRL_MOVE\r\n";
PROGMEM_DECLARE(const char, ctrl_stop_dbg[]) = "CTRL_STOP\r\n";
PROGMEM_DECLARE(const char, ctrl_moveTo_dbg[]) = "CTRL_MOVETO\r\n";
PROGMEM_DECLARE(const char, ctrl_moveOf_dbg[]) = "CTRL_MOVEOF\r\n";
PROGMEM_DECLARE(const char, ctrl_wink_dbg[]) = "CTRL_WINK\r\n";

void somfy_control_moveMomentary(somfyAddr_t addr, control_move_t *move)
{
	somfy_packet_t pkt;
	pkt.msg = move_momentary_mode;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr;
	pkt.src = src;
	pkt.dest = addr;
	
	//set the size of the packet being sent
	pkt.data[0] = move->dir;
	//set the position
	pkt.data[1] = (move->value & 0xff);
	pkt.data[2] = move->speed;
	
	pkt.data_size = 3;
	
	somfy_Tx_pkt(&pkt);	

#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(ctrl_move_momentary_dbg);
#endif
#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
#endif		
}

void somfy_control_stop(somfyAddr_t addr, control_move_t *move)
{
	somfy_packet_t pkt;
	pkt.msg = stop_movement;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;
	
	//add dummy data
	pkt.data[0] = dnc;
	pkt.data_size = 1;
	
	somfy_Tx_pkt(&pkt);
	
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(ctrl_stop_dbg);
	#endif
	#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
	#endif	
}

void somfy_control_moveAbsPos(somfyAddr_t addr, control_move_t *move)
{
	somfy_packet_t pkt;
	pkt.msg = move_to_pos;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; 
	pkt.src = src;
	
	pkt.dest = addr;
	
	//set the size of the packet being sent
	pkt.data[0] = move->moveTo;

	//set the position
	pkt.data[1] = (move->value & 0xff);
	pkt.data[2] = (move->value >> 8) & 0xff;
	
	//dummy value
	pkt.data[3] = dnc;
	
	pkt.data_size = 4;
	
	somfy_Tx_pkt(&pkt);
	
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(ctrl_moveTo_dbg);
	#endif
	#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
	#endif	
}

void somfy_control_moveRelPos(somfyAddr_t addr, control_move_t *move)
{
	somfy_packet_t pkt;
	pkt.msg = move_to_relative_pos;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;

	//set the size of the packet being sent
	pkt.data[0] = move->moveRelative;
	
	//set the position
	pkt.data[1] = (move->value & 0xff);
	pkt.data[2] = (move->value >> 8) & 0xff;
	
	//dummy value
	pkt.data[3] = dnc;
	
	pkt.data_size = 4;
	
	somfy_Tx_pkt(&pkt);
	
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(ctrl_moveOf_dbg);
	#endif
	#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
	#endif	
}



void somfy_control_wink(somfyAddr_t addr)
{
	somfy_packet_t pkt;
	pkt.msg = feedback;
	
	//set the needed address
	struct somfyAddr src = kdefaultDevAddr; pkt.src = src;
	pkt.dest = addr;
	
	//transmit the packet
	somfy_Tx_pkt(&pkt);
	
	#ifdef DEBUG_MSG
	somfy_debug_TxPgmMem(ctrl_wink_dbg);
	#endif
	#ifdef DEBUG_PKT_TX
	somfy_display_packet_Tx(&pkt);
	#endif	
	
}
