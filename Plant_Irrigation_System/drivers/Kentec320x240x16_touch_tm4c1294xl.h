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
//*****************************************************************************
//
// Kentec320x240x16_touch_tm4c1294xl.h - Prototypes for the touch screen driver.
//
//
//*****************************************************************************

#ifndef __TOUCH_TM4C1294XL_H__
#define __TOUCH_TM4C1294XL_H__

//*****************************************************************************
//
// The lowest ADC reading assumed to represent a press on the screen.  Readings
// below this indicate no press is taking place.
//
//*****************************************************************************
#define TOUCH_MIN 300

//*****************************************************************************
//
// Prototypes for the functions exported by the touch screen driver.
//
//*****************************************************************************
extern volatile int16_t g_i16TouchX;
extern volatile int16_t g_i16TouchY;
extern int16_t g_i16TouchMin;
extern void TouchScreenIntHandler(void);
extern void TouchScreenIntHandler(void);
extern void TouchScreenInit(uint32_t ui32SysClock);
extern void TouchScreenCallbackSet(int32_t (*pfnCallback)(uint32_t ui32Message,
                                                          int32_t i32X,
                                                          int32_t i32Y));

#endif // __TOUCH_TM4C1294XL_H__
