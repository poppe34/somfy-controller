/*
 * somfyRS485.c
 *
 * Created: 10/21/2011 10:19:16 AM
 *  Author: matt poppe
 */
#include <stdio.h>

#include "board.h"
#include "serial.h"
#include "somfy.h"

#include "DTG.h"
#include "conf_somfyRS485.h"
#include "somfySerialMgr.h"
#include "PCserialMgr.h"

static somfy_packet_t incoming_pkt;
static bool incoming_init_flag = false;
static uint8_t *data_pointer = 0;
static uint8_t *incomingPtr;
static uint16_t running_Checksum = 0;
static enum incoming_stage currentPktStage = WAITING_FOR_NEW_MSG;
static uint8_t incomingBuffer[kmaxPacketSize];

void somfy_Tx_pkt(somfy_packet_t *pkt)
{
    somfy_buildPkt(pkt);
	somfy_serial_Tx(pkt);
}

void somfy_Rx_data(uint8_t data)
{
	running_Checksum += data;
	
	switch(currentPktStage){
		case WAITING_FOR_NEW_MSG:
			//reset the pointer of the packet
			data_pointer = incoming_pkt.packet;
			
			//put the data in the packet... The data still has not been inverted
			*data_pointer++ = data;
			
			//put the inverted data in the msg tag of the structure
			incoming_pkt.msg = ~data & 0x00ff;
			
			//TODO check that it is valid msg
		//increment the stage for next data byte
		currentPktStage++;
		
		break;
		case WAITING_FOR_PACKET_SIZE:
			
			*data_pointer++ = data;
			incoming_pkt.packet_size = ~data;
			incoming_pkt.data_size = incoming_pkt.packet_size - kminPacketSize;
			
			if(incoming_pkt.packet_size < 11 || incoming_pkt.packet_size > 16)
			{
#ifdef DEBUG_ERR
				usart_writeString(&usart_somfyDebug, "Incoming Packet out of size out of Range\r\n");

#endif	
			}
		
		//increment the stage for next data byte
		currentPktStage++;	
		
		break;
		case WAITING_FOR_DIRECTION:
			*data_pointer++ = data;
			incoming_pkt.dir = ~data;
			
			//increment the stage for next data byte
			currentPktStage++;	
			
			if(incoming_pkt.dir != kReceivedPacket)
			{
#ifdef DEBUG_ERR
				usart_writeString(&usart_somfyDebug, "Incoming Packet direction is invalid\r\n");

#endif				
			}
		break;
		case WAITING_FOR_SRC_ADDR:
			*data_pointer++ = data;
			
			if((data_pointer-1) == &incoming_pkt.packet[5])
			{
				incoming_pkt.src.firstByte = ~incoming_pkt.packet[5];
				incoming_pkt.src.secondByte = ~incoming_pkt.packet[4];
				incoming_pkt.src.thirdByte = ~incoming_pkt.packet[3];
				
				//increment the stage for next data byte
				currentPktStage++;	
			}
		break;
		case WAITING_FOR_DEST_ADDR:
			*data_pointer++ = data;
			
			if((data_pointer-1) == &incoming_pkt.packet[8])
			{
				incoming_pkt.dest.firstByte = ~incoming_pkt.packet[6];
				incoming_pkt.dest.secondByte = ~incoming_pkt.packet[7];
				incoming_pkt.dest.thirdByte = ~incoming_pkt.packet[8];
				//increment the stage for next data byte
				currentPktStage++;
			}
			//If there is no data do an extra increment in the stages
			if(incoming_pkt.data_size == 0)
			{
				currentPktStage++;
			}

		break;
		case WAITING_FOR_DATA:
			*data_pointer++ = data;

			
			if(data_pointer == &incoming_pkt.packet[(9+incoming_pkt.data_size)])
			{
				for(uint8_t x = 0; x<incoming_pkt.data_size; x++)
				{
					incoming_pkt.data[x] = ~incoming_pkt.packet[(9+x)];
				}
				//increment the stage for next data byte
				currentPktStage++;	
			}
		break;
		case WAITING_FOR_CHECKSUM:
			*data_pointer++ = data;
			running_Checksum -= data;
			if(data_pointer == &incoming_pkt.packet[incoming_pkt.packet_size])
			{
				MSB(incoming_pkt.checksum) = incoming_pkt.packet[incoming_pkt.packet_size - 2];
				LSB(incoming_pkt.checksum) = incoming_pkt.packet[incoming_pkt.packet_size - 1];
				if(incoming_pkt.checksum != running_Checksum)
				{
					uint8_t buff[100];
					sprintf(buff, "Checksum did not match %i != %i\r\n", incoming_pkt.checksum, running_Checksum);
					usart_writeString(&usart_somfyDebug, buff);
				}
				currentPktStage = WAITING_FOR_NEW_MSG;
		
#ifdef DEBUG_PKT_RX
				somfy_displayRxPktWithInvert(&incoming_pkt);
#endif
				//Send the fully transmitted and decoded packet on the update the status
				somfy_Rx_statusUpdate(&incoming_pkt);
				running_Checksum = 0;
			}

		break;
		
		default:
		break;
		
		
	}
}	

void somfy_Rx_packetHandler(somfy_packet_t *pkt)
{
	//I am using this function to send the packet to the proper somfy function
}

void somfy_buildPkt(somfy_packet_t *pkt)
{

    uint8_t *ptr = pkt->packet;
    
    
	//Packet Type byte 0
	*ptr++ = ~(pkt->msg);
    
    //Packet size byte 1
	pkt->packet_size = ((pkt->data_size + kminPacketSize));
    *ptr++ = ~(pkt->packet_size);
    
    //Packet Set packet as an output packet
    //byte 2
    *ptr++ = ~(kSendPacket);
    
    //Packet source addr bytes 3-5
	
	*ptr++ = ~(LSB2(pkt->src));
	*ptr++ = ~(LSB1(pkt->src));
	*ptr++ = ~(LSB0(pkt->src));
    
    //Packet Dest addr bytes 6-8
	*ptr++ = ~(LSB2(pkt->dest));
	*ptr++ = ~(LSB1(pkt->dest));
	*ptr++ = ~(LSB0(pkt->dest));
    
    //Packet Data
    for(uint8_t x = 0;x<pkt->data_size;x++)
	{
        *ptr++ = ~(pkt->data[x]);
    }
    
    somfy_getChecksum(pkt);
    
    //Packet Checksum
    *ptr++ = MSB(pkt->checksum);
    *ptr++ = LSB(pkt->checksum);
	
	
}

/*\brief Decodes the incoming packet*/
void somfy_decodeRxPkt(void)
{
	uint8_t *dataPtr = incomingBuffer;
	somfy_packet_t pkt;
	
	//Get the message of the packet
	pkt.msg = *dataPtr++;
	
	//! Get the size of the packet
	pkt.packet_size = *dataPtr++;
	
	//! Get the direction of the packet
	pkt.dir = *dataPtr++;
	
	//! Get the Source address
	pkt.src.firstByte = *dataPtr++;
	pkt.src.secondByte = *dataPtr++;
	pkt.src.thirdByte = *dataPtr++;
	
	//! Get the Dest
	
	incoming_pkt.packet_size = incoming_pkt.packet[1];
	
    if(somfy_checkCheckSum())
	{
		switch(incoming_pkt.packet[0])
		{
			case(move_momentary_mode):
			{
				//! check if the length is correct
				if(incoming_pkt.packet_size == kmoveinMomentModeSize)
				{
					//somfy_control_moveMomentary(incoming_pkt.)
				}
			}
		}
	}	
}
void somfy_displayRxPktWithInvert(somfy_packet_t *pkt)
{
	
	uint8_t i, temp, x = 0;
	size_t dataSentSize;
	uint8_t buffer[100];

	cli();	
	uint8_t *ptr = pkt->packet;
	
	usart_writeString(&usart_somfyDebug, "<incoming packet:\>");
	/* This goes through the packet an converts all the values to ASCII
	 */
	for (i = 0; i < pkt->packet_size; i++)
	{
		//get the next byte in the packet
		temp = ~(*ptr);
		ptr++;
		// Upper nibble
		buffer[x++] = nibble_to_ascii[(temp & 0xF0) >> 4];
		// Lower nibble
		buffer[x++] = nibble_to_ascii[temp & 0x0F];
		buffer[x++] = ' ';
	}	
	buffer[x++] = '\r';
	buffer[x++] = '\n';
	buffer[x++] = '\0';

	usart_writeString(&usart_somfyDebug, buffer);
			
	sei();
}
void somfy_msgHandler(somfy_packet_t *pkt)
{
	
}

/*\brief Checks if the check sum is valid*/
bool somfy_checkCheckSum(void)
{
	return(running_Checksum == incoming_pkt.checksum);
}

/*\brief Calculates the check some of the packet*/
void somfy_getChecksum(somfy_packet_t *pkt)
{
	U16 sum = 0;
	
	U8 *ptr = pkt->packet;
	
	for(U8 x=0; x<(pkt->packet_size-2);x++)
	{
		sum += (*ptr);
		ptr++;
	}
	pkt->checksum = (sum);

}

void reportBackStatus(uint8_t posPercent, somfyAddr_t *addr)
{
		uint8_t buf[50];
		uint8_t len, temp = 50;
	
		len = sprintf(buf, "OK %i %i %i LEVEL %i \r\n", addr->firstByte, addr->secondByte, addr->thirdByte, posPercent);

#ifdef DEBUG_STATUS
		usart_writeString(&usart_somfyDebug, buf);
#endif


		usart_writeString(&usart_PCinter, buf);
}
