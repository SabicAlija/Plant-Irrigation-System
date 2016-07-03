/**
 * Plant_Irrigation_Server.h
 *
 * Created on: 06.06.2016
 * Author: Alija Sabic
 * E-Mail: sabic.alija@gmail.com
 *  
 * GCopyright (C) 2016  Alija Sabic
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
#ifndef PLANTIRRIGATIONSYSTEM_PLANT_IRRIGATION_SERVER_H_
#define PLANTIRRIGATIONSYSTEM_PLANT_IRRIGATION_SERVER_H_

/** XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/** BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/** NDK Header files */
#include <sys/socket.h>

/** Standard Header files */
#include <string.h>

/** Size of incoming tcp packets in Byte. */
#define PLANTPACKETSIZE 256

#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <PlantIrrigationSystem/Plant_Irrigation_System.h>

/** Port used for Communication with Client
 * 	Server listens on this port.
 */
#define PLANTPORT 36363

/** TCP handler stack size */
#define PLANTIRRIGATIONSERVERSTACK 1024

/** Number of parallel request/workers */
#define NUMPLANTWORKERS 3

/** Prototypes */
/**
 *  @brief Open Hook function of Plant Irrigation Server.
 *  
 *  @return none.
 *  
 *  @details The open hook function is called after IP
 *  address is received.
 */
void PlantIrrigationServerOpenHook();

/**
 *  @brief Plant Irrigation Server handles incoming
 *  messages.
 *  
 *  @param [in] arg0 TCP Port Number
 *  @param [in] arg1 not in use
 *  @return none.
 *  
 *  @details The Plant Irrigation Server Task handles
 *  (listens on) incoming tcp packages and assigns each
 *  to an PlantWorker.
 */
Void PlantIrrigationServer(UArg arg0, UArg arg1);

/**
 *  @brief Plant Worker tasks handles incoming
 *  command request and resonse messages.
 *  
 *  @param [in] arg0 Client File Descriptor
 *  @param [in] arg1 not in use
 *  @return none.
 *  
 *  @details Plant Worker sends incoming messages
 *  to Plant Irrigation System and wait on a response
 *  message to transmit back to client.
 */
Void PlantWorker(UArg arg0, UArg arg1);

#endif /* PLANTIRRIGATIONSYSTEM_PLANT_IRRIGATION_SERVER_H_ */
