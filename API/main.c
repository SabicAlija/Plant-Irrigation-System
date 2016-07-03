/*
 * main.c
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

//----------------------------------------
// BIOS header files
//----------------------------------------
#include <Board/Board.h>
#include <xdc/std.h>  						//mandatory - have to include first, for BIOS types
#include <ti/sysbios/BIOS.h> 				//mandatory - if you call APIs like BIOS_start()
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>				//needed for any Log_info() call
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h> 				//header file for statically defined objects/handles
#include <ti/drivers/GPIO.h>

//------------------------------------------
// NDK - bsd like sockets
//------------------------------------------
#include <sys/socket.h>

//------------------------------------------
// Standard Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
// TiveWare Peripherals
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
// TivaWare Graphic Library
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
// Utilities
#include "utils/ustdlib.h"

//------------------------------------------
// Display Header Files
//------------------------------------------
#include "drivers/Kentec320x240x16_ssd2119_spi.h"
#include "drivers/Kentec320x240x16_touch_tm4c1294xl.h"

//------------------------------------------
// Plant Irrigation Sytem - User Interface
//------------------------------------------
#include <PlantIrrigationSystem/PIS_UI.h>
#include <PlantIrrigationSystem/PIS_UI_Images.h>

//------------------------------------------
// Plant Irrigation Sytem
//------------------------------------------
#include <PlantIrrigationSystem/Plant_Irrigation_System.h>
#include <PlantIrrigationSystem/Plant_Irrigation_Server.h>

//------------------------------------------
// Plant Irrigation System - Hardware
//------------------------------------------
#include "drivers/tpic6a595_hal.h"



//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);

#define TITLE_XPOS		50
#define TITLE_YPOS		12
#define COPYRIGHT_XPOS	144
#define COPYRIGHT_YPOS	225


//---------------------------------------
// Globals
//---------------------------------------
//------------------------------------------------------------------------------
//
// System clock rate in Hz.
//
//------------------------------------------------------------------------------
uint32_t g_ui32SysClock;


//------------------------------------------------------------------------------
//
// Plant Irrigation System
//
//------------------------------------------------------------------------------
PlantIrrigationSystem_t g_System;



//------------------------------------------------------------------------------
//
// The DMA control structure table.
//
//------------------------------------------------------------------------------
#ifdef ewarm
#pragma data_alignment=1024
tDMAControlTable psDMAControlTable[64];
#elif defined(ccs)
#pragma DATA_ALIGN(i16DMAControlTable, 1024)
tDMAControlTable psDMAControlTable[64];
#else
tDMAControlTable psDMAControlTable[64] __attribute__ ((aligned(1024)));
#endif



//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{

	//
	// Initialize Plant Irrigation System Object
	//
	initPlantIrrigationSystem(&g_System);
	//startPlantIrrigationSystem(&g_System);


	//
	// Initialize Hardware via Xware
	//
	hardware_init();

	//
	// Initialize Board and Ethernet Driver
	//
	Board_initGeneral();
	Board_initGPIO();
	Board_initEMAC();

	//
	// Intialize valve control
	//
	initValveControl();

	//
	// Start TI-RTOS
	//
	BIOS_start();

}


//---------------------------------------------------------------------------
// hardware_init()
//
// initalize hardware for communication with display driver and for
// measuring TS pen position.
//---------------------------------------------------------------------------
void hardware_init(void)
{
    tContext sContext;
    tRectangle sRect;
    //uint32_t ui32Idx;

    //
    // The FPU should be enabled because some compilers will use floating-
    // point registers, even for non-floating-point code.  If the FPU is not
    // enabled this will cause a fault.  This also ensures that floating-
    // point operations could be added to this application and would work
    // correctly and use the hardware floating-point unit.  Finally, lazy
    // stacking is enabled for interrupt handlers.  This allows floating-
    // point instructions to be used within interrupt handlers, but at the
    // expense of extra stack usage.
    //
    FPUEnable();
    FPULazyStackingEnable();

    //
    // Run from the PLL at 120 MHz.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                SYSCTL_CFG_VCO_480), 120000000);

    //
    // Initialize the display driver.
    //
    Kentec320x240x16_SSD2119Init(g_ui32SysClock);

    //
    // Initialize the graphics context.
    //
    GrContextInit(&sContext, &g_sKentec320x240x16_SSD2119);

    //
    // Fill the screen with white color.
    //
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 239;
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &sRect);

    //
    // Put the application name in the middle of the banner.
    //
    GrContextForegroundSet(&sContext, ClrBlack);
    GrContextFontSet(&sContext, &g_sFontCm22);
    GrStringDraw(&sContext, "Plant Irrigation System", -1, TITLE_XPOS, TITLE_YPOS, 0);
    GrContextFontSet(&sContext, &g_sFontCm12);
    GrStringDraw(&sContext, "Copyright (c) 2015 Sabic Alija", -1, COPYRIGHT_XPOS, COPYRIGHT_YPOS, 0);

    //
    // Configure and enable uDMA
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlDelay(10);
    uDMAControlBaseSet(&psDMAControlTable[0]);
    uDMAEnable();

    //
    // Initialize the touch screen driver and have it route its messages to the
    // widget tree.
    //
    TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(WidgetPointerMessage);

    //
    // Add the first panel to the widget tree.
    //
    g_ui32Panel = 0;
    WidgetAdd(WIDGET_ROOT, (tWidget *)g_psPanels);

    //
    // Issue the initial paint request to the widgets.
    //
    WidgetPaint(WIDGET_ROOT);
    WidgetMessageQueueProcess();
}


//---------------------------------------------------------------------------
// init_PlantIrrigationSystem()
//
// Initialize PlantIrrigationSystem Task, Event and Mailbox
//---------------------------------------------------------------------------
void init_PlantIrrigationSystem(void)
{
	//
	// Start Plant Irrigation System and enable control
	// request handling on Port 36363
	//
	startPlantIrrigationSystem(&g_System);
}


//---------------------------------------------------------------------------
// update_LCD()
//
// Process message in the widget message queue frequently
//---------------------------------------------------------------------------
void update_LCD(void)
{
	while(1)
	{
		// +++++++++++++++++++++++++++++++++++++++ LCD +++++++++++++++++++++++++++++++++++
		//gateLCDKey = GateHwi_enter(gateLCD);
		Hwi_disable();

		WidgetMessageQueueProcess();

		//GateHwi_leave(gateLCD, gateLCDKey);
		Hwi_enable();
		// +++++++++++++++++++++++++++++++++++++++ LCD +++++++++++++++++++++++++++++++++++

		Task_sleep(200);
	}
}
