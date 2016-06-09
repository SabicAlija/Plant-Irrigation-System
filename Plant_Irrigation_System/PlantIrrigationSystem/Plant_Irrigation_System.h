/*
 * Plant_Irrigation_System.h
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
 */

#ifndef PLANTIRRIGATIONSYSTEM_PLANT_IRRIGATION_SYSTEM_H_
#define PLANTIRRIGATIONSYSTEM_PLANT_IRRIGATION_SYSTEM_H_

#include <stdint.h>
#include <stdbool.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/gates/GateMutex.h>

//------------------------------------------
// Plant Irrigation System - Hardware
//------------------------------------------
#include "drivers/tpic6a595_hal.h"
#include "drivers/boost_drv8848_hal.h"

//------------------------------------------
// Plant Irrigation System - Software
//------------------------------------------
#include "Plant_Irrigation_Server.h"

/**
 * Defines
 */
#define PLANT_IRRIGATION_LINE_INIT_STATE		false   /**< Initial state of automatic irrigation */
#define PUMP_DUTYCYCLE_INIT						100     /**< Initial dutycycle of the pump */
#define PUMP_STATE_INIT							0       /**< Initial state of the pump */
#define MOISTURE_SENSOR_CURRENT_VALUE_INIT		0       /**< Initial current moisture level */
#define MOISTURE_SENSOR_DRY_THRESHOLD_INIT		0       /**< Initial dry threshold of moisture sensor */
#define MOISTURE_SENSOR_WET_THRESHOLD_INIT		0       /**< Initial wet threshold of moisture sensor*/
#define VALVE_STATE_INIT						false   /**< Initial state of the valves */
#define PLANT_CMD_ECHO 							0       /**< Control Command: Echo */
#define PLANT_CMD_AUTOMAT_ON 					1       /**< Control Command: Automatic irrigation on */
#define PLANT_CMD_AUTOMAT_OFF 					2       /**< Control Command: Automatic irrigation off */
#define PLANT_CMD_VALVE_OPEN 					3       /**< Control Command: Open valve */
#define PLANT_CMD_VALVE_CLOSE 					4       /**< Control Command: Close valve */
#define PLANT_CMD_PUMP_ON 						5       /**< Control Command: Start pump */
#define PLANT_CMD_PUMP_OFF 						6       /**< Control Command: Stop pump */
#define PLANT_CMD_PUMP_DUTY_CYCLE 				7       /**< Control Command: Set pump dutycycle */
#define PLANT_CMD_MOISTURE_SENSOR 				8       /**< Control Command: Configure moisture sensor */
#define PLANTIRRIGATIONSYSTEMSTACK 				1024 	/**< Control Request handler stack size */


/**
 * Globals
 */
Mailbox_Handle 		PlantIrrigationMailboxRx;           /**< Receive Mailbox Handle: Control request */
Mailbox_Handle 		PlantIrrigationMailboxTx;           /**< Transmit Mailbox Handle: Control/Echo response */
GateMutex_Handle	PlantIrrigationSystemGate;          /**< Gate Handle for Plant Irrigation System access protection */
Event_Handle		PlantControlEvent;                  /**< Event Handle for Plant Irrigation System control request */

/**
 * Structures
 */
typedef struct Pump_t
{
	uint8_t				DutyCycle;
	uint8_t				State;
} Pump_t;

typedef struct MoistureSensor_t
{
	uint8_t				CurrentValue;
	uint8_t				ThresholdDry;
	uint8_t				ThresholdWet;
} MoistureSensor_t;

typedef struct Valve_t
{
	bool				State;
} Valve_t;

typedef struct PlantIrrigationSystem_t
{
	bool 				State[4];
	Pump_t 				Pump;
	MoistureSensor_t	MoistureSensor[4];
	Valve_t				Valve[4];
} PlantIrrigationSystem_t;

typedef struct MsgObj {
	int		clientfd;									/**< Client descriptor */
	int		length;										/**< Length of received message */
    uint8_t	buffer[PLANTPACKETSIZE]; 					/* receive buffer of Plant Irrigation Server */
} MsgObj, *Msg;

/**
 * Prototypes
 */
/**
 *  @brief Initialize Plant Irrigation System
 *  
 *  @param [out] pSystem Pointer to Plant Irrigation System to initialize.
 *  @return none.
 *  
 *  @details This prototype is used to initialize the global
 *  Irrigation System Structure.
 */
void initPlantIrrigationSystem(PlantIrrigationSystem_t* pSystem);


/**
 *  @brief Start Task that handles control request for Plant Irrigation System
 *  
 *  @param [out] pSystem Pointer to Plant Irrigation System
 *  @return none.
 *  
 *  @details This prototype is used to initialize the PlantIrrigationSystem
 *  Task that handles incoming commands from TCP clients in the network.
 */
void startPlantIrrigationSystem(PlantIrrigationSystem_t* pSystem);

/**
 *  @brief Plant Irrigation System Task that handles control requests.
 *  
 *  @param [in] arg0 Pointer to Plant Irrigation System
 *  @param [in] arg1 not in use
 *  @return Return_Description
 *  
 *  @details This prototype describes a handler for incoming
 *  control request for the Plant Irrigation System, which receives
 *  data per MsgBox.
 */
Void PlantIrrigationSystem(UArg arg0, UArg arg1);

/**
 *  @brief Reads values of intented Plant Irrigation System state.
 *  
 *  @param [in] ui8CmdIdx Command Counter
 *  @param [out] pSystem Pointer to Plant Irrigation System (Intent) Object
 *  @param [in] pBuffer Pointer to Receive Buffer
 *  @return Return_Description
 *  
 *  @details Reads values of intented Plant Irrigation System state from
 *  command none.
 */
void readSystem(uint8_t ui8CmdIdx, PlantIrrigationSystem_t* pSystem, uint8_t* pBuffer);

/**
 *  @brief Writes values Plant Irrigation System after execution.
 *  
 *  @param [in] ui8CmdIdx Command Counter
 *  @param [in] pSystem Pointer to Plant Irrigation System Object
 *  @param [out] pBuffer Pointer of Transmit Buffer
 *  @return none.
 *  
 *  @details Writes values of Plant Irrigation System in transmit
 *  buffer.
 */
void writeSystem(uint8_t ui8CmdIdx, PlantIrrigationSystem_t* pSystem, uint8_t* pBuffer);

/**
 *  @brief Writes command received for Plant Irrigation System
 *  after execution.
 *  
 *  @param [in] ui8Idx Current Command Counter
 *  @param [in] ui8Cmd Command Counter
 *  @param [out] pBuffer Pointer of Transmit Buffer
 *  @return none.
 */
void writeCommand(uint8_t ui8Idx, uint8_t ui8Cmd, uint8_t* pBuffer);


#endif /* PLANTIRRIGATIONSYSTEM_PLANT_IRRIGATION_SYSTEM_H_ */
