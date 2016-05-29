/******************************************************************************
 *  Copyright (C) 2016 by Alija Sabic
 *
 *  Project: Plant Irrigation System
 *
 *****************************************************************************/

/**
 *	@file boost_drv8848_hal.c
 *
 *  @author Alija Sabic, ew13b510, BEW5A
 *
 *  @date Jan 2016
 *
 *  @brief Hardware Abstraction Layer for Boosterpack DRV8848
 */

/*
 * boost_drv8848_hal.c
 *
 *  Created on: 11.01.2016
 *      Author: Alija
 */
#include "boost_drv8848_hal.h"

/******************************************************************************
 * @function initPumpControl()
 *
 * @brief Initalize peripherals for pump control via Boosterpack DRV8848.
 *
 * This function is called to initalize the PWM module and control pins used
 * for pump control via Boosterpack DRV8848.
 *
 * @param ui8DutyCycle specifies inital duty cycle in % (<=100%).
 *
 * @return None.
 *
 */
void
initPumpControl(uint8_t ui8DutyCycle)
{
	/**
	 * Check the arguments.
	 */
	ASSERT(ui8DutyCycle > 0);
	ASSERT(ui8DutyCycle <= 100);

	/**
	 * Enable Peripheral connected to PWM6 and PWM7 and PWM module
	 */
	SysCtlPeripheralEnable(BOOST_DRV8848_PWM_PERIPH_GPIO);
	SysCtlPeripheralEnable(BOOST_DRV8848_PWM_PERIPH_PWM);

	/**
	 * Enable Peripheral connected to Boosterpack DRV8848 control
	 */
	SysCtlPeripheralEnable(BOOST_DRV8848_CNTR_PERIPH);

	/**
	 * Configure clock source of PWM module
	 */
	PWMClockSet(BOOST_DRV8848_PWM_MODULE,PWM_SYSCLK_DIV_1);

	/**
	 * Activate alternate pin function M0PWM6, M0PWM7
	 */
	GPIOPinConfigure(BOOST_DRV8848_AIN1_PINF);
	GPIOPinConfigure(BOOST_DRV8848_AIN2_PINF);

	/**
	 * Configure used GPIO pins connected to Boosterpack DRV8848
	 * control pins as output.
	 */
	GPIOPinTypeGPIOOutput(BOOST_DRV8848_CNTR_PORT, BOOST_DRV8848_nSLEEP);

	/**
	 * Set Type of PWM and which pins will be used
	 */
	GPIOPinTypePWM(BOOST_DRV8848_PWM_PORT,	BOOST_DRV8848_AIN1 |
											BOOST_DRV8848_AIN2);

	/**
	 * Configure the PWM generator for count down mode with immediate updates
	 * to the parameters.
	 */
	PWMGenConfigure(BOOST_DRV8848_PWM_MODULE, PWM_GEN_3, PWM_GEN_MODE_DOWN |
														 PWM_GEN_MODE_DBG_RUN);

	/**
	 * Set the PWM frequency.
	 */
	PWMGenPeriodSet(BOOST_DRV8848_PWM_MODULE, PWM_GEN_3, BOOST_DRV8848_50kHZ);

	/**
	 * Set the pulse width of PWM7/AIN1 for provided duty cycle.
	 */
	PWMPulseWidthSet(BOOST_DRV8848_PWM_MODULE,
					 BOOST_DRV8848_AIN1_OUT,
					 BOOST_DRV8848_50kHZ*ui8DutyCycle/100);

	/**
	 * Enable the outputs. Due to the fact that pump can be only driven in
	 * one direction, only BOOST_DRV8848_AIN1 output is enabled.
	 */
	PWMOutputState(BOOST_DRV8848_PWM_MODULE, BOOST_DRV8848_AIN1_OUT_BIT, true);

	/**
	 * Deactivate sleep mode of Boosterpack DRV8848
	 */
	GPIOPinWrite(BOOST_DRV8848_CNTR_PORT, BOOST_DRV8848_nSLEEP ,
										  BOOST_DRV8848_nSLEEP);

	/**
	 * Start the timers in generator 3.
	 */
	PWMGenEnable(BOOST_DRV8848_PWM_MODULE, PWM_GEN_3);
}

/******************************************************************************
 * @function deinitPumpControl()
 *
 * @brief Deinitalize peripherals for pump control via Boosterpack DRV8848.
 *
 * This function is called to deinitalize the PWM module and control pins used
 * for pump control via Boosterpack DRV8848 and restore previous state.
 *
 * @return None.
 *
 */
void
deinitPumpControl(void)
{
	/**
	 * Enable Peripheral connected to PWM6 and PWM7 and PWM module
	 */
	SysCtlPeripheralDisable(BOOST_DRV8848_PWM_PERIPH_GPIO);
	SysCtlPeripheralDisable(BOOST_DRV8848_PWM_PERIPH_PWM);

	/**
	 * Enable Peripheral connected to Boosterpack DRV8848 control
	 */
	SysCtlPeripheralDisable(BOOST_DRV8848_CNTR_PERIPH);
}

/******************************************************************************
 * @function initPumpControl()
 *
 * @brief Set new duty cycle of pump control via Boosterpack DRV8848.
 *
 * This function is called to configure the duty cycle of the PWM module.
 *
 * @param ui8DutyCycle specifies desired duty cycle in % (<=100%).
 *
 * @return None.
 *
 */
void
setDutyCycle(uint8_t ui8DutyCycle)
{
	/**
	 * Check the arguments.
	 */
	ASSERT(ui8DutyCycle > 0);
	ASSERT(ui8DutyCycle <= 100);

	/**
	 * Enable the PWM Outputs.
	 */
	PWMOutputState(BOOST_DRV8848_PWM_MODULE, BOOST_DRV8848_AIN1_OUT_BIT |
											 BOOST_DRV8848_AIN1_OUT_BIT, true);

	/**
	 * Set the pulse width of AIN1 for desired duty cycle.
	 */
	PWMPulseWidthSet(BOOST_DRV8848_PWM_MODULE,
			 	 	 BOOST_DRV8848_AIN1_OUT,
					BOOST_DRV8848_50kHZ*ui8DutyCycle/100);

	/**
	 * Start the timers in generator 3.
	 */
	PWMGenEnable(BOOST_DRV8848_PWM_MODULE, PWM_GEN_3);
}

/******************************************************************************
 * @function stopPump()
 *
 * @brief Stop PWM generation via Boosterpack DRV8848.
 *
 * This function is called to pause/stop PWM output generation on Boosterpack
 * DRV8848.
 *
 * @return None.
 *
 */
void
stopPump(void)
{
	/**
	 * Disable the PWM Outputs.
	 */
	PWMOutputState(BOOST_DRV8848_PWM_MODULE, BOOST_DRV8848_AIN1_OUT_BIT |
											 BOOST_DRV8848_AIN1_OUT_BIT, false);

	/**
	 * Stop the timers in generator 3.
	 */
	PWMGenDisable(BOOST_DRV8848_PWM_MODULE, PWM_GEN_3);
}
