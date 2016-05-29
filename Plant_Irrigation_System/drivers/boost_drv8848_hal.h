/******************************************************************************
 *  Copyright (C) 2016 by Alija Sabic
 *
 *  Project: Plant Irrigation System
 *
 *****************************************************************************/

/**
 *	@file boost_drv8848_hal.h
 *
 *  @author Alija Sabic, ew13b510, BEW5A
 *
 *  @date Jan 2016
 *
 *  @brief Hardware Abstraction Layer for Boosterpack DRV8848
 */

/*
 * boost_drv8848_hal.h
 *
 *  Created on: 11.01.2016
 *      Author: Alija
 */

#ifndef BOOST_DRV8848_HAL_H_
#define BOOST_DRV8848_HAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_pwm.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"

/**
 * System clock rate in Hz.
 */
extern uint32_t g_ui32SysClock;

/**
 * PWM frequency. For a 50 KHz frequency, the period = 1/50,000, or 20
 * microseconds. For a 120 MHz clock, this translates to 2400 clock ticks.
 * Use this value to set the period.
 */
#define BOOST_DRV8848_50kHZ			2400	/**<Clock ticks needed for 50kHZ */

/**
 * Peripherals connected to Boosterpack DRV8848
 *
 * GPIO PORT K	>>	PWM6, PWM7
 * GPIO PORT H	>>	Boosterpack DRV8848 Control
 * PWM0 MODULE
 */
#define BOOST_DRV8848_CNTR_PERIPH	SYSCTL_PERIPH_GPIOH	/**<Peripheral
												connected to control pins 	 */
#define BOOST_DRV8848_PWM_PERIPH_GPIO SYSCTL_PERIPH_GPIOK	/**<Peripheral
												connected to PWM pins 		 */
#define BOOST_DRV8848_PWM_PERIPH_PWM SYSCTL_PERIPH_PWM0	/**<Peripheral
												connected to PWM module		 */

/**
 * Ports and Modules connected to Boosterpack DRV8848
 *
 * GPIO PORT K	>>	PWM6, PWM7
 * GPIO PORT H	>>	Boosterpack DRV8848 Control
 * PWM0 MODULE
 */
#define BOOST_DRV8848_CNTR_PORT		GPIO_PORTH_BASE	/**<Port of connected
														control pins 		 */
#define BOOST_DRV8848_PWM_PORT		GPIO_PORTK_BASE	/**<Port of connected
														PWM pins 			 */
#define BOOST_DRV8848_PWM_MODULE	PWM0_BASE		/**<Module connected to SPI
														module 				 */

/**
 * Control Pins connected to Boosterpack DRV8848
 */
#define BOOST_DRV8848_nSLEEP 		GPIO_PIN_3		/**<Pin connected to
														nSLEEP 	(DRV8848) 	 */

/**
 * Pins connected to PWM0 module to control Boosterpack DRV8848
 */
#define BOOST_DRV8848_AIN1			GPIO_PIN_5	/**<PWM pin M0PWM7 connected to
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	  AIN1 (DRV8848) */
#define BOOST_DRV8848_AIN2			GPIO_PIN_4	/**<PWM pin M0PWM6 connected to
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	  AIN2 (DRV8848) */

/**
 * PWM outputs connected to Boosterpack DRV8848
 */
#define BOOST_DRV8848_AIN1_OUT		PWM_OUT_7	/**<PWM output connected to
												  AIN1 of Boosterpack DRV8848*/
#define BOOST_DRV8848_AIN2_OUT		PWM_OUT_6	/**<PWM output connected to
												  AIN2 of Boosterpack DRV8848*/

/**
 * Bit-wise ID for PWM
 */
#define BOOST_DRV8848_AIN1_OUT_BIT	PWM_OUT_7_BIT /**<Bit-wise ID for PWM7   */
#define BOOST_DRV8848_AIN2_OUT_BIT	PWM_OUT_6_BIT /**<Bit-wise ID for PWM6	 */

/**
 * Pin functions used for PWM generation
 */
#define BOOST_DRV8848_AIN1_PINF		GPIO_PK5_M0PWM7	/**<PWM pin function
											connected to pin AIN1 (DRV8848)  */
#define BOOST_DRV8848_AIN2_PINF		GPIO_PK4_M0PWM6	/**<PWM pin function
											connected to pin AIN2 (DRV8848)  */

/******************************************************************************
 * Prototype to initalize peripherals for pump control via Boosterpack DRV8848.
 *
 */
void initPumpControl(uint8_t ui8DutyCycle);

/******************************************************************************
 * Prototype to deinitalize peripherals for pump control via Boosterpack
 * DRV8848.
 *
 */
void deinitPumpControl(void);

/******************************************************************************
 * Prototype to configure duty cycle of pump control via Boosterpack DRV8848.
 *
 */
void setDutyCycle(uint8_t ui8DutyCycle);

/******************************************************************************
 * Prototype to stop PWM generation on Boosterpack DRV8848.
 *
 */
void stopPump(void);

#endif /* BOOST_DRV8848_HAL_H_ */
