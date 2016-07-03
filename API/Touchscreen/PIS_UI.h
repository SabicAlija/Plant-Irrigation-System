/**
 * PIS_UI.h
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
#include <stdint.h>
#include <stdbool.h>
#include <ti/sysbios/gates/GateMutex.h>

#include "../PlantIrrigationSystem/PIS_UI_Images.h"
#include "../PlantIrrigationSystem/Plant_Irrigation_System.h"
#include "drivers/tpic6a595_hal.h"
#include "drivers/boost_drv8848_hal.h"

#ifndef UTILS_PIS_UI_H_
#define UTILS_PIS_UI_H_


extern GateMutex_Handle PlantIrrigationSystemGate;

//*****************************************************************************
//
// PANEL Home #define
//
//*****************************************************************************
#define PLANT_ICON_WIDTH			60
#define PLANT_ICON_HEIGHT			100
#define PLANT_ICON_YPOS				50
#define PLANT_ICON_SPACE			10

#define PLANT_IRR_ICON_HEIGHT		25
#define PLANT_IRR_ICON_YPOS			155

#define PLANT_CALIB_ICON_HEIGHT		25
#define PLANT_CALIB_ICON_YPOS		185

#define PLANT1_ICON_XPOS			25

#define PANEL_FRAME_XPOS			5
#define PANEL_FRAME_YPOS			40
#define PANEL_FRAME_WIDTH			310
#define PANEL_FRAME_HEIGHT			185


//*****************************************************************************
//
// PANEL Calibration #define
//
//*****************************************************************************
#define PANEL_CALIB_ICON_XPOS		20
#define PANEL_CALIB_ICON_YPOS		50

#define PANEL_CALIB_ICON_WIDTH		100
#define PANEL_CALIB_ICON_HEIGHT		167

#define PANEL_CALIB_FRAME_WIDTH		80
#define PANEL_CALIB_FRAME_HEIGHT	25


//*****************************************************************************
//
// PANEL Watering #define
//
//*****************************************************************************
#define PANEL_WATERING_ICON_XPOS	20
#define PANEL_WATERING_ICON_YPOS	50

#define PANEL_WATERING_ICON_WIDTH	100
#define PANEL_WATERING_ICON_HEIGHT	167

#define PANEL_WATERING_FRAME_WIDTH	60
#define PANEL_WATERING_FRAME_HEIGHT	25

extern PlantIrrigationSystem_t	g_System;
extern tPushButtonWidget g_psPlantIcons[];
extern tPushButtonWidget g_psPlantIrrigateIcons[];
extern tPushButtonWidget g_psPlantCalibrateIcons[];
extern tCanvasWidget g_psCalibrationIndicators[];
extern tCanvasWidget g_psPanels[];
extern tPushButtonWidget g_psPushButtonsCalibrate[];
extern tCanvasWidget g_psWateringIndicators[];
extern tPushButtonWidget g_psPushButtonsWatering[];
extern tSliderWidget g_psWateringSliders[];

//*****************************************************************************
//
// Forward declarations for the globals required to define the widgets at
// compile-time.
//
//*****************************************************************************
void OnPlantIcon(tWidget *psWidget);
void OnPlantIrrigateIcon(tWidget *psWidget);
void OnPlantCalibIcon(tWidget *psWidget);
void OnBack(tWidget *psWidget);
void OnValve(tWidget *psWidget);
void OnPump(tWidget *psWidget);
void OnSlidePump(tWidget *psWidget, int32_t i32Value);


//*****************************************************************************
//
// Current line selected.
//
//*****************************************************************************
uint8_t	g_ui8ActiveLine;

//*****************************************************************************
//
// The first panel, which contains introductory text explaining the
// application.
//
//*****************************************************************************
tPushButtonWidget g_psPlantIcons[] =
{
	RectangularButtonStruct(g_psPanels, g_psPlantIcons + 1, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS, PLANT_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrGray,
		ClrBlack, 0, 0, g_pui8Plant1IconGray, g_pui8Plant1Icon, 0, 0, OnPlantIcon),
	RectangularButtonStruct(g_psPanels, g_psPlantIcons + 2, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (1 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrGray,
		ClrBlack, 0, 0, g_pui8Plant2IconGray, g_pui8Plant2Icon, 0, 0, OnPlantIcon),
	RectangularButtonStruct(g_psPanels, g_psPlantIcons + 3, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (2 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrGray,
		ClrBlack, 0, 0, g_pui8Plant3IconGray, g_pui8Plant3Icon, 0, 0, OnPlantIcon),
	RectangularButtonStruct(g_psPanels, g_psPlantIrrigateIcons, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (3 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrGray,
		ClrBlack, 0, 0, g_pui8Plant4IconGray, g_pui8Plant4Icon, 0, 0, OnPlantIcon),
};
#define NUM_PLANT_ICONS		    (sizeof(g_psPlantIcons) /   \
								 sizeof(g_psPlantIcons[0]))

tPushButtonWidget g_psPlantIrrigateIcons[] =
{
	RectangularButtonStruct(g_psPanels, g_psPlantIrrigateIcons + 1, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS, PLANT_IRR_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_IRR_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrWhiteSmoke,
		ClrBlack, 0, 0, g_pui8PlantIrrigateIcon, g_pui8PlantIrrigateIcon, 0, 0, OnPlantIrrigateIcon),
	RectangularButtonStruct(g_psPanels, g_psPlantIrrigateIcons + 2, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (1 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_IRR_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_IRR_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrWhiteSmoke,
		ClrBlack, 0, 0, g_pui8PlantIrrigateIcon, g_pui8PlantIrrigateIcon, 0, 0, OnPlantIrrigateIcon),
	RectangularButtonStruct(g_psPanels, g_psPlantIrrigateIcons + 3, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (2 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_IRR_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_IRR_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrWhiteSmoke,
		ClrBlack, 0, 0, g_pui8PlantIrrigateIcon, g_pui8PlantIrrigateIcon, 0, 0, OnPlantIrrigateIcon),
	RectangularButtonStruct(g_psPanels, g_psPlantCalibrateIcons, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (3 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_IRR_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_IRR_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrWhiteSmoke,
		ClrBlack, 0, 0, g_pui8PlantIrrigateIcon, g_pui8PlantIrrigateIcon, 0, 0, OnPlantIrrigateIcon),
};

tPushButtonWidget g_psPlantCalibrateIcons[] =
{
	RectangularButtonStruct(g_psPanels, g_psPlantCalibrateIcons + 1, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS, PLANT_CALIB_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_CALIB_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrWhiteSmoke,
		ClrBlack, 0, 0, g_pui8PlantCalibrateIcon, g_pui8PlantCalibrateIcon, 0, 0, OnPlantCalibIcon),
	RectangularButtonStruct(g_psPanels, g_psPlantCalibrateIcons + 2, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (1 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_CALIB_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_CALIB_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrWhiteSmoke,
		ClrBlack, 0, 0, g_pui8PlantCalibrateIcon, g_pui8PlantCalibrateIcon, 0, 0, OnPlantCalibIcon),
	RectangularButtonStruct(g_psPanels, g_psPlantCalibrateIcons + 3, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (2 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_CALIB_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_CALIB_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrWhiteSmoke,
		ClrBlack, 0, 0, g_pui8PlantCalibrateIcon, g_pui8PlantCalibrateIcon, 0, 0, OnPlantCalibIcon),
	RectangularButtonStruct(g_psPanels, 0, 0, &g_sKentec320x240x16_SSD2119,
		PLANT1_ICON_XPOS + (3 * (PLANT_ICON_WIDTH + PLANT_ICON_SPACE)), PLANT_CALIB_ICON_YPOS, PLANT_ICON_WIDTH, PLANT_CALIB_ICON_HEIGHT,
		PB_STYLE_IMG | PB_STYLE_OUTLINE, ClrWhite, ClrWhite, ClrWhiteSmoke,
		ClrBlack, 0, 0, g_pui8PlantCalibrateIcon, g_pui8PlantCalibrateIcon, 0, 0, OnPlantCalibIcon),
};

//*****************************************************************************
//
// The second panel, which contains the calibration view
//
//
//*****************************************************************************
tCanvasWidget g_psCalibrationIndicators[] =
{
	CanvasStruct(g_psPanels + 1, g_psCalibrationIndicators + 1, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS, PANEL_CALIB_ICON_YPOS, PANEL_CALIB_ICON_WIDTH, PANEL_CALIB_ICON_HEIGHT,
				 CANVAS_STYLE_IMG, 0, 0, 0, 0, 0, g_pui8Plant1IconLarge, 0),
	CanvasStruct(g_psPanels + 1, g_psCalibrationIndicators + 2, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH + PLANT_ICON_SPACE, 50, PANEL_CALIB_FRAME_WIDTH*2 + PLANT_ICON_SPACE, PANEL_CALIB_FRAME_HEIGHT,
				 CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_LEFT, ClrWhite, 0, ClrBlack, &g_sFontCm12, "Moisture Measurement Result", 0, 0),
	CanvasStruct(g_psPanels + 1,  g_psCalibrationIndicators + 3, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH + PLANT_ICON_SPACE, 80, PANEL_CALIB_FRAME_WIDTH, PANEL_CALIB_FRAME_HEIGHT,
				 CANVAS_STYLE_OUTLINE, 0, ClrGray, 0, 0, 0, 0, 0),
	CanvasStruct(g_psPanels + 1, g_psPushButtonsCalibrate, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH + PLANT_ICON_SPACE, 110, PANEL_CALIB_FRAME_WIDTH*2 + PLANT_ICON_SPACE, PANEL_CALIB_FRAME_HEIGHT,
				 CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_LEFT, ClrWhite, 0, ClrBlack, &g_sFontCm12, "Calibrate", 0, 0),
};

tPushButtonWidget g_psPushButtonsCalibrate[] =
{
	RectangularButtonStruct(g_psPanels + 1, g_psPushButtonsCalibrate + 1, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH*2 + PLANT_ICON_SPACE/4 + 45, 180, 35, 25,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "+", 0, 0, 0, 0, 0),
	RectangularButtonStruct(g_psPanels + 1, g_psPushButtonsCalibrate + 2, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH*2 + PLANT_ICON_SPACE/4, 180, 35, 25,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "-", 0, 0, 0, 0, 0),
    RectangularButtonStruct(g_psPanels + 1, g_psPushButtonsCalibrate + 3, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH*2 + PLANT_ICON_SPACE/4, 80, PANEL_CALIB_FRAME_WIDTH, PANEL_CALIB_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "REFRESH", 0, 0, 0, 0, 0),
	RectangularButtonStruct(g_psPanels + 1, g_psPushButtonsCalibrate + 4, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH + PLANT_ICON_SPACE, 140, PANEL_CALIB_FRAME_WIDTH, PANEL_CALIB_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "DRY", 0, 0, 0, 0, 0),
	RectangularButtonStruct(g_psPanels + 1, g_psPushButtonsCalibrate + 5, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH*2 + PLANT_ICON_SPACE/4, 140, PANEL_CALIB_FRAME_WIDTH, PANEL_CALIB_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "WET", 0, 0, 0, 0, 0),
	RectangularButtonStruct(g_psPanels + 1, 0, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS + PANEL_CALIB_ICON_WIDTH + PLANT_ICON_SPACE, 180, PANEL_CALIB_FRAME_WIDTH, PANEL_CALIB_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrLightGrey, ClrGray, ClrBlack, ClrWhite,
				&g_sFontCm16, "BACK", 0, 0, 0, 0, OnBack),
};
#define NUM_PUSH_CALIBRATE_BUTTONS        (sizeof(g_psPushButtonsCalibrate) /   \
                                 	 	  sizeof(g_psPushButtonsCalibrate[0]))


//*****************************************************************************
//
// The third panel, which contains the watering view
//
//
//*****************************************************************************
tCanvasWidget g_psWateringIndicators[] =
{
	CanvasStruct(g_psPanels + 1, g_psWateringIndicators + 1, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_CALIB_ICON_XPOS, PANEL_CALIB_ICON_YPOS, PANEL_CALIB_ICON_WIDTH, PANEL_CALIB_ICON_HEIGHT,
				 CANVAS_STYLE_IMG, 0, 0, 0, 0, 0, g_pui8Plant1IconLarge, 0),
	CanvasStruct(g_psPanels + 2, g_psWateringIndicators + 2, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH + PLANT_ICON_SPACE, 50, PANEL_CALIB_FRAME_WIDTH*2 + PLANT_ICON_SPACE, PANEL_CALIB_FRAME_HEIGHT,
				 CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_LEFT, ClrWhite, 0, ClrBlack, &g_sFontCm12, "Valve", 0, 0),
	CanvasStruct(g_psPanels + 2, g_psWateringIndicators + 3, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH + PLANT_ICON_SPACE, 105, PANEL_CALIB_FRAME_WIDTH*2 + PLANT_ICON_SPACE, PANEL_CALIB_FRAME_HEIGHT,
				 CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_LEFT, ClrWhite, 0, ClrBlack, &g_sFontCm12, "Pump", 0, 0),
	CanvasStruct(g_psPanels + 2, g_psWateringIndicators + 4, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH*2 + PLANT_ICON_SPACE/4 - 15, 160, PANEL_CALIB_FRAME_WIDTH*2 + PLANT_ICON_SPACE, PANEL_CALIB_FRAME_HEIGHT,
				 CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_LEFT, ClrWhite, 0, ClrBlack, &g_sFontCm12, "Moisture", 0, 0),
	CanvasStruct(g_psPanels + 2, g_psPushButtonsWatering, 0,
				 &g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH*2 - 15, 180, PANEL_WATERING_FRAME_WIDTH, PANEL_WATERING_FRAME_HEIGHT,
				 CANVAS_STYLE_OUTLINE, 0, ClrGray, 0, 0, 0, 0, 0),
};

tPushButtonWidget g_psPushButtonsWatering[] =
{
	RectangularButtonStruct(g_psPanels + 2, g_psPushButtonsWatering + 1, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH + PLANT_ICON_SPACE, 75, PANEL_WATERING_FRAME_WIDTH, PANEL_WATERING_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "OPEN", 0, 0, 0, 0, OnValve),
	RectangularButtonStruct(g_psPanels + 2, g_psPushButtonsWatering + 2, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH*2 - 15, 75, PANEL_WATERING_FRAME_WIDTH, PANEL_WATERING_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "CLOSE", 0, 0, 0, 0, OnValve),
    RectangularButtonStruct(g_psPanels + 2, g_psPushButtonsWatering + 3, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH + PLANT_ICON_SPACE, 130, PANEL_WATERING_FRAME_WIDTH, PANEL_WATERING_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "ON", 0, 0, 0, 0, OnPump),
	RectangularButtonStruct(g_psPanels + 2, g_psPushButtonsWatering + 4, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH*2 - 15, 130, PANEL_WATERING_FRAME_WIDTH, PANEL_WATERING_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrWhiteSmoke, ClrLightGrey, ClrBlack, ClrBlack,
				&g_sFontCm16, "OFF", 0, 0, 0, 0, OnPump),
	RectangularButtonStruct(g_psPanels + 2, g_psWateringSliders, 0,
				&g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH + PLANT_ICON_SPACE, 180, PANEL_WATERING_FRAME_WIDTH, PANEL_WATERING_FRAME_HEIGHT,
				PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT,
				ClrLightGrey, ClrGray, ClrBlack, ClrWhite,
				&g_sFontCm16, "BACK", 0, 0, 0, 0, OnBack),
};
#define NUM_PUSH_WATERING_BUTTONS        (sizeof(g_psPushButtonsWatering) /   \
                                 	 	  sizeof(g_psPushButtonsWatering[0]))

tSliderWidget g_psWateringSliders[] =
{
    SliderStruct(g_psPanels + 2, 0, 0,
                 &g_sKentec320x240x16_SSD2119, PANEL_WATERING_ICON_XPOS + PANEL_WATERING_ICON_WIDTH*3 - 40, 75, 25, 130, 0, 100, 100,
                 (SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE | SL_STYLE_VERTICAL),
				 ClrLightSkyBlue, ClrAzure, ClrSilver, 0, 0,
				 0, 0, 0, 0, OnSlidePump),
};

//*****************************************************************************
//
//
//
//*****************************************************************************
tCanvasWidget g_psPanels[] =
{
	CanvasStruct(0, 0, &g_psPlantIcons, &g_sKentec320x240x16_SSD2119, PANEL_FRAME_XPOS, PANEL_FRAME_YPOS,
			PANEL_FRAME_WIDTH, PANEL_FRAME_HEIGHT, CANVAS_STYLE_FILL, ClrWhite, ClrGray, 0, 0, 0, 0, 0),
	CanvasStruct(0, 0, &g_psCalibrationIndicators, &g_sKentec320x240x16_SSD2119, PANEL_FRAME_XPOS, PANEL_FRAME_YPOS,
			PANEL_FRAME_WIDTH, PANEL_FRAME_HEIGHT, CANVAS_STYLE_FILL, ClrWhite, ClrGray, 0, 0, 0, 0, 0),
	CanvasStruct(0, 0, &g_psWateringIndicators, &g_sKentec320x240x16_SSD2119, PANEL_FRAME_XPOS, PANEL_FRAME_YPOS,
			PANEL_FRAME_WIDTH, PANEL_FRAME_HEIGHT, CANVAS_STYLE_FILL, ClrWhite, ClrGray, 0, 0, 0, 0, 0),
};
//*****************************************************************************
//
// The number of panels.
//
//*****************************************************************************
#define NUM_PANELS              (sizeof(g_psPanels) / sizeof(g_psPanels[0]))

//*****************************************************************************
//
// The panel that is currently being displayed.
//
//*****************************************************************************
uint32_t g_ui32Panel;

//*****************************************************************************
//
// Handles presses of each plant icon button.
//
//*****************************************************************************
void
OnPlantIcon(tWidget *psWidget)
{
	uint32_t ui32Idx;

    //
    // Find the index of this slider.
    //
    for(ui32Idx = 0; ui32Idx < NUM_PLANT_ICONS; ui32Idx++)
    {
        if(psWidget == (tWidget *)(g_psPlantIcons + ui32Idx))
        {
            break;
        }
    }

    //
    // Toggle Valve State
    //
    g_System.State[ui32Idx] = !g_System.State[ui32Idx];

    WidgetPaint(WIDGET_ROOT);
    WidgetMessageQueueProcess();
}

//*****************************************************************************
//
// Handles presses of each plant irrigate icon button.
//
//*****************************************************************************
void
OnPlantIrrigateIcon(tWidget *psWidget)
{
	const uint8_t* ui8Picture;
	uint32_t ui32Idx;

	//
	// Find the index of this check box.
	//
	for(ui32Idx = 0; ui32Idx < NUM_PLANT_ICONS; ui32Idx++)
	{
		if(psWidget == (tWidget *)(g_psPlantIrrigateIcons + ui32Idx))
		{
			break;
		}
	}

	//
	// Return if the check box could not be found.
	//
	if(ui32Idx == NUM_PLANT_ICONS)
	{
		return;
	}

	g_ui8ActiveLine = (uint8_t) ui32Idx;

	switch(ui32Idx)
	{
	case 0:
		ui8Picture = g_pui8Plant1IconLarge;
		break;
	case 1:
		ui8Picture = g_pui8Plant2IconLarge;
		break;
	case 2:
		ui8Picture = g_pui8Plant3IconLarge;
		break;
	case 3:
		ui8Picture = g_pui8Plant4IconLarge;
		break;
	default:
		break;
	}

	//
	// There is nothing to be done if the last panel is already being
	// displayed.
	//
	if(g_ui32Panel == (NUM_PANELS - 1))
	{
		return;
	}

	//
	// Remove the current panel.
	//
	WidgetRemove((tWidget *)(g_psPanels + g_ui32Panel));

	//
	// Increment the panel index.
	//
	g_ui32Panel = 2;

	//
	// Add and draw the new panel.
	//
	WidgetAdd(WIDGET_ROOT, (tWidget *)(g_psPanels + g_ui32Panel));

	CanvasImageSet((tCanvasWidget *) g_psWateringIndicators, ui8Picture);

	WidgetPaint((tWidget *)(g_psPanels + g_ui32Panel));

}

//*****************************************************************************
//
// Handles presses of each plant calibrate icon button.
//
//*****************************************************************************
void
OnPlantCalibIcon(tWidget *psWidget)
{
	const uint8_t* ui8Picture;
    uint32_t ui32Idx;

    //
    // Find the index of this check box.
    //
    for(ui32Idx = 0; ui32Idx < NUM_PLANT_ICONS; ui32Idx++)
    {
        if(psWidget == (tWidget *)(g_psPlantCalibrateIcons + ui32Idx))
        {
            break;
        }
    }

    //
    // Return if the check box could not be found.
    //
    if(ui32Idx == NUM_PLANT_ICONS)
    {
        return;
    }

	switch(ui32Idx)
	{
	case 0:
		ui8Picture = g_pui8Plant1IconLarge;
		break;
	case 1:
		ui8Picture = g_pui8Plant2IconLarge;
		break;
	case 2:
		ui8Picture = g_pui8Plant3IconLarge;
		break;
	case 3:
		ui8Picture = g_pui8Plant4IconLarge;
		break;
	default:
		break;
	}

	//
	// There is nothing to be done if the last panel is already being
	// displayed.
	//
	if(g_ui32Panel == (NUM_PANELS - 1))
	{
		return;
	}

	//
	// Remove the current panel.
	//
	WidgetRemove((tWidget *)(g_psPanels + g_ui32Panel));

	//
	// Increment the panel index.
	//
	g_ui32Panel = 1;

	//
	// Add and draw the new panel.
	//
	WidgetAdd(WIDGET_ROOT, (tWidget *)(g_psPanels + g_ui32Panel));

	CanvasImageSet((tCanvasWidget *) g_psCalibrationIndicators, ui8Picture);

	WidgetPaint((tWidget *)(g_psPanels + g_ui32Panel));


}

//*****************************************************************************
//
// Handles presses of each back button.
//
//*****************************************************************************
void
OnBack(tWidget *psWidget)
{
	//
	// Remove the current panel.
	//
	WidgetRemove((tWidget *)(g_psPanels + g_ui32Panel));

	//
	// Increment the panel index.
	//
	g_ui32Panel = 0;

	//
	// Add and draw the new panel.
	//
	WidgetAdd(WIDGET_ROOT, (tWidget *)(g_psPanels + g_ui32Panel));
	WidgetPaint((tWidget *)(g_psPanels + g_ui32Panel));
}


//*****************************************************************************
//
// Handles presses of valve button on watering panel.
//
//*****************************************************************************
void
OnValve(tWidget *psWidget)
{
	int 	action;
	UInt 	plantGateKey;

	//
	// Button: OPEN
	//
	if(psWidget == (tWidget *) (g_psPushButtonsWatering))
		action = 1;

	//
	// Button: CLOSE
	//
	else if(psWidget == (tWidget *) (g_psPushButtonsWatering + 1))
		action = 0;

	//
	// Default
	//
	else
		action = -1;


	switch(action) {

		// OPEN
		case 1:
			// Enter critical region. Access Plant Irrigation System.
			plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

			// Check if valve is open
			if(!g_System.Valve[g_ui8ActiveLine].State) {

				// Set current status.
				g_System.Valve[g_ui8ActiveLine].State = true;

				// Handle active line individually.
				switch(g_ui8ActiveLine) {

					case 0:
						switchValve(TPIC6A595_VALVE_1);
						g_System.Valve[1].State = false;
						g_System.Valve[2].State = false;
						g_System.Valve[3].State = false;
						break;

					case 1:
						switchValve(TPIC6A595_VALVE_2);
						g_System.Valve[0].State = false;
						g_System.Valve[2].State = false;
						g_System.Valve[3].State = false;
						break;

					case 2:
						switchValve(TPIC6A595_VALVE_3);
						g_System.Valve[0].State = false;
						g_System.Valve[1].State = false;
						g_System.Valve[3].State = false;
						break;

					case 3:
						switchValve(TPIC6A595_VALVE_4);
						g_System.Valve[0].State = false;
						g_System.Valve[1].State = false;
						g_System.Valve[2].State = false;
						break;

					default:
						break;
				}
			}

			// Exit critical region.
			GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
			break;

		// CLOSE
		case 0:
			// Enter critical region. Access Plant Irrigation System.
			plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

			//
			// Close valve of active line.
			//
			g_System.Valve[g_ui8ActiveLine].State = false;
			switchValve(TPIC6A595_CLOSE_VALVES);

			// Exit critical region.
			GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
			break;

		default:
			break;

	}
}

//*****************************************************************************
//
// Handles presses of pump button on watering panel.
//
//*****************************************************************************
void
OnPump(tWidget *psWidget)
{
	int 	action;
	UInt 	plantGateKey;

	//
	// Button: ON
	//
	if(psWidget == (tWidget *) (g_psPushButtonsWatering + 2))
		action = 1;

	//
	// Button: OFF
	//
	else if(psWidget == (tWidget *) (g_psPushButtonsWatering + 3))
		action = 0;

	//
	// Default
	//
	else
		action = -1;


	switch(action) {

		// ON
		case 1:
			// Enter critical region. Access Plant Irrigation System.
			plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

			// Check if Valve of active line is open.
			if(g_System.Valve[g_ui8ActiveLine].State) {

				//
				// Turn on pump.
				//
				g_System.Pump.State = 1;

				//
				// Currently pump in use supports only Dutycylce
				// of 100% (!).
				//
				initPumpControl(g_System.Pump.DutyCycle);
			}

			// Exit critical region.
			GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
			break;

		// OFF
		case 0:
			// Enter critical region. Access Plant Irrigation System.
			plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

			//
			// Turn off pump.
			//
			g_System.Pump.State = 0;
			stopPump();

			// Exit critical region.
			GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);
			break;

		default:
			break;
	}
}



//*****************************************************************************
//
// Handles slider events for pump duty cycle on watering panel.
//
//*****************************************************************************
void OnSlidePump(tWidget *psWidget, int32_t i32Value)
{
	UInt plantGateKey;

	// Enter critical region. Access Plant Irrigation System.
	plantGateKey = GateMutex_enter(PlantIrrigationSystemGate);

	//
	// Set pump dutycycle.
	//
	g_System.Pump.DutyCycle = i32Value;

	// Exit critical region.
	GateMutex_leave(PlantIrrigationSystemGate, plantGateKey);

}


#endif /* UTILS_PIS_UI_H_ */
