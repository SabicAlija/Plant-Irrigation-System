/******************************************************************************
 * Project: Plant Irrigation System
 *
 * Created on: 06.06.2016
 * Author: Alija Sabic
 * E-Mail: sabic.alija@gmail.com
 *  
 * Copyright (C) 2016  Alija Sabic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *****************************************************************************/

/**
 *	@file moisture_hal.c
 *
 *  @author Alija Sabic, ew13b510, BEW5A
 *
 *  @date Jan 2016
 *
 *  @brief Hardware Abstraction Layer for moisture measurement and power supply
 *  control
 */

#include "moisture_hal.h"

/******************************************************************************
 * @function initMoistureMeasurementDrive()
 *
 * @brief Initalize peripheral driving the moisture measurement.
 *
 * This function is called to initalize peripheral to drive pins for moisture
 * measurement.
 *
 * @return None.
 *
 */
void
initMoistureMeasurementDrive(void)
{
	/**
	 * Enable Peripheral connected to pins controling the power supply of
	 * moisture measurement
	 */
	SysCtlPeripheralEnable(MOISTURE_DRIVE_PERIPH);

	/**
	 * Configure used GPIO pins connected to power supply control of moisture
	 * measurement as output
	 */
	GPIOPinTypeGPIOOutput(MOISTURE_DRIVE_PORT, MOISTURE_DRIVE1_PIN |
											   MOISTURE_DRIVE2_PIN |
											   MOISTURE_DRIVE3_PIN |
											   MOISTURE_DRIVE4_PIN);

	/**
	 * Turn off power supply of moisture measurement
	 */
	disablePowerSupply(MOISTURE_DRIVE1_PIN |
					   MOISTURE_DRIVE2_PIN |
					   MOISTURE_DRIVE3_PIN |
					   MOISTURE_DRIVE4_PIN);

}

/******************************************************************************
 * @function enablePowerSupply()
 *
 * @brief Enables power supply of specified moisture measurement circuit(s).
 *
 * This function is called to provide power supply of give moisture measurement
 * circuit and should be any logical or combination of MOISTURE_DRIVE1_PIN
 * through MOISTURE_DRIVE_PIN4.
 *
 * @return None.
 *
 */
void
enablePowerSupply(uint32_t ui32MoistureDrivePin)
{
	/**
	 * Check the arguments.
	 */
	ASSERT((ui32MoistureDrivePin == MOISTURE_DRIVE1_PIN) ||
	       (ui32MoistureDrivePin == MOISTURE_DRIVE2_PIN) ||
		   (ui32MoistureDrivePin == MOISTURE_DRIVE3_PIN) ||
	       (ui32MoistureDrivePin == MOISTURE_DRIVE4_PIN));

	/**
	 * Enable power supply of specifcied pin(s).
	 */
	GPIOPinWrite(MOISTURE_DRIVE_PORT, ui32MoistureDrivePin , ui32MoistureDrivePin);

}

/******************************************************************************
 * @function disablePowerSupply()
 *
 * @brief Disables power supply of specified moisture measurement circuit(s).
 *
 * This function is called to turn power supply of give moisture measurement
 * circuit off and should be any logical or combination of MOISTURE_DRIVE1_PIN
 * through MOISTURE_DRIVE_PIN4.
 *
 * @return None.
 *
 */
void
disablePowerSupply(uint32_t ui32MoistureDrivePin)
{
	/**
	 * Check the arguments.
	 */
	ASSERT((ui32MoistureDrivePin == MOISTURE_DRIVE1_PIN) ||
	       (ui32MoistureDrivePin == MOISTURE_DRIVE2_PIN) ||
		   (ui32MoistureDrivePin == MOISTURE_DRIVE3_PIN) ||
	       (ui32MoistureDrivePin == MOISTURE_DRIVE4_PIN));

	/**
	 * Disable power supply of specifcied pin(s).
	 */
	GPIOPinWrite(MOISTURE_DRIVE_PORT, ui32MoistureDrivePin , 0);

}


/******************************************************************************
 * @function initMoistureMeasurementPeriodic()
 *
 * @brief Initalize peripherals for periodic moisture measurement.
 *
 * This function is called to initalize peripherals needed for periodic
 * moisture measurement (adc-measurement).
 *
 * @param ui32Period specifies moisture measurement frequency (in seconds)
 * 		  Parameter must be greater 0 and less than 65536
 *
 * @return None.
 *
 */
void
initMoistureMeasurementPeriodic(uint32_t ui32Period)
{
	/**
	 * Check the arguments.
	 */
	ASSERT(ui32Period > 0);
	ASSERT(ui32Period < 65536);

	//ADD_SEMAPHORE
	/**
	 * Set the initial state of the moisture measurement state machine.
	 */
	g_ui32MoistureMeasurementState = MOISTURE_MEASUREMENT_DRIVE1;

	/**
	 * Enable Peripheral connected for periodic moisture measurement
	 *
	 */
	SysCtlPeripheralEnable(MOISTURE_ADC_PERIPH_GPIO);
	SysCtlPeripheralEnable(MOISTURE_ADC_PERIPH_PERIODIC);
	SysCtlPeripheralEnable(MOISTURE_CYCLE_TIMER_PERIPH);
	SysCtlPeripheralEnable(MOISTURE_TIMER_PERIPH_PERIODIC);

	/**
	 * Configure used GPIO pins for use as analog-to-digital converter
	 * inputs
	 *
	 */
	GPIOPinTypeADC(MOISTURE_ADC_PORT, MOISTURE_AIN1_PIN |
									  MOISTURE_AIN2_PIN |
									  MOISTURE_AIN3_PIN |
									  MOISTURE_AIN4_PIN);

	/**
	 * Configures the hardware oversampling factor of the analog-digital
	 * conversion
	 *
	 */
	ADCHardwareOversampleConfigure(MOISTURE_ADC_MODULE_PERIODIC,
								   MOISTURE_OVERSAMPLE);

	/**
	 * Configures the processor as trigger source and priority of the
	 * adc sample sequence
	 */
	ADCSequenceConfigure(MOISTURE_ADC_MODULE_PERIODIC,   /**<ADC module				*/
						 MOISTURE_PERIODIC_SEQUENCE_NUM, /**<Sample sequence number */
						 ADC_TRIGGER_PROCESSOR, 		 /**Proccessor as trigger
						 	 	 	 	 	 	 	 	 	source					*/
						 MOISTURE_PERIODIC_PRIORITY);	 /**Priority with respect to
						 	 	 	 	 	 	 	 	 	other sample sequences	*/

	/**
	 * Configure the step of the sample sequencer for periodic moisture measurement
	 *
	 */
	ADCSequenceStepConfigure(MOISTURE_ADC_MODULE_PERIODIC,   /**<ADC module			*/
							 MOISTURE_PERIODIC_SEQUENCE_NUM, /**<Sample sequence
																 number			 	*/
							 MOISTURE_PERIODIC_STEP,		 /**<Step to be
																 configured			*/
							 MOISTURE_AIN1_CH |				 /**<First channel to be
																 measured			*/
							 ADC_CTL_END |					 /**<Last step in the
																 sequence			*/
							 ADC_CTL_IE |					 /**<Interrupt enable   */
							 ADC_CTL_SHOLD_256);			 /**<Sample and hold 256
																 ADC clocks	 		*/
	/**
	 * Enable the sample sequence
	 */
	ADCSequenceEnable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
					  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																 number			 	*/

	/**
	 * Enable the ADC sample sequence interrupt.
	 */
	ADCIntEnable(MOISTURE_ADC_MODULE_PERIODIC,    /**<ADC module					*/
				 MOISTURE_PERIODIC_SEQUENCE_NUM); /**<Sample sequence			    */

	/**
	 * Enable the interrupt handling on NVIC
	 */
	IntEnable(MOISTURE_PERIODIC_ADC_INTERRUPT);

	/**
	 * Configure the timer as full-width one-shot timer to allow counting up to
	 * 2^32 (= 4.294.967.296 ticks / 120 MHz ~> 357 seconds)
	 *
	 */
	TimerConfigure(MOISTURE_CYCLE_TIMER_MODULE, 		/**<Timer module to schedule
														periodic measurement		*/
				   TIMER_CFG_ONE_SHOT);					/**<Full-width one-shot
														timer						*/

	/**
	 * Enable timer interrupt source
	 */
	TimerIntEnable(MOISTURE_CYCLE_TIMER_MODULE,			/**<Timer module for periodic
														measurement	*/
				   TIMER_TIMA_TIMEOUT);					/**<Timeout interrupt		*/

	/**
	 * Enable the interrupt handling on NVIC
	 */
	IntEnable(MOISTURE_CYCLE_TIMER_INTERRUPT);

	/**
	 * Enable 32.768-kHz Hibernation oscillator
	 */
	HibernateEnableExpClk(MOISTURE_RTC_CLOCK);

	/**
	 * Disable the RTC feature of the Hibernation module before configuration
	 */
	HibernateRTCDisable();

	/**
	 * Write the required RTC match value
	 */
	HibernateRTCMatchSet(0, ui32Period);
	HibernateRTCSSMatchSet(0, ui32Period);

	/**
	 * Set the initial value of the real time clock (RTC) counter
	 */
	HibernateRTCSet(0);

	/**
	 * Enable interrupt for the Hibernation module when rtc match occurs
	 */
	HibernateIntEnable(HIBERNATE_INT_RTC_MATCH_0);

	/**
	 * Enable the interrupt handling on NVIC
	 */
	IntEnable(MOISTURE_PERIODIC_TIMER_INTERRUPT);

	/**
	 * Enable the RTC feature of the Hibernation module
	 */
	HibernateRTCEnable();
}

/******************************************************************************
 * @function __wait()
 *
 * @brief Sets the measurement cycle timer to trigger state switching.
 *
 * This function is called configure the cycle timer to schedule the moisture
 * measurement.
 *
 * @param ui32CycleTime	is the load value for the cycle timer
 *
 * @return None.
 *
 */
static void
__wait(uint32_t ui32CycleTime)
{
	/**
	 * Configure the timer to trigger the sampling of moisture measurement.
	 */
	TimerLoadSet(MOISTURE_CYCLE_TIMER_MODULE, 		/**<Timer module to schedule
														periodic measurement	*/
				 TIMER_A,							/**<Timer for configuration
														in concatenated mode	*/
				 ui32CycleTime);					/**<Cycle time				*/

	/**
	 * Enable the timer. At this point, the state of moisture measurement state
	 * machine will be cycled once when cycle timer reaches timeout
	 */
	TimerEnable(MOISTURE_CYCLE_TIMER_MODULE, 		/**<Timer module for periodic
														measurement	*/
				TIMER_A);							/**<Timer for configuration in
														concatenated mode		*/

}

/******************************************************************************
 * @function restartCountCycle()
 *
 * @brief Reinitialises the counting cylce of RTC feature of HIB module.
 *
 * This function is called to stop and reload RTC counter value of HIB module.
 *
 * @param ui32Value	is the counter value of RTC counter loaded into HIB RTC
 * counter register before count cycle is being restarted.
 *
 * @return None.
 *
 */
void
restartCountCycle(uint32_t ui32Value)
{
	/*
	 * Disable the RTC feature of the Hibernation module before configuration
	 */
	HibernateRTCDisable();

	/**
	 * Set the initial value of the real time clock (RTC) counter
	 */
	HibernateRTCSet(ui32Value);

	/**
	 * Enable the RTC feature of the Hibernation module
	 */
	HibernateRTCEnable();
}

/******************************************************************************
 * @function setCycleTime()
 *
 * @brief Sets the specified moisture measurement cycle time.
 *
 * This function is called to set the computed value in the match register of
 * RTC feature of HIB module.
 *
 * @param ui32Period is the match value of RTC counter loaded into HIB RTC
 * counter register.
 *
 * @return None.
 *
 */
void
setCycleTime(uint32_t ui32Period)
{
	/**
	 * Disable the RTC feature of the Hibernation module before configuration
	 */
	HibernateRTCDisable();

	/**
	 * Write the required RTC match value
	 */
	HibernateRTCMatchSet(0, ui32Period);
	HibernateRTCSSMatchSet(0, ui32Period);

	/**
	 * Enable the RTC feature of the Hibernation module again
	 */
	HibernateRTCEnable();
}


/******************************************************************************
 * @function PeriodicMoistureMeasurementIntHandler()
 *
 * @brief Handles the ADC interrupt of the moisture measurement.
 *
 * This function is called when the ADC sequence that samples the moisture
 * measurement has completed its acquisition. The moisture measurement state
 * machine state machine is advanced and the acquired ADC sample is processed
 * appropriately.
 *
 * It is the responsibility of the application using the moisture measurement HAL
 * to ensure that this function is installed in the interrupt vector table for
 * the ADC3 interrupt and administered by the TI-RTOS.
 *
 * @return None.
 *
 */
void
PeriodicMoistureMeasurementIntHandler(void)
{
	/**
	 * Clear the interrupt asserted by the ADC module used for periodic
	 * measurement
	 */
	ADCIntClear(MOISTURE_ADC_MODULE_PERIODIC,    		/**<ADC module		 */
				MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
															number			 */

	/**
	 * Determine what to do, bases on the current state of the
	 */
	switch(g_ui32MoistureMeasurementState)
	{
		/**
		 * The power supply of moisture measurement of first plant has to be
		 * enabled
		 */
		case MOISTURE_MEASUREMENT_DRIVE1:

			/**
			 * Enable power supply of first moisture measurement circuit
			 */
			enablePowerSupply(MOISTURE_DRIVE1_PIN);

			/**
			 * Disable the sample sequence before configuration
			 */
			ADCSequenceDisable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
							  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																		 number			 	*/

			/**
			 * Configure the step of the sample sequencer for periodic moisture measurement
			 *
			 */
			ADCSequenceStepConfigure(MOISTURE_ADC_MODULE_PERIODIC,   /**<ADC module			*/
									 MOISTURE_PERIODIC_SEQUENCE_NUM, /**<Sample sequence
																		 number			 	*/
									 MOISTURE_PERIODIC_STEP,		 /**<Step to be
																		 configured			*/
									 MOISTURE_AIN1_CH |				 /**<First channel to be
																		 measured			*/
									 ADC_CTL_END |					 /**<Last step in the
																		 sequence			*/
									 ADC_CTL_IE |					 /**<Interrupt enable   */
									 ADC_CTL_SHOLD_256);			 /**<Sample and hold 256
																		 ADC clocks	 		*/

			/**
			 * Enable the sample sequence again
			 */
			ADCSequenceEnable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
							  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																		 number			 	*/

			/**
			 * The next cycle will perform the moisture measurment
			 */
			g_ui32MoistureMeasurementState = MOISTURE_MEASUREMENT_AIN1;

			/**
			 * Set the trigger for the next state
			 */
			__wait(MOISTURE_CYCLE_DRIVE_TIME);

			break;

		/**
		 * The new sample of first moisture measurement circuit is ready to be processed.
		 */
		case MOISTURE_MEASUREMENT_AIN1:

			/**
			 * Read the raw ADC sample.
			 */
			g_i16MoistureValueCircuit[0] = HWREG(MOISTURE_ADC_MODULE_PERIODIC + ADC_O_SSFIFO3);

			/**
			 * Disable power supply of first moisture measurement circuit
			 */
			disablePowerSupply(MOISTURE_DRIVE1_PIN);

		/**
		 * The power supply of moisture measurement of second plant has to be
		 * enabled
		 */
		case MOISTURE_MEASUREMENT_DRIVE2:

			/**
			 * Enable power supply of second moisture measurement circuit
			 */
			enablePowerSupply(MOISTURE_DRIVE2_PIN);

			/**
			 * Disable the sample sequence before configuration
			 */
			ADCSequenceDisable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
							  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																		 number			 	*/

			/**
			 * Configure the step of the sample sequencer for periodic moisture measurement
			 *
			 */
			ADCSequenceStepConfigure(MOISTURE_ADC_MODULE_PERIODIC,   /**<ADC module			*/
									 MOISTURE_PERIODIC_SEQUENCE_NUM, /**<Sample sequence
																		 number			 	*/
									 MOISTURE_PERIODIC_STEP,		 /**<Step to be
																		 configured			*/
									 MOISTURE_AIN2_CH |				 /**<Second channel to be
																		 measured			*/
									 ADC_CTL_END |					 /**<Last step in the
																		 sequence			*/
									 ADC_CTL_IE |					 /**<Interrupt enable   */
									 ADC_CTL_SHOLD_256);			 /**<Sample and hold 256
																		 ADC clocks	 		*/

			/**
			 * Enable the sample sequence again
			 */
			ADCSequenceEnable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
							  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																		 number			 	*/

			/**
			 * The next cycle will perform the moisture measurment
			 */
			g_ui32MoistureMeasurementState = MOISTURE_MEASUREMENT_AIN2;

			/**
			 * Set the trigger for the next state
			 */
			__wait(MOISTURE_CYCLE_DRIVE_TIME);
			break;

		/**
		 * The new sample of second moisture measurement circuit is ready to be processed.
		 */
		case MOISTURE_MEASUREMENT_AIN2:

			/**
			 * Read the raw ADC sample.
			 */
			g_i16MoistureValueCircuit[1] = HWREG(MOISTURE_ADC_MODULE_PERIODIC + ADC_O_SSFIFO3);

			/**
			 * Disable power supply of second moisture measurement circuit
			 */
			disablePowerSupply(MOISTURE_DRIVE2_PIN);

		/**
		 * The power supply of moisture measurement of third plant has to be
		 * enabled
		 */
		case MOISTURE_MEASUREMENT_DRIVE3:

			/**
			 * Enable power supply of third moisture measurement circuit
			 */
			enablePowerSupply(MOISTURE_DRIVE3_PIN);

			/**
			 * Disable the sample sequence before configuration
			 */
			ADCSequenceDisable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
							  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																		 number			 	*/

			/**
			 * Configure the step of the sample sequencer for periodic moisture measurement
			 *
			 */
			ADCSequenceStepConfigure(MOISTURE_ADC_MODULE_PERIODIC,   /**<ADC module			*/
									 MOISTURE_PERIODIC_SEQUENCE_NUM, /**<Sample sequence
																		 number			 	*/
									 MOISTURE_PERIODIC_STEP,		 /**<Step to be
																		 configured			*/
									 MOISTURE_AIN3_CH |				 /**<Thrid channel to be
																		 measured			*/
									 ADC_CTL_END |					 /**<Last step in the
																		 sequence			*/
									 ADC_CTL_IE |					 /**<Interrupt enable   */
									 ADC_CTL_SHOLD_256);			 /**<Sample and hold 256
																		 ADC clocks	 		*/

			/**
			 * Enable the sample sequence again
			 */
			ADCSequenceEnable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
							  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																		 number			 	*/

			/**
			 * The next cycle will perform the moisture measurment
			 */
			g_ui32MoistureMeasurementState = MOISTURE_MEASUREMENT_AIN3;

			/**
			 * Set the trigger for the next state
			 */
			__wait(MOISTURE_CYCLE_DRIVE_TIME);
			break;

		/**
		 * The new sample of third moisture measurement circuit is ready to be processed.
		 */
		case MOISTURE_MEASUREMENT_AIN3:

			/**
			 * Read the raw ADC sample.
			 */
			g_i16MoistureValueCircuit[2] = HWREG(MOISTURE_ADC_MODULE_PERIODIC + ADC_O_SSFIFO3);

			/**
			 * Disable power supply of second moisture measurement circuit
			 */
			disablePowerSupply(MOISTURE_DRIVE3_PIN);

		/**
		 * The power supply of moisture measurement of fourth plant has to be
		 * enabled
		 */
		case MOISTURE_MEASUREMENT_DRIVE4:

			/**
			 * Enable power supply of fourth moisture measurement circuit
			 */
			enablePowerSupply(MOISTURE_DRIVE4_PIN);

			/**
			 * Disable the sample sequence before configuration
			 */
			ADCSequenceDisable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
							  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																		 number			 	*/

			/**
			 * Configure the step of the sample sequencer for periodic moisture measurement
			 *
			 */
			ADCSequenceStepConfigure(MOISTURE_ADC_MODULE_PERIODIC,   /**<ADC module			*/
									 MOISTURE_PERIODIC_SEQUENCE_NUM, /**<Sample sequence
																		 number			 	*/
									 MOISTURE_PERIODIC_STEP,		 /**<Step to be
																		 configured			*/
									 MOISTURE_AIN4_CH |				 /**<Thrid channel to be
																		 measured			*/
									 ADC_CTL_END |					 /**<Last step in the
																		 sequence			*/
									 ADC_CTL_IE |					 /**<Interrupt enable   */
									 ADC_CTL_SHOLD_256);			 /**<Sample and hold 256
																		 ADC clocks	 		*/

			/**
			 * Enable the sample sequence again
			 */
			ADCSequenceEnable(MOISTURE_ADC_MODULE_PERIODIC,   		/**<ADC module			*/
							  MOISTURE_PERIODIC_SEQUENCE_NUM);		/**<Sample sequence
																		 number			 	*/

			/**
			 * The next cycle will perform the moisture measurment
			 */
			g_ui32MoistureMeasurementState = MOISTURE_MEASUREMENT_AIN4;

			/**
			 * Set the trigger for the next state
			 */
			__wait(MOISTURE_CYCLE_DRIVE_TIME);
			break;

		/**
		 * The new sample of fourth moisture measurement circuit is ready to be processed.
		 */
		case MOISTURE_MEASUREMENT_AIN4:

			/**
			 * Read the raw ADC sample.
			 */
			g_i16MoistureValueCircuit[3] = HWREG(MOISTURE_ADC_MODULE_PERIODIC + ADC_O_SSFIFO3);

			/**
			 * Disable power supply of second moisture measurement circuit
			 */
			disablePowerSupply(MOISTURE_DRIVE4_PIN);
		default:
			break;
	}

}

/******************************************************************************
 * @function PeriodicTimerIntHandler()
 *
 * @brief Handles the timer interrupt to trigger the moisture measurement.
 *
 * This function is called when the timer that specifies the measurement
 * frequency signals a timeout event.
 *
 * It is the responsibility of the application using the moisture measurement HAL
 * to ensure that this function is installed in the interrupt vector table for
 * the specified HIB interrupt and administered by the TI-RTOS.
 *
 * @return None.
 *
 */
void
PeriodicRTCTimerIntHandler(void)
{
	/**
	 * Clear the interrupt asserted by the Hiberanation module after RTC match
	 */
	HibernateIntClear(HIBERNATE_INT_RTC_MATCH_0);

	//ADD_SEMAPHORE
	/**
	 * Set the initial state of the moisture measurement state machine.
	 */
	g_ui32MoistureMeasurementState = MOISTURE_MEASUREMENT_DRIVE1;

	//
	// Trigger the sample sequence.
	//
	ADCProcessorTrigger(ADC1_BASE, 3);

	/**
	 * Reinitiate RTC counting cycle
	 */
	restartCountCycle(0);
}

/******************************************************************************
 * @function CycleTimerIntHandler()
 *
 * @brief Handles the timer interrupt to schedule the moisture measurement.
 *
 * This function is called when the timer that specifies the measurement
 * frequency signals a timeout event.
 *
 * It is the responsibility of the application using the moisture measurement HAL
 * to ensure that this function is installed in the interrupt vector table for
 * the specified TIMER interrupt and administered by the TI-RTOS.
 *
 * @return None.
 *
 */
void
CycleTimerIntHandler(void)
{
	/**
	 * Clear the interrupt asserted by the Hiberanation module after RTC match
	 */
	TimerIntClear(MOISTURE_CYCLE_TIMER_MODULE, INT_TIMER3A);

	//
	// Trigger the sample sequence.
	//
	ADCProcessorTrigger(ADC1_BASE, 3);
}

