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
