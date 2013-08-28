/*
 * somfy.h
 *
 * Created: 10/26/2011 8:17:09 AM
 *  Author: matt poppe
 */ 


#ifndef SOMFY_H_
#define SOMFY_H_

#include "somfyRS485.h"
#include "conf_somfyRS485.h"
#include "conf_usart_serial.h"
#include "somfySerialMgr.h"

//somfy_settings.c
void send_factory_default(somfyAddr_t addr, enum default_list list);
void send_motor_limits(somfyAddr_t addr,struct set_motor_limits *limits);
void send_motor_direction(somfyAddr_t addr,enum rotation_dir dir);
void send_motor_speed(somfyAddr_t addr, struct rolling_speed *speed);
void send_DCT_lock(somfyAddr_t addr, struct DCT_lock *dct);
void send_IP(somfyAddr_t addr, struct set_IP *ip);

//somfy_status.c
void somfyMotor_defaults_init(void);
void somfy_statusReq(somfyAddr_t addr, enum status_request req, uint8_t data);
void somfy_reqMotorPos(somfyAddr_t addr);
void somfy_reqMotorStatus(somfyAddr_t addr);
void somfy_reqMotorLimits(somfyAddr_t addr);
void somfy_reqMotorDirection(somfyAddr_t addr);
void somfy_reqMotorSpeed(somfyAddr_t addr);
void somfy_reqMotorIP(somfyAddr_t addr, uint8_t ip);
void somfy_reqMotorFactDefaults(somfyAddr_t addr, uint8_t data);
void somfy_reqMotorDCTlock(somfyAddr_t addr);
void somfy_Rx_statusUpdate(somfy_packet_t *pkt);

//somfy_control.c
void somfy_control_wink(somfyAddr_t addr);
void somfy_control_stop(somfyAddr_t addr, control_move_t *move);
void somfy_control_moveAbsPos(somfyAddr_t addr, control_move_t *move);
void somfy_control_moveRelPos(somfyAddr_t addr, control_move_t *move);
void somfy_control_moveMomentary(somfyAddr_t addr, control_move_t *move);
#endif /* SOMFY_H_ */