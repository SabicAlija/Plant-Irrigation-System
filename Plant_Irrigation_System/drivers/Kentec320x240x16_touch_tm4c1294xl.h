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
