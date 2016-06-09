/******************************************************************************
 * Project: Plant Irrigation System
 *
 * Created on: 06.06.2016
 * Author: Alija Sabic
 * E-Mail: sabic.alija@gmail.com
 *  
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 * 
 * Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 *****************************************************************************/

/**
 *	@file moisture_hal.h
 *
 *  @author Alija Sabic, ew13b510, BEW5A
 *
 *  @date Jan 2016
 *
 *  @brief Hardware Abstraction Layer for moisture measurement and power supply
 *  control
 */

#ifndef MOISTURE_HAL_H_
#define MOISTURE_HAL_H_

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_adc.h"
#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"

#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/hibernate.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/timer.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"

/**
 * System clock rate in Hz.
 */
extern uint32_t g_ui32SysClock;

/**
 * The most recent raw ADC reading for the moisture measurement results
 */
volatile int16_t g_i16MoistureValueCircuit[4];

/**
 * State of the repective periodic measurement circuits.
 */
//volatile bool g_bPeriodicMeasurementCircuit[3] = {false, false, false, false};

/**
 * Hardware oversampling factor
 */
#define MOISTURE_OVERSAMPLE			64 /**<Hardware oversampling rate 64x	 */

/**
 * Peripherals connected to moisture measurement circuit
 *
 * GPIO PORT M	>>	PM0, PM1, PM2, PM3
 * GPIO PORT K	>>	PK0, PK1, PK2, PK3
 * ADC0 MODULE	>> 	AIN16, AIN17, AIN18, AIN19 (periodic)
 * ADC1 MODULE	>> 	AIN16, AIN17, AIN18, AIN19 (on-demand)
 */
#define MOISTURE_DRIVE_PERIPH		SYSCTL_PERIPH_GPIOM	/**<Peripheral
									connected to moisture measurement drive
									pins 		 							 */
#define MOISTURE_ADC_PERIPH_GPIO	SYSCTL_PERIPH_GPIOK	/**<Peripheral
									connected to pins for adc measurement  	 */
#define MOISTURE_ADC_PERIPH_PERIODIC \
									SYSCTL_PERIPH_ADC1	/**<Peripheral
									connected to pins for periodic adc
									measurement  	 						 */
#define MOISTURE_ADC_PERIPH_ON_DEMAND \
									SYSCTL_PERIPH_ADC1	/**<Peripheral
									connected to pins for on-demand adc
									measurement  	 						 */
#define MOISTURE_TIMER_PERIPH_PERIODIC \
									SYSCTL_PERIPH_HIBERNATE /**<Peripheral of
									Hibernate modul used as rtc timer to
									trigger the periodic moisture
									measurement.			 				 */
#define MOISTURE_CYCLE_TIMER_PERIPH SYSCTL_PERIPH_TIMER3 /**<Peripheral of
									timer used to cycle the moisture
									measurement state machine				 */

/**
 * Ports and Modules connected to moisture measurement circuit
 *
 * GPIO PORT M	>>	PM0, PM1, PM2, PM3
 * GPIO PORT K	>>	PK0, PK1, PK2, PK3
 * ADC0 MODULE	>> 	AIN16, AIN17, AIN18, AIN19 (periodic)
 * ADC1 MODULE	>> 	AIN16, AIN17, AIN18, AIN19 (on-demand)
 */
#define MOISTURE_DRIVE_PORT			GPIO_PORTM_BASE	/**<Port connected to
									moisture measurement drive pins 		 */
#define MOISTURE_ADC_PORT			GPIO_PORTK_BASE	/**<Port connected to
									pins for adc measurement	 			 */
#define MOISTURE_ADC_MODULE_PERIODIC \
									ADC1_BASE /**<ADC module connected to
									pins for periodic adc measurement	 	 */
#define MOISTURE_ADC_MODULE_ON_DEMAND \
									ADC1_BASE /**<ADC module connected to
									pins for on-demand adc measurement	 	 */
#define MOISTURE_CYCLE_TIMER_MODULE TIMER3_BASE /**<Timer module used to cycle
 	 	 	 	 	 	 	 	 	the moisture measurement state machine	 */

/**
 * Pins connected to moisture measurement power supply control
 *
 */
#define MOISTURE_DRIVE1_PIN			GPIO_PIN_0	/**<GPIO pin connected to power
									supply control of first moisture
									measurement circuit						 */
#define MOISTURE_DRIVE2_PIN			GPIO_PIN_1	/**<GPIO pin connected to power
									supply control of second moisture
									measurement circuit						 */
#define MOISTURE_DRIVE3_PIN			GPIO_PIN_2	/**<GPIO pin connected to power
									supply control of third moisture
									measurement circuit						 */
#define MOISTURE_DRIVE4_PIN			GPIO_PIN_3	/**<GPIO pin connected to power
									supply control of fourth moisture
									measurement circuit						 */

/**
 * Pins connected to adc module for moisture measurement
 *
 */
#define MOISTURE_AIN1_PIN			GPIO_PIN_0	/**<GPIO pin connected to adc
									channel of first moisture measurement
									circuit									 */
#define MOISTURE_AIN2_PIN			GPIO_PIN_1	/**<GPIO pin connected to adc
									channel of second moisture measurement
									circuit									 */
#define MOISTURE_AIN3_PIN			GPIO_PIN_2	/**<GPIO pin connected to adc
									channel of third moisture measurement
									circuit									 */
#define MOISTURE_AIN4_PIN			GPIO_PIN_3	/**<GPIO pin connected to adc
									channel of fourth moisture measurement
									circuit									 */

/**
 * ADC channels connected to pins for moisture measurement
 *
 */
#define MOISTURE_AIN1_CH			ADC_CTL_CH16 /**<GPIO pin connected to adc
									channel of first moisture measurement
									circuit									 */
#define MOISTURE_AIN2_CH			ADC_CTL_CH17 /**<GPIO pin connected to adc
									channel of second moisture measurement
									circuit									 */
#define MOISTURE_AIN3_CH			ADC_CTL_CH18 /**<GPIO pin connected to adc
									channel of third moisture measurement
									circuit									 */
#define MOISTURE_AIN4_CH			ADC_CTL_CH19 /**<GPIO pin connected to adc
									channel of fourth moisture measurement
									circuit									 */

/**
 * Defines for periodic analog-digital conversion
 */
#define MOISTURE_PERIODIC_SEQUENCE_NUM \
									3 /**<ADC sequence number used for
									periodic measurement.					 */
#define MOISTURE_PERIODIC_PRIORITY	1 /**<Priority of adc sample sequence with
									respect to other adc sample sequences	 */
#define MOISTURE_PERIODIC_STEP		0 /**<Step number of periodic adc sample
									sequence								 */
#define MOISTURE_PERIODIC_ADC_INTERRUPT \
									INT_ADC1SS3	/**<ADC1 Sequence 3 Interrupt*/
#define MOISTURE_PERIODIC_TIMER_INTERRUPT \
									INT_HIBERNATE	/**<HIB Interrupt 		 */
#define MOISTURE_CYCLE_TIMER_INTERRUPT \
									INT_TIMER3A	/*<Timer3A Interrupt 		 */

/**
 * Defines for rtc timer
 */
#define MOISTURE_RTC_CLOCK			32768 /**<32.768-kHz oscillation frequency
									of hiberanation module rtc				 */

/**
 * Defines for periodic moisture measurement
 */
#define MOISTURE_CYCLE_DRIVE_TIME	(g_ui32SysClock / 500) /**<Time to drive
									the moisture measurement circuit before
									adc measurement - 200ms.				*/



/****************************************************************************
 *
 * The current state of the periodic moisture measurement state machine. This
 * is used to cycle the moisture measurement through all activated measurement
 * circuits
 *
 */
static uint32_t g_ui32MoistureMeasurementState;
#define MOISTURE_MEASUREMENT_DRIVE1 0 /**<Enable power supply of measurement
									circuit of first plant					 */
#define MOISTURE_MEASUREMENT_AIN1   1 /**<Perform moisture measurement at
									first plant								 */
#define MOISTURE_MEASUREMENT_DRIVE2 2 /**<Enable power supply of measurement
									circuit of first plant					 */
#define MOISTURE_MEASUREMENT_AIN2   3 /**<Perform moisture measurement at
									second plant							 */
#define MOISTURE_MEASUREMENT_DRIVE3 4 /**<Enable power supply of measurement
									circuit of first plant					 */
#define MOISTURE_MEASUREMENT_AIN3   5 /**<Perform moisture measurement at
									third plant								 */
#define MOISTURE_MEASUREMENT_DRIVE4 6 /**<Enable power supply of measurement
									circuit of first plant					 */
#define MOISTURE_MEASUREMENT_AIN4   7 /**<Perform moisture measurement at
									fourth plant							 */


/******************************************************************************
 * Prototype to initalize peripheral driving the moisture measurement.
 *
 */
extern void initMoistureMeasurementDrive(void);

/******************************************************************************
 * Prototype to enable power supply of specified moisture measurement circuit(s).
 *
 */
extern void enablePowerSupply(uint32_t ui32MoistureDrivePin);

/******************************************************************************
 * Prototype to disable power supply of specified moisture measurement circuit(s).
 *
 */
extern void disablePowerSupply(uint32_t ui32MoistureDrivePin);

/******************************************************************************
 * Prototype to initalize peripherals for periodic moisture measurement.
 *
 */
extern void initMoistureMeasurementPeriodic(uint32_t ui32Period);

/******************************************************************************
 * Prototype to reinitialises the counting cylce of RTC feature of HIB module.
 *
 */
extern void restartCountCycle(uint32_t ui32Value);

/******************************************************************************
 * Prototype to set the specified moisture measurement cycle time.
 *
 */
extern void setCycleTime(uint32_t ui32Period);

/******************************************************************************
 * Prototype to handle the ADC interrupt of the moisture measurement.
 *
 */
extern void PeriodicMoistureMeasurementIntHandler(void);

/******************************************************************************
 * Prototype to handle the timer interrupt to trigger the moisture measurement.
 *
 */
extern void PeriodicRTCTimerIntHandler(void);

#endif /* MOISTURE_HAL_H_ */
