/******************************************************************************
 *  Copyright (C) 2016 by Alija Sabic
 *
 *  Project: Plant Irrigation System
 *
 *****************************************************************************/

/**
 *	@file tpic6a595_hal.h
 *
 *  @author Alija Sabic, ew13b510, BEW5A
 *
 *  @date Jan 2016
 *
 *  @brief Hardware Abstraction Layer for TPIC6A595 for valve control
 */

/**
 * tpic6a595_hal.h
 *
 *  Created on: 11.01.2016
 *      Author: Alija
 */

#ifndef TPIC6A595_HAL_H_
#define TPIC6A595_HAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"

/**
 * System clock rate in Hz.
 */
extern uint32_t g_ui32SysClock;

/**
 * Delay time for TPIC6A595
 */
#define TPIC6A595_DELAY				10		/**<TPIC6A595 delay time.		 */

/**
 * Commands related to valve control.
 */
#define TPIC6A595_CLOSE_VALVES		0x00	/**<Command to close all valves  */
#define TPIC6A595_VALVE_4 			0x03	/**<Pins of TPIC6A595 connected
												to Valve 1	 				 */
#define TPIC6A595_VALVE_3 			0x0c	/**<Pins of TPIC6A595 connected
												to Valve 2	 				 */
#define TPIC6A595_VALVE_2 			0x30	/**<Pins of TPIC6A595 connected
												to Valve 3	 				 */
#define TPIC6A595_VALVE_1 			0xc0	/**<Pins of TPIC6A595 connected
												to Valve 4					 */



/**
 * Peripherals connected to TPIC6A595
 *
 * GPIO PORT D	>>	SSI SERIN, SRCK
 * GPIO PORT H	>>	TPIC6A595 Control
 * SSI2 MODULE
 */
#define TPIC6A595_CNTR_PERIPH		SYSCTL_PERIPH_GPIOH	/**<Peripheral
													connected control pins 	 */
#define TPIC6A595_SPI_PERIPH_GPIO	SYSCTL_PERIPH_GPIOD	/**<Peripheral
													connected SPI pins 		 */
#define TPIC6A595_SPI_PERIPH_SPI	SYSCTL_PERIPH_SSI2	/**<Peripheral
													connected SPI module	 */

/**
 * Ports and Modules connected to TPIC6A595
 *
 * GPIO PORT D	>>	SSI SERIN, SRCK
 * GPIO PORT H	>>	TPIC6A595 Control
 * SSI2 MODULE
 */
#define TPIC6A595_CNTR_PORT			GPIO_PORTH_BASE	/**<Port of connected
														control pins 		 */
#define TPIC6A595_SPI_PORT			GPIO_PORTD_BASE	/**<Port of connected
														SPI pins 			 */
#define TPIC6A595_SPI_MODULE		SSI2_BASE		/**<Module base address
 	 	 	 	 	 	 	 	 	 	 	 	 	 	connected SPI module */

/**
 * Control Pins connected to TPIC6A595.
 */
#define TPIC6A595_nSRCLS 			GPIO_PIN_0		/**<Pin connected to
														nSRCLS 	(TPIC6A595)  */
#define TPIC6A595_nG				GPIO_PIN_1		/**<Pin connected to
														nG		(TPIC6A595)	 */
#define TPIC6A595_RCK				GPIO_PIN_2		/**<Pin connected to
														RCK		(TPIC6A595)	 */

/**
 * Pins connected to SSI2 Module for 2 wire SPI communciation with TPIC6A595
 */
#define TPIC6A595_SERIN				GPIO_PIN_1		/**<Pin connected to
														SERIN 	(TPIC6A595)  */
#define TPIC6A595_SRCK				GPIO_PIN_3		/**<Pin connected to
														SRCK 	(TPIC6A595)  */

/**
 * Pin functions used for SPI communciation
 */
#define TPIC6A595_SERIN_PINF		GPIO_PD1_SSI2XDAT0	/**<MOSI pin function
 	 	 	 	 	 	 	 	 	 	 connected to pin SERIN (TPIC6A595)	 */
#define TPIC6A595_SRCK_PINF			GPIO_PD3_SSI2CLK	/**<SRCK pin function
 	 	 	 	 	 	 	 	 	 	 connected to pin SRCK (TPIC6A595)	 */

/******************************************************************************
 * Prototype to initalize peripherals for valve control via TPIC6A595.
 *
 */
void initValveControl(void);

/******************************************************************************
 * Prototype to switch valves via TPIC6A595.
 *
 */
void switchValve(uint8_t ui8Valve);

#endif /* TPIC6A595_HAL_H_ */
