/**
 * \file
 *
 * \brief STK600 with the RC064X routing card board header file.
 *
 * This file contains definitions and services related to the features of the
 * STK600 board with the routing card for 64-pin TQFP AVR XMEGA devices.
 *
 * To use this board, define BOARD=STK600_RC064X.
 *
 * Copyright (C) 2010 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#ifndef _STK600_RC064_H_
#define _STK600_RC064_H_

#include "compiler.h"


//! Number of LEDs.
#define LED_COUNT   8

/*! \name GPIO Connections of LEDs. To use these defines, connect the STK600
 * PORTE connector to the LEDs connector.
 */
//! @{
#  define LED0_GPIO   IOPORT_CREATE_PIN(PORTE,0)
#  define LED1_GPIO   IOPORT_CREATE_PIN(PORTE,1)
#  define LED2_GPIO   IOPORT_CREATE_PIN(PORTE,2)
#  define LED3_GPIO   IOPORT_CREATE_PIN(PORTE,3)
#  define LED4_GPIO   IOPORT_CREATE_PIN(PORTE,4)
#  define LED5_GPIO   IOPORT_CREATE_PIN(PORTE,5)
#  define LED6_GPIO   IOPORT_CREATE_PIN(PORTE,6)
#  define LED7_GPIO   IOPORT_CREATE_PIN(PORTE,7)
//! @}


/*! \name GPIO Connections of Push Buttons. To use these defines, connect the
 * STK600 PORTF connector to the SWITCHES connector.
 */
//! @{
#  define GPIO_PUSH_BUTTON_0	IOPORT_CREATE_PIN(PORTF,0)
#  define GPIO_PUSH_BUTTON_1	IOPORT_CREATE_PIN(PORTF,1)
#  define GPIO_PUSH_BUTTON_2	IOPORT_CREATE_PIN(PORTF,2)
#  define GPIO_PUSH_BUTTON_3	IOPORT_CREATE_PIN(PORTF,3)
#  define GPIO_PUSH_BUTTON_4	IOPORT_CREATE_PIN(PORTF,4)
#if !XMEGA_A3B
#  define GPIO_PUSH_BUTTON_5	IOPORT_CREATE_PIN(PORTF,5)
#endif
#  define GPIO_PUSH_BUTTON_6	IOPORT_CREATE_PIN(PORTF,6)
#  define GPIO_PUSH_BUTTON_7	IOPORT_CREATE_PIN(PORTF,7)
//! @}

/*! \name SPI Connections of the AT45DBX Data Flash Memory. To use these defines,
 * connect :
 * - PC4 to /CS pin on DataFlash connector
 * - PC7 to SCK pin on DataFlash connector
 * - PC6 to SO pin on DataFlash connector
 * - PC5 to SI pin on DataFlash connector
 */
//! @{
#define AT45DBX_SPI           &SPIC
#define AT45DBX_CS            IOPORT_CREATE_PIN(PORTC,4)  // CS as output
#define AT45DBX_MASTER_SS     IOPORT_CREATE_PIN(PORTC,4)  // SS as output and overlayed with CS
#define AT45DBX_MASTER_SCK    IOPORT_CREATE_PIN(PORTC,7)  // SCK as output
#define AT45DBX_MASTER_MOSI   IOPORT_CREATE_PIN(PORTC,5)  // MOSI as output
#define AT45DBX_MASTER_MISO   IOPORT_CREATE_PIN(PORTC,6)  // MISO as input
#define AT45DBX_SPI_CLK_MASK  SYSCLK_PORT_C

//! @}

/*! \name SPI Connections of the AT25FBX Data Flash Memory. To use these defines,
 * connect :
 * - PC4 to /CS pin on DataFlash connector
 * - PC7 to SCK pin on DataFlash connector
 * - PC6 to SO pin on DataFlash connector
 * - PC5 to SI pin on DataFlash connector
 */
//! @{
#define AT25DFX_SPI           &SPIC
#define AT25DFX_CS            IOPORT_CREATE_PIN(PORTC,4)  // CS as output
#define AT25DFX_MASTER_SS     IOPORT_CREATE_PIN(PORTC,4)  // SS as output and overlayed with CS
#define AT25DFX_MASTER_SCK    IOPORT_CREATE_PIN(PORTC,7)  // SCK as output
#define AT25DFX_MASTER_MOSI   IOPORT_CREATE_PIN(PORTC,5)  // MOSI as output
#define AT25DFX_MASTER_MISO   IOPORT_CREATE_PIN(PORTC,6)  // MISO as input
#define AT25DFX_SPI_CLK_MASK  SYSCLK_PORT_C

//! @}

//! \name Somfy RS485 interface
//@{
#define SOMFY_INTERFACE_TX				USARTC0_TXD
#define SOMFY_INTERFACE_RX				USARTC0_RXD
#define SOMFY_DEBUG_INTERFACE_TX		USARTC1_TXD
#define SOMFY_DEBUG_INTERFACE_RX		USARTC1_RXD	
#define PC_INTERFACE_TX					USARTD0_TXD
#define PC_INTERFACE_RX					USARTD0_RXD
#define TEMP_INTERFACE_TX				USARTD1_TXD
#define TEMP_INTERFACE_RX				USARTD1_RXD
#define SOMFY_DEVICE_RS485_PIN			IOPORT_CREATE_PIN(PORTC, 0)
//@}

/*! \name ADD Serial D0
 */
//@{
#define CONF_BOARD_ENABLE_USARTC0
#define CONF_BOARD_ENABLE_USARTC1
#define CONF_BOARD_ENABLE_USARTD0
#define CONF_BOARD_ENABLE_USARTD1
//@}


/*! \name External oscillator
 */
//@{
#define BOARD_XOSC_HZ          8000000
#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
#define BOARD_XOSC_STARTUP_US  500000
//@}


#endif  // _STK600_RC064_H_
