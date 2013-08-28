/*
 * messages_and_callbacks.h
 *
 * Created: 12/15/2011 12:52:30 PM
 *  Author: matt poppe
 */ 


#ifndef MESSAGES_CALLBACKS_H_
#define MESSAGES_CALLBACKS_H_

#include "DTG.h"





extern uint8_t ctrl_move_momentary_dbg[];


/*

PROGMEM_DECLARE(const char, cmd_query[]) = "QUERY";
PROGMEM_DECLARE(const char, cmd_send[]) = "SEND";
PROGMEM_DECLARE(const char, REPLY_STATUS_OK[]) = "OK\r\n";

PROGMEM_DECLARE(const char, ctrl_stop_dbg[]) = "CTRL_STOP";
PROGMEM_DECLARE(const char, ctrl_moveTo_dbg[]) = "CTRL_MOVETO";
PROGMEM_DECLARE(const char, ctrl_moveOf_dbg[]) = "CTRL_MOVEOF";
PROGMEM_DECLARE(const char, ctrl_wink_dbg[]) = "CTRL_WINK";
*/

void dummy(void);
void cmd_QueryHandler(uint8_t *data);
void cmd_sendHandler(uint8_t *data);
void cmd_upHandler(uint8_t *data);
void cmd_downHandler(uint8_t *data);
#endif /* MESSAGES_AND_CALLBACKS_H_ */