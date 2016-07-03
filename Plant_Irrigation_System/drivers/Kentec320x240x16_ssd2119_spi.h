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
// Kentec320x240x16_ssd2119_spi.h - Prototypes for the Kentec K350QVG-V2-F
//                                  display driver with an SSD2119
//                                  controller.
//
// 
//
//*****************************************************************************

#ifndef __KENTEC320X240X16_SSD2119_SPI_H__
#define __KENTEC320X240X16_SSD2119_SPI_H__

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern uint32_t g_ui32SysClock;
extern void Kentec320x240x16_SSD2119Init(uint32_t ui32SysClockSpeed);
extern const tDisplay g_sKentec320x240x16_SSD2119;
extern void Kentec320x240x16_SSD2119SetLCDControl(uint8_t ui8Mask,
                                                  uint8_t ui8Val);
extern void Kentec_LED_ON(void);
extern void Kentec_LED_OFF(void);
#endif // __KENTEC320X240X16_SSD2119_SPI_H__
