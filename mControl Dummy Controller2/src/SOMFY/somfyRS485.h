/*
 * somfyRS485.h
 *
 * Created: 10/21/2011 9:58:30 AM
 *  Author: matt poppe
 */


#ifndef SOMFYRS485_H_
#define SOMFYRS485_H_

#define kdefaultDevAddr {0x33, 0xff, 0xff}

#define kSendPacket 0x02
#define kReceivedPacket 0x20

#define kmaxPacketSize 16
#define kminPacketSize 11

#define kmoveinMomentModeSize	14
#define ksetMotorLimitsSize 15
#define ksetMotorDirectSize 12
#define ksetMotorSpeedSize 14
#define ksetMotorIP 15
#define ksetDCTlockSize 14
#define ksetFactoryDefaultSize 12


#define newPacket(pkt) somfy_packet_t *pkt = (somfy_packet_t *)malloc(sizeof(somfy_packet_t))
			
#define dnc 0xff
#define dataLength(pkt) pkt - kminPacketSize

extern uint8_t nibble_to_ascii[];

enum setting_types{
	set_limits = 0x11,
	set_motor_rotation = 0x12,
	set_rolling_speed = 0x13,
	set_intermediate_position = 0x15,
	set_dry_contact_lock = 0x17,
	set_to_factory_settings = 0x1f,
};
enum control_types{
	move_momentary_mode = 0x01,
	stop_movement = 0x02,
	move_to_pos = 0x03,
	move_to_relative_pos = 0x04,
	feedback = 0x05,
};
enum status_request{
	get_position = 0x0c,
	get_status = 0x0e,
	get_limits = 0x21,
	get_direction = 0x22,
	get_speed = 0x23,
	get_intermediate_pos = 0x25,
	get_dry_contact_lock = 0x27,
	get_factory_default_status = 0x2f,
};
enum status_report{
	status_position = 0x0d,
	status_motor_status = 0x0f,
	status_limits = 0x31,
	status_direction = 0x32,
	status_speed = 0x33,
	status_intermediate_pos = 0x35,
	status_dry_contact_lock = 0x37,
	status_factory_default = 0x3f,
};

enum incoming_stage{
	WAITING_FOR_NEW_MSG,
	WAITING_FOR_PACKET_SIZE,
	WAITING_FOR_DIRECTION,
	WAITING_FOR_SRC_ADDR,
	WAITING_FOR_DEST_ADDR,
	WAITING_FOR_DATA,
	WAITING_FOR_CHECKSUM,
	PACKET_COMPLETE,
};
typedef struct somfyAddr{
	uint8_t firstByte;
	uint8_t secondByte;
	uint8_t thirdByte;
}somfyAddr_t;

typedef uint16_t motor_position_t;
typedef uint16_t motor_pulses_t;

/****************************
// SET_MOTOR_LIMITS
****************************/
enum limit_type{
	limit_delete = 0x00,
	limit_set_current_pos = 0x01,
	limit_set_specific_pos = 0x02,
	limit_set_10ms_jog = 0x04,
	limit_set_pulse_jog = 0x05,
};
enum limit_direction{
	down_limit = 0x00,
	up_limit = 0x01,
};

typedef uint16_t pos_pulseCount_t;

struct set_motor_limits{
	enum limit_type type;
	enum limit_direction dir;
	pos_pulseCount_t pos_pulseCount;
};

/****************************
// SET_MOTOR_ROTATION_DIRECTION
****************************/
enum rotation_dir{
	standard_rot = 0x00,
	reverse_rot = 0x01,
};

/****************************
// SET_MOTOR_ROLLING_SPEED
****************************/
typedef uint8_t rolling_speed_t;

struct rolling_speed{
	rolling_speed_t up;
	rolling_speed_t down;
	rolling_speed_t slow;
};

/****************************
// SET_MOTOR_IP
****************************/
enum IP_action{
	ip_delete = 0x00,
	ip_setCurrentPos = 0x01,
	ip_setFromUpLimitPulses = 0x02,
	ip_setFromUpLimitPercent = 0x03,
};

struct set_IP {
	enum IP_action action;
	U8 IP_num;
	pos_pulseCount_t pos_count;

};

/****************************
// SET_DCT_LOCK
****************************/
enum DCT_locks_status{
	DCT_unlocked = 0x00,
	DCT_locked = 0x01,
};

struct DCT_lock{
	enum DCT_locks_status status;
	U8 num;
	U8 priority;
};

/****************************
// SET_FACTORY_DEFAULT
****************************/
enum default_list{
	all_settings = 0x00,
	clear_all_groups = 0x01,
	delete_all_limits = 0x11,
	restore_rotation = 0x12,
	restore_speed = 0x13,
	delete_IPs = 0x15,
	delete_locks = 0x17,
};


/****************************
// Device Info
****************************/

typedef struct {
	somfyAddr_t addr;
	enum rotation_dir dir;
	struct rolling_speed speed;

}somfy_device_t;


/*****************************
// Packet setup
****************************/

typedef struct{
	uint8_t packet[kmaxPacketSize];
    uint8_t data[kmaxPacketSize - kminPacketSize];
	uint16_t msg;
	uint8_t data_size;
	uint8_t packet_size;
	uint8_t dir;
	somfyAddr_t src;
	somfyAddr_t dest;
	uint16_t checksum;
}somfy_packet_t;

/********************************************************
// SOMFY device parameters
********************************************************/

enum motor_state{
	motor_stopped = 0x00,
	motor_running = 0x01,
	motor_blocked = 0x02,
	motor_locked  = 0x03, 
};

enum motor_last_dir{
	last_up = 0x00,
	last_down = 0x01,
};

enum motor_LSA{
	LSA_internal = 0x00,
	LSA_network = 0x01,
	LSA_DCT	= 0x02,
};

enum motor_CLA{
	CLA_target_reached = 0x00,
	CLA_explicit_command = 0x01,
	CLA_wink = 0x02,
	CLA_limits_not_set = 0x10,
	CLA_IP_not_set = 0x11,
	CLA_polarity_not_checked = 0x12,
	CLA_in_config_mode = 0x13,
	CLA_obstacle_dectected = 0x20,
	CLA_OC_protection = 0x21,
	CLA_thermal_protection = 0x22,
};

typedef struct {
	motor_pulses_t pulses;
	uint8_t percent_down;
}IP_t;

typedef struct somfy_motor{
	
	somfyAddr_t			addr;
	enum motor_state	motorState;
	enum motor_last_dir	dir;
	enum motor_LSA		LSA;
	enum motor_CLA		CLA;
	enum motor_last_dir MLD;
	motor_pulses_t		upLimit;
	motor_pulses_t		downLimit;
	motor_pulses_t		pulsesFromUpLimit;
	uint8_t				percentFromUpLimit;
	uint8_t				currentIP;
	uint8_t				upSpeed;
	uint8_t				downSpeed;
	uint8_t				slowSpeed;
	IP_t				ip[16];	
}somfy_motor_t;

typedef struct{
	bool position_flag:1;
	bool status_flag:1;
	bool limits_flag:1;
	bool rotation_flag:1;
	bool speed_flag:1;
	bool ip_flag:1;
	bool lock_flag:1;
	bool factory_def_flag:1;
}status_waiting_flags_t;

/*****************************
// control 
****************************/
enum control_direction{
	move_up = 0x00,
	move_down = 0x01,
	move_cancelled = 0x02,
};

enum control_speed{
	up_speed = 0x00,
	down_speed = 0x01,
	slow_speed = 0x02,
};

enum control_moveTo
{
	move_to_down_limit = 0x00,
	move_to_up_limit = 0x01,
	move_to_IP = 0x02,
	move_to_pos_pulses = 0x03,
	move_to_pos_percent = 0x04,
};

enum control_moveRelative
{
	move_to_next_IP_down = 0x00,
	move_to_next_IP_up = 0x01,
	move_to_jog_down_pulses = 0x02,
	move_to_jog_up_pulses = 0x03,
	move_to_jog_down_time = 0x04,
	move_to_jog_up_time = 0x05,	
};

typedef struct control_move
{
	enum control_direction dir;
	enum control_speed	speed;
	enum control_moveTo moveTo;
	enum control_moveRelative moveRelative;
	uint16_t value;
}control_move_t;

/*****************************
// Prototypes
****************************/
void somfy_buildPkt(somfy_packet_t *pkt);
void somfy_Tx_pkt(somfy_packet_t *pkt);
void somfy_getChecksum(somfy_packet_t *pkt);
void somfy_decodePkt(somfy_packet_t *pkt);
void somfy_Rx_data(uint8_t data);
void somfy_decodeRxPkt(void);
bool somfy_checkCheckSum(void);
void somfy_displayRxPktWithInvert(somfy_packet_t *pkt);
void reportBackStatus(uint8_t posPercent, somfyAddr_t *addr);

#endif /* SOMFYRS485_H_ */