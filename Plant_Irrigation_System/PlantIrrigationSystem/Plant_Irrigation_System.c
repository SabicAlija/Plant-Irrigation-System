/**
 * Plant_Irrigation_System.c
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
 */
#include <PlantIrrigationSystem/Plant_Irrigation_System.h>


/**
 *  ======== initPlantIrrigationSystem ========
 *  Initializes Plant Irrigation System structure.
 */
void initPlantIrrigationSystem(PlantIrrigationSystem_t* pSystem) {

	uint8_t ui8Idx;

	/** Initialize pump */
	pSystem->Pump.DutyCycle = PUMP_DUTYCYCLE_INIT;
	pSystem->Pump.State = PUMP_STATE_INIT;

	for(ui8Idx = 0; ui8Idx < 4; ui8Idx++) {

		/** Initialize automatic irrigation */
		pSystem->State[ui8Idx] = PLANT_IRRIGATION_LINE_INIT_STATE;

		/** Initialize moisutre sensor */
		pSystem->MoistureSensor[ui8Idx].CurrentValue = MOISTURE_SENSOR_CURRENT_VALUE_INIT;
		pSystem->MoistureSensor[ui8Idx].ThresholdDry = MOISTURE_SENSOR_DRY_THRESHOLD_INIT;
		pSystem->MoistureSensor[ui8Idx].ThresholdWet = MOISTURE_SENSOR_WET_THRESHOLD_INIT;

		/** Initialize valve */
		pSystem->Valve[ui8Idx].State = VALVE_STATE_INIT;
	}

}

/**
 *  ======== startPlantIrrigationSystem ========
 *  Creates new Task to handle incoming control request.
 */
void startPlantIrrigationSystem(PlantIrrigationSystem_t* pSystem) {

	Task_Handle 			PlantIrrigationSystemTask;
	Task_Params 			taskParams;

	GateMutex_Params		gateParams;

	Mailbox_Params 	mboxParams;

	Error_Block 	eb;

	Error_init(&eb);

	/**
	 * Create the Task that listes on Control Request MsgBox
	 */
	Task_Params_init(&taskParams);
	taskParams.stackSize = PLANTIRRIGATIONSYSTEMSTACK;
	taskParams.priority = 1;
	taskParams.arg0 = (UArg) pSystem;
	PlantIrrigationSystemTask = Task_create((Task_FuncPtr)PlantIrrigationSystem, &taskParams, &eb);
	if (PlantIrrigationSystemTask == NULL) {
		System_printf("startPlantIrrigationSystem: Task creation failed.\n");
	}

	/**
	 * Create Event used by Plant Irrigation System
	 */
	PlantControlEvent = Event_create(NULL, &eb);
	if (PlantControlEvent == NULL) {
		System_printf("startPlantIrrigationSystem: Event creation failed.\n");
	}

	Mailbox_Params_init(&mboxParams);
	mboxParams.readerEvent = PlantControlEvent;

	/** Asign Event_Id_00 to PlantIrrigationMailbox "not empty" event */
	mboxParams.readerEventId = Event_Id_00;

	/**
	 * Mailbox_create() sets Mailbox's readerEvent to
	 * counting mode and initial count = NUMPLANTWORKERS*2
	 */
	PlantIrrigationMailboxRx = Mailbox_create(sizeof(MsgObj), NUMPLANTWORKERS*2, &mboxParams, &eb);
	if (PlantIrrigationMailboxRx == NULL) {
		System_printf("startPlantIrrigationSystem: Mailbox creation failed.\n");
	}

	Mailbox_Params_init(&mboxParams);

	/**
	 * Mailbox_create() sets Mailbox's readerEvent to
	 * counting mode and initial count = NUMPLANTWORKERS*2
	 */
	PlantIrrigationMailboxTx = Mailbox_create(sizeof(MsgObj), NUMPLANTWORKERS*2, &mboxParams, &eb);
	if (PlantIrrigationMailboxTx == NULL) {
		System_printf("startPlantIrrigationSystem: Mailbox creation failed.\n");
	}


	GateMutex_Params_init(&gateParams);

	/**
	 * Create GateMutex (with Semaphores) to protect Plant Irrigation System
	 * from concurrent access and control requests.
	 */
	PlantIrrigationSystemGate = GateMutex_create(&gateParams, &eb);
	if (PlantIrrigationSystemGate == NULL) {
		System_printf("startPlantIrrigationSystem: GateMutex creation failed.");
	}

	System_flush();

}


/**
 *  ======== PlantIrrigationSystem ========
 *  Creates new Task to handle incoming control request.
 */
Void PlantIrrigationSystem(UArg arg0, UArg arg1) {

	PlantIrrigationSystem_t* 	pSystem = (PlantIrrigationSystem_t *) arg0;
	PlantIrrigationSystem_t		systemIntent;
	MsgObj						msg;
	UInt						plantGateKey;
	UInt						events;
	uint8_t						ui8ActiveLine;
	uint8_t						ui8CmdIdx;
	uint8_t						ui8Cmd;
	uint8_t						ui8Idx;
	uint8_t						ui8Idy;
	bool						otherValveOpen = false;
	char 						buffer[PLANTPACKETSIZE];

	while (1) {

		/** Listen on Plant Control Events */
		events =  Event_pend(PlantControlEvent,			/** Event Handle */
						Event_Id_NONE,					/** AND mask */
						Event_Id_00 + Event_Id_01,		/** OR mask */
						BIOS_WAIT_FOREVER);				/** Timeout */




		/** Control Events from Plant Irrigation Server */
		if (events & Event_Id_00) {
			Mailbox_pend(PlantIrrigationMailboxRx, &msg, BIOS_NO_WAIT);

			/** Load active Line */
			ui8ActiveLine = msg.buffer[0];

			/** Load Command Counter */
			ui8CmdIdx = msg.buffer[1] + 1;

			/** Enter critical region. Access Plant Irrigation System. */
			plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

			/** Load intent Plant Irrigation System state. */
			readSystem(ui8CmdIdx, &systemIntent, msg.buffer);

			/** Exit critical region. */
			GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);

			/**
			 * Iterate over received commands.
			 */
			for (ui8Idx = 2; ui8Idx <= ui8CmdIdx; ui8Idx++) {

				/** Load Command */
				ui8Cmd = msg.buffer[ui8Idx];

				/**
				 * Handle received command individually.
				 */
				switch(ui8Cmd) {

					case PLANT_CMD_ECHO:
						break;

					case PLANT_CMD_AUTOMAT_ON:
					case PLANT_CMD_AUTOMAT_OFF:
						/** Enter critical region. Access Plant Irrigation System. */
						plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

						pSystem->State[ui8ActiveLine] = systemIntent.State[ui8ActiveLine];

						/** Exit critical region. */
						GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
						break;

					case PLANT_CMD_VALVE_OPEN:

						/** Check if client intents to open more than one valve */
						for(ui8Idy = 0; ui8Idy < 4; ui8Idy++) {
							if(systemIntent.Valve[ui8Idy].State == true) {
								if(ui8Idy != ui8ActiveLine) {
									otherValveOpen = true;
								}
							}
						}

						/** Enter critical region. Access Plant Irrigation System. */
						plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

						/** Check if valve of active line or other valves are open */
						if(!pSystem->Valve[ui8ActiveLine].State && !otherValveOpen) {

							pSystem->Valve[ui8ActiveLine].State = true;

							/** Handle active line individually. */
							switch(ui8ActiveLine) {

								case 0:
									switchValve(TPIC6A595_VALVE_1);
									pSystem->Valve[1].State = false;
									pSystem->Valve[2].State = false;
									pSystem->Valve[3].State = false;
									break;

								case 1:
									switchValve(TPIC6A595_VALVE_2);
									pSystem->Valve[0].State = false;
									pSystem->Valve[2].State = false;
									pSystem->Valve[3].State = false;
									break;

								case 2:
									switchValve(TPIC6A595_VALVE_3);
									pSystem->Valve[0].State = false;
									pSystem->Valve[1].State = false;
									pSystem->Valve[3].State = false;
									break;

								case 3:
									switchValve(TPIC6A595_VALVE_4);
									pSystem->Valve[0].State = false;
									pSystem->Valve[1].State = false;
									pSystem->Valve[2].State = false;
									break;

								default:
									break;
							}
						}

						/** Reload open valve check */
						otherValveOpen = false;

						/** Exit critical region. */
						GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
						System_printf("plantGateKey = 0x%x\n", plantGateKey);
						System_flush();
						break;

					case PLANT_CMD_VALVE_CLOSE:
						/** Enter critical region. Access Plant Irrigation System. */
						plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

						pSystem->Valve[ui8ActiveLine].State = systemIntent.Valve[ui8ActiveLine].State;
						switchValve(TPIC6A595_CLOSE_VALVES);

						/** Exit critical region. */
						GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
						System_printf("plantGateKey = %d\n", plantGateKey);
						System_flush();
						break;

					case PLANT_CMD_PUMP_ON:
						/** Enter critical region. Access Plant Irrigation System. */
						plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

						if(pSystem->Valve[ui8ActiveLine].State &&
							systemIntent.Valve[ui8ActiveLine].State ) {

							pSystem->Pump.State = 1;

							initPumpControl(pSystem->Pump.DutyCycle);
						}

						/** Exit critical region. */
						GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
						break;

					case PLANT_CMD_PUMP_OFF:
						/** Enter critical region. Access Plant Irrigation System. */
						plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);
						pSystem->Pump.State = systemIntent.Pump.State;
						stopPump();

						/** Exit critical region. */
						GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
						break;

					case PLANT_CMD_PUMP_DUTY_CYCLE:
						/** Enter critical region. Access Plant Irrigation System. */
						plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

						pSystem->Pump.DutyCycle = systemIntent.Pump.DutyCycle;

						/** Exit critical region. */
						GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
						break;

					case PLANT_CMD_MOISTURE_SENSOR:
						/** Enter critical region. Access Plant Irrigation System. */
						plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

						pSystem->MoistureSensor[ui8ActiveLine].CurrentValue = systemIntent.MoistureSensor[ui8ActiveLine].CurrentValue;
						pSystem->MoistureSensor[ui8ActiveLine].ThresholdDry = systemIntent.MoistureSensor[ui8ActiveLine].ThresholdDry;
						pSystem->MoistureSensor[ui8ActiveLine].ThresholdWet = systemIntent.MoistureSensor[ui8ActiveLine].ThresholdWet;

						/** Exit critical region. */
						GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
						break;

					default:
						break;
				}

				/** Write command into transmit buffer */
				writeCommand(ui8Idx, ui8Cmd, (uint8_t *) buffer);

			}

			/** Prepare transmit buffer */
			buffer[0] = ui8ActiveLine;
			buffer[1] = ui8Idx - 2;

			/** Enter critical region. Access Plant Irrigation System. */
			plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

			/** Write Plant Irrigation System Object data into transmit buffer */
			writeSystem(ui8CmdIdx, pSystem, (uint8_t *) buffer);

			/** Exit critical region. */
			GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);

			/** Load buffer into message object */
			memcpy(msg.buffer, buffer, sizeof(buffer));

			/** Send message to Plant Irrigation Server Task */
			Mailbox_post(PlantIrrigationMailboxTx, &msg, BIOS_WAIT_FOREVER);

		}


		/** Control Event from TS Display */
		if (events & Event_Id_01) {

		}


	}

}


/**
 *  ======== readSystem ========
 *  Reads values of intented Plant Irrigation System state.
 */
void readSystem(uint8_t ui8CmdIdx, PlantIrrigationSystem_t* pSystem, uint8_t* pBuffer)
{

	uint8_t		ui8Start;
	uint8_t		ui8Idx;

	/** Calculate start position of Plant Irrigation System object
	 * based on amount of commands.
	 */
	ui8Start = ui8CmdIdx + 1;

	/** Read pump data from buffer */
	pSystem->Pump.DutyCycle = pBuffer[ui8Start + 4];
	pSystem->Pump.State = pBuffer[ui8Start + 5];

	for(ui8Idx = 0; ui8Idx < 4; ui8Idx++) {

		/** Read state of automatic irrigation from buffer */
		pSystem->State[ui8Idx] = (bool) pBuffer[ui8Start + ui8Idx];

		/** Read data of moisutre sensor from buffer */
		pSystem->MoistureSensor[ui8Idx].CurrentValue = pBuffer[ui8Start + 6 + (ui8Idx * 3)];
		pSystem->MoistureSensor[ui8Idx].ThresholdDry = pBuffer[ui8Start + 7 + (ui8Idx * 3)];
		pSystem->MoistureSensor[ui8Idx].ThresholdWet = pBuffer[ui8Start + 8 + (ui8Idx * 3)];

		/** Read valve state from buffer */
		pSystem->Valve[ui8Idx].State = (bool) pBuffer[ui8Start + 18 + ui8Idx];
	}
}

/**
 *  ======== writeSystem ========
 *  Writes values of Plant Irrigation System.
 */
void writeSystem(uint8_t ui8CmdIdx, PlantIrrigationSystem_t* pSystem, uint8_t* pBuffer) {

	uint8_t		ui8Start;
	uint8_t		ui8Idx;

	/** Calculate start position of Plant Irrigation System object
	 * based on amount of commands.
	 */
	ui8Start = ui8CmdIdx + 1;

	/** Write pump data into buffer */
	pBuffer[ui8Start + 4] = pSystem->Pump.DutyCycle;
	pBuffer[ui8Start + 5] = pSystem->Pump.State;

	for(ui8Idx = 0; ui8Idx < 4; ui8Idx++) {

		/** Write state of automatic irrigation into buffer */
		pBuffer[ui8Start + ui8Idx] = pSystem->State[ui8Idx];

		/** Write data of moisutre sensor into buffer */
		pBuffer[ui8Start + 6 + (ui8Idx * 3)] = pSystem->MoistureSensor[ui8Idx].CurrentValue;
		pBuffer[ui8Start + 7 + (ui8Idx * 3)] = pSystem->MoistureSensor[ui8Idx].ThresholdDry;
		pBuffer[ui8Start + 8 + (ui8Idx * 3)] = pSystem->MoistureSensor[ui8Idx].ThresholdWet;

		/** Write valve state into buffer */
		pBuffer[ui8Start + 18 + ui8Idx] = pSystem->Valve[ui8Idx].State;
	}
}

/**
 *  ======== writeCommand ========
 *  Writes command for Plant Irrigation System.
 */
void writeCommand(uint8_t ui8Idx, uint8_t ui8Cmd, uint8_t* pBuffer) {

	/** Write command into buffer */
	pBuffer[ui8Idx] = ui8Cmd;

}
