/*
 * conf_somfyRS485.h
 *
 * Created: 10/21/2011 10:40:02 AM
 *  Author: matt poppe
 */ 


#ifndef CONF_SOMFYRS485_H_
#define CONF_SOMFYRS485_H_

#define knumOfMotors 3

// used to display the somfy messages name
#define DEBUG_MSG

//used to display transmitted packets to the motors
#define DEBUG_PKT_TX

//used to display received packets from the motors
#define DEBUG_PKT_RX

//used to display the status updates of the device
#define DEBUG_STATUS

//used to display error messages in my code or in rx/tx messages
#define DEBUG_ERR

//used to display incoming messages from PC interface
//#define DEBUG_FROM_PC
#endif /* CONF_SOMFYRS485_H_ */