/******************************************************************************
 *  Copyright (C) 2016 by Alija Sabic
 *
 *  Project: Plant Irrigation System
 *
 *****************************************************************************/

/**
 *	@file tpic6a595_hal.c
 *
 *  @author Alija Sabic, ew13b510, BEW5A
 *
 *  @date Jan 2016
 *
 *  @brief Hardware Abstraction Layer for TPIC6A595 for valve control
 */

/**
 * tpic6a595_hal.c
 *
 *  Created on: 11.01.2016
 *      Author: Alija
 */
#include "tpic6a595_hal.h"

/******************************************************************************
 * @function initValveControl()
 *
 * @brief Initalize peripherals for valve control via TPIC6A595.
 *
 * This function is called to initalize valve control chip TPIC6A595
 * communication and GPIO ports used to control TPIC6A595.
 *
 *
 */
void
initValveControl(void)
{
	/**
	 * Enable Peripherals GPIO Port and SSI Module connected to TPIC6A595.
	 *
	 * PORT D 	> PIN PD1 >> SSI2XDAT0 (SERIN)
	 * 			> PIN PD3 >> SSI2CLK
	 *
	 * PORT D	> SSI2
	 */
	SysCtlPeripheralEnable(TPIC6A595_SPI_PERIPH_GPIO);
	SysCtlPeripheralEnable(TPIC6A595_SPI_PERIPH_SPI);

	/**
	 * Enable Peripherals Port connected to control pins of TPIC6A595.
	 *
	 * PORT H	> PIN PH0 >> nSRCLR
	 * 			> PIN PH1 >> nG
	 * 			> PIN PH2 >> RCK
	 */
	SysCtlPeripheralEnable(TPIC6A595_CNTR_PERIPH);

	/**
	 * Activate alternate pin function SSI2CLK, SSI2SDO
	 */
	GPIOPinConfigure(TPIC6A595_SRCK_PINF);
	GPIOPinConfigure(TPIC6A595_SERIN_PINF);

	/**
	 * Configure GPIO pins connected to TPIC6A595 control pins as output.
	 */
	GPIOPinTypeGPIOOutput(TPIC6A595_CNTR_PORT, 	TPIC6A595_nSRCLS
										  	  | TPIC6A595_nG
											  | TPIC6A595_RCK);

	/**
	 * Set Type of SSI communication 2 wire (MOSI | CLK) and which
	 * pins will be used
	 */
	GPIOPinTypeSSI(TPIC6A595_SPI_PORT, TPIC6A595_SRCK | TPIC6A595_SERIN);

	/**
	 * Sets the data clock source for the SSI Module connected to TPIC6A595
	 */
	SSIClockSourceSet(TPIC6A595_SPI_MODULE, SSI_CLOCK_SYSTEM);

	/**
	 * Configure the SSI2 Communication with TPIC6A595
	 */
	SSIConfigSetExpClk(	TPIC6A595_SPI_MODULE, 	 /**<SSI Port (SSI3)         */
						g_ui32SysClock, 		 /**<µController SysClock    */
						SSI_FRF_MOTO_MODE_0, 	 /**<Communication Mode      */
						SSI_MODE_MASTER, 		 /**<µController = Master    */
						100000, 				 /**<Bit rate (6 Mhz)        */
						8);						 /**<Data packet width       */

	/**
	 * Enable the SSI2 module
	 */
	SSIEnable(TPIC6A595_SPI_MODULE);

	/**
	 * Turn off all valves.
	 */
	switchValve(TPIC6A595_CLOSE_VALVES);
}

/******************************************************************************
 * @function writeCommandValveControl()
 *
 * @brief Send command to valve control cip TPIC6A595.
 *
 * This function is called to send command to valve control chip TPIC6A595.
 *
 * @param ui8Valve specifies the valve to be driven.
 *
 * 		  ui8Valve can take on the following values:
 *
 *		  TPIC6A595_CLOSE_VALVES - Close all valves.
 * 		  TPIC6A595_VALVE_1	- Pins of TPIC6A595 connected to Valve 1
 * 		  TPIC6A595_VALVE_2	- Pins of TPIC6A595 connected to Valve 2
 * 		  TPIC6A595_VALVE_3	- Pins of TPIC6A595 connected to Valve 3
 * 		  TPIC6A595_VALVE_4	- Pins of TPIC6A595 connected to Valve 4
 */
static void
writeCommandValveControl(uint8_t ui8Valve)
{
	/**
	 * Check the arguments.
	 */
	ASSERT( (ui8Valve == TPIC6A595_CLOSE_VALVES) ||
			(ui8Valve == TPIC6A595_VALVE_2) ||
			(ui8Valve == TPIC6A595_VALVE_2) ||
			(ui8Valve == TPIC6A595_VALVE_3) ||
			(ui8Valve == TPIC6A595_VALVE_4));

	/**
	 * RCK (Storage Register Clock) logical "0".
	 */
	GPIOPinWrite(TPIC6A595_CNTR_PORT, TPIC6A595_RCK , 0);

	/**
	 * Delay.
	 */
	SysCtlDelay(TPIC6A595_DELAY);

	/**
	 * nSRCLR Logical "1". Enable data transfer to storage register.
	 */
	GPIOPinWrite(TPIC6A595_CNTR_PORT, TPIC6A595_nSRCLS , TPIC6A595_nSRCLS);

	/**
	 * Delay.
	 */
	SysCtlDelay(TPIC6A595_DELAY);

	/**
	 * nG Output buffers logical "0". Drain outputs (connected to Valves) off.
	 */
	GPIOPinWrite(TPIC6A595_CNTR_PORT, TPIC6A595_nG , TPIC6A595_nG);

	/**
	 * Delay.
	 */
	SysCtlDelay(TPIC6A595_DELAY);
	/**
	 * Transmit data to TPIC6A595 via SSI.
	 */
	SSIDataPut(TPIC6A595_SPI_MODULE, ui8Valve);

	/**
	 * Wait for the transmit buffer to become empty.
	 */
	while( (SSI_TXEOT & SSIIntStatus(TPIC6A595_SPI_MODULE, false)) !=
			SSI_TXEOT);

	/**
	 * RCK (Storage Register Clock) transfers data to output buffers when
	 * nSRCLS is logical "1".
	 */
	GPIOPinWrite(TPIC6A595_CNTR_PORT, TPIC6A595_RCK , TPIC6A595_RCK);

	/**
	 * Delay.
	 */
	SysCtlDelay(TPIC6A595_DELAY);

	/**
	 * RCK (Storage Register Clock) logical "0" after data transfer to
	 * output buffers completed to restore previous condition.
	 */
	GPIOPinWrite(TPIC6A595_CNTR_PORT, TPIC6A595_RCK , 0);

	/**
	 * Delay.
	 */
	SysCtlDelay(TPIC6A595_DELAY);

	/**
	 * nG logical "0". Data from storage register is transparent to output
	 * buffers.
	 */
	//#G
	GPIOPinWrite(TPIC6A595_CNTR_PORT, TPIC6A595_nG , 0);

	/**
	 * Delay.
	 */
	SysCtlDelay(TPIC6A595_DELAY);
}

/******************************************************************************
 * @function writeCommandValveControl()
 *
 * @brief Switch valves via TPIC6A595.
 *
 * This function is called to switch valves via TPIC6A595.
 *
 * @param ui8Valve specifies the valve to be driven.
 *
 * 		  ui8Valve can take on the following values:
 *
 *		  TPIC6A595_CLOSE_VALVES - Close all valves.
 * 		  TPIC6A595_VALVE_1	- Pins of TPIC6A595 connected to Valve 1
 * 		  TPIC6A595_VALVE_2	- Pins of TPIC6A595 connected to Valve 2
 * 		  TPIC6A595_VALVE_3	- Pins of TPIC6A595 connected to Valve 3
 * 		  TPIC6A595_VALVE_4	- Pins of TPIC6A595 connected to Valve 4
 */
void
switchValve(uint8_t ui8Valve)
{
	/**
	 * Send command twice to switch respective valve on/off.
	 */
	writeCommandValveControl(ui8Valve);
	writeCommandValveControl(ui8Valve);
}
