//*****************************************************************************
//
// Kentec320x240x16_ssd2119_spi.c - Display driver for the Kentec K350QVG-V2-F
//                                  TFT display with an SSD2119 controller.
//                                  This version assumes an SPI (Master-Slave)
//                                  interface between the micro and display.
//                                   
//
//                 TM4C1294XL                 BOOSTXL-K350QVG-S1
//                -----------------              ------------
//             ^ |     PQ2/SSI3Tx	|---------> |LCD_SDI     |
//            /|\|                  |           |            |
//             | |     PQ0/SSI3Clk	|---------> |LCD_SCL     |
//             	 |	               	|           |            |
//               |     PP3			|---------> |LCD_SCS     |
//               |     PP4			|---------> |LCD_SDC     |
//               |     PK6			|---------> |LCD_RESET   |
//               |     PG1/M0PWM5	|---------> |LCD_PWM     |
//               |                  |            ------------
//               |                  |
//               |     PN1          |--------->  USER_LED_1      
//               |     PN0          |--------->  USER_LED_2
//               |                  |            
//                ------------------
//
//
// Copyright (c) 2015 Sabic Alija
// 
//
//*****************************************************************************

//*****************************************************************************
//
// Standard Header
//
//*****************************************************************************
#include <stdbool.h>
#include <stdint.h>

//*****************************************************************************
//
// TI Hardware Header
//
//*****************************************************************************
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

//*****************************************************************************
//
// TI Peripherals Header
//
//*****************************************************************************
#include "driverlib/gpio.h"
#include "driverlib/epi.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "driverlib/ssi.h"

//*****************************************************************************
//
// TI Graphics Header
//
//*****************************************************************************
#include "grlib/grlib.h"

//*****************************************************************************
//
// Custom Display Header
//
//*****************************************************************************
#include "Kentec320x240x16_ssd2119_spi.h"

//*****************************************************************************
//
// This driver operates in four different screen orientations.  They are:
//
// * Portrait - The screen is taller than it is wide, and the flex connector is
//              on the left of the display.  This is selected by defining
//              PORTRAIT.
//
// * Landscape - The screen is wider than it is tall, and the flex connector is
//               on the bottom of the display.  This is selected by defining
//               LANDSCAPE.
//
// * Portrait flip - The screen is taller than it is wide, and the flex
//                   connector is on the right of the display.  This is
//                   selected by defining PORTRAIT_FLIP.
//
// * Landscape flip - The screen is wider than it is tall, and the flex
//                    connector is on the top of the display.  This is
//                    selected by defining LANDSCAPE_FLIP.
//
// These can also be imagined in terms of screen rotation; if portrait mode is
// 0 degrees of screen rotation, landscape is 90 degrees of counter-clockwise
// rotation, portrait flip is 180 degrees of rotation, and landscape flip is
// 270 degress of counter-clockwise rotation.
//
// If no screen orientation is selected, "landscape flip" mode will be used.
//
//*****************************************************************************
#if ! defined(PORTRAIT) && ! defined(PORTRAIT_FLIP) && \
    ! defined(LANDSCAPE) && ! defined(LANDSCAPE_FLIP)
#define LANDSCAPE
//#define PORTRAIT
#endif


//*****************************************************************************
//
// Various definitions controlling coordinate space mapping and drawing
// direction in the four supported orientations.
//
//*****************************************************************************
#ifdef PORTRAIT
#define HORIZ_DIRECTION 0x28
#define VERT_DIRECTION 0x20
#define MAPPED_X(x, y) (319 - (y))
#define MAPPED_Y(x, y) (x)
#endif
#ifdef LANDSCAPE
#define HORIZ_DIRECTION 0x00
#define VERT_DIRECTION  0x08
#define MAPPED_X(x, y) (319 - (x))
#define MAPPED_Y(x, y) (239 - (y))
#endif
#ifdef PORTRAIT_FLIP
#define HORIZ_DIRECTION 0x18
#define VERT_DIRECTION 0x10
#define MAPPED_X(x, y) (y)
#define MAPPED_Y(x, y) (239 - (x))
#endif
#ifdef LANDSCAPE_FLIP
#define HORIZ_DIRECTION 0x30
#define VERT_DIRECTION  0x38
#define MAPPED_X(x, y) (x)
#define MAPPED_Y(x, y) (y)
#endif

//*****************************************************************************
//
// Defines for the pins that are used to communicate with the SSD2119.
//
//*****************************************************************************
#define HAL_LCD_MCLK_FREQUENCY      	120000000				// MCU frequency

#define GRLIB_TM4C_MODE           		1

#define BOOSTERPACK_2
#ifdef BOOSTERPACK_1
	//*****************************************************************************
	//
	// BOOSTERPACK 1
	//
	//*****************************************************************************
	// Peripherals from TM4C1294XL connected to LCD
	#define LCD_SSI_PERIPH					SYSCTL_PERIPH_SSI2		// SSI2 Module
	#define LCD_SDI_PERIPH     				SYSCTL_PERIPH_GPIOD		// SSI2 Module
	#define LCD_SCL_PERIPH     				SYSCTL_PERIPH_GPIOD		// SSI2 Module
	#define LCD_SCS_PERIPH     				SYSCTL_PERIPH_GPION		// Chip Select
	#define LCD_SDC_PERIPH     				SYSCTL_PERIPH_GPIOC		// Display Command
	#define LCD_RESET_PERIPH   				SYSCTL_PERIPH_GPIOL		// Reset
	#define LCD_BACKLIGHT_PERIPH     		SYSCTL_PERIPH_GPIOF		// Backlight
	#define LCD_USER_LED_PERIPH				SYSCTL_PERIPH_GPION		// User LED	


	// Ports/Modules from TM4C1294XL connected to LCD
	#define LCD_SSI_MODULE					SSI2_BASE				// SSI2 Module
	#define LCD_SDI_PORT       				GPIO_PORTD_BASE			// SSI2 Module
	#define LCD_SCL_PORT       				GPIO_PORTD_BASE         // SSI2 Module
	#define LCD_SCS_PORT       				GPIO_PORTN_BASE         // Chip Select
	#define LCD_SDC_PORT       				GPIO_PORTC_BASE         // Display Command
	#define LCD_RESET_PORT     				GPIO_PORTL_BASE         // Reset
	#define LCD_BACKLIGHT_PORT       		GPIO_PORTF_BASE         // Backlight
	#define LCD_USER_LED_PORT				GPIO_PORTN_BASE			// User LED

	// Pins from TM4C1294XL connected to LCD
	#define LCD_SDI_PIN                     GPIO_PIN_1				// SSI2 Module
	#define LCD_SDI_PIN_FUNCTION            (0x0003040F)			// Alternate Function: 
																	// SSI2XDAT0
	#define LCD_SCL_PIN                     GPIO_PIN_3				// SSI2 Module
	#define LCD_SCL_PIN_FUNCTION            (0x00030C0F)			// Alternate Function:
																	// SSI2Clk
	#define LCD_SCS_PIN                     GPIO_PIN_2				// Chip Select
	#define LCD_SDC_PIN                     GPIO_PIN_7				// Display Command
	#define LCD_RESET_PIN                   GPIO_PIN_2				// Reset
	#define LCD_BACKLIGHT_PIN               GPIO_PIN_1				// Backlight
	#define LCD_BACKLIGHT_PIN_FUNCTION      (0x00050406)			// Alternate Function:
																	// M0PWM1
	#define LCD_USER_LED_1_PIN				GPIO_PIN_1				// User LED 1
	#define LCD_USER_LED_2_PIN				GPIO_PIN_0				// User LED 2
#else
	//*****************************************************************************
	//
	// BOOSTERPACK 2
	//
	//*****************************************************************************
	// Peripherals from TM4C1294XL connected to LCD
	#define LCD_SSI_PERIPH					SYSCTL_PERIPH_SSI3		// SSI3 Module
	#define LCD_SDI_PERIPH     				SYSCTL_PERIPH_GPIOQ		// SSI3 Module
	#define LCD_SCL_PERIPH     				SYSCTL_PERIPH_GPIOQ		// SSI3 Module
	#define LCD_SCS_PERIPH     				SYSCTL_PERIPH_GPIOP		// Chip Select
	#define LCD_SDC_PERIPH     				SYSCTL_PERIPH_GPIOP		// Display Command
	#define LCD_RESET_PERIPH   				SYSCTL_PERIPH_GPIOK		// Reset
	#define LCD_BACKLIGHT_PERIPH     		SYSCTL_PERIPH_GPIOG		// Backlight
	#define LCD_USER_LED_PERIPH				SYSCTL_PERIPH_GPION		// User LED


	// Ports/Modules from TM4C1294XL connected to LCD
	#define LCD_SSI_MODULE					SSI3_BASE				// SSI3 Module
	#define LCD_SDI_PORT       				GPIO_PORTQ_BASE			// SSI3 Module
	#define LCD_SCL_PORT       				GPIO_PORTQ_BASE         // SSI3 Module
	#define LCD_SCS_PORT       				GPIO_PORTP_BASE         // Chip Select
	#define LCD_SDC_PORT       				GPIO_PORTP_BASE         // Display Command
	#define LCD_RESET_PORT     				GPIO_PORTK_BASE         // Reset
	#define LCD_BACKLIGHT_PORT       		GPIO_PORTG_BASE         // Backlight
	#define LCD_USER_LED_PORT				GPIO_PORTN_BASE			// User LED

	// Pins from TM4C1294XL connected to LCD
	#define LCD_SDI_PIN                     GPIO_PIN_2				// SSI3 Module
	#define LCD_SDI_PIN_FUNCTION            (0x000E080E)			// Alternate Function:
																	// SSI3XDAT0
	#define LCD_SCL_PIN                     GPIO_PIN_0				// SSI3 Module
	#define LCD_SCL_PIN_FUNCTION            (0x000E000E)			// Alternate Function:
																	// SSI3Clk
	#define LCD_SCS_PIN                     GPIO_PIN_3				// Chip Select
	#define LCD_SDC_PIN                     GPIO_PIN_4				// Display Command
	#define LCD_RESET_PIN                   GPIO_PIN_6				// Reset
	#define LCD_BACKLIGHT_PIN               GPIO_PIN_1				// Backlight
	#define LCD_BACKLIGHT_PIN_FUNCTION      (0x00060406)			// Alternate Function:
																	// M0PWM5
	#define LCD_USER_LED_1_PIN				GPIO_PIN_1				// User LED 1
	#define LCD_USER_LED_2_PIN				GPIO_PIN_0				// User LED 2
#endif

//*****************************************************************************
//
// Various internal SD2119 registers name labels
//
//*****************************************************************************
#define SSD2119_DEVICE_CODE_READ_REG  0x00
#define SSD2119_OSC_START_REG         0x00
#define SSD2119_OUTPUT_CTRL_REG       0x01
#define SSD2119_LCD_DRIVE_AC_CTRL_REG 0x02
#define SSD2119_PWR_CTRL_1_REG        0x03
#define SSD2119_DISPLAY_CTRL_REG      0x07
#define SSD2119_FRAME_CYCLE_CTRL_REG  0x0B
#define SSD2119_PWR_CTRL_2_REG        0x0C
#define SSD2119_PWR_CTRL_3_REG        0x0D
#define SSD2119_PWR_CTRL_4_REG        0x0E
#define SSD2119_GATE_SCAN_START_REG   0x0F
#define SSD2119_SLEEP_MODE_REG        0x10
#define SSD2119_ENTRY_MODE_REG        0x11
#define SSD2119_GEN_IF_CTRL_REG       0x15
#define SSD2119_PWR_CTRL_5_REG        0x1E
#define SSD2119_RAM_DATA_REG          0x22
#define SSD2119_FRAME_FREQ_REG        0x25
#define SSD2119_VCOM_OTP_1_REG        0x28
#define SSD2119_VCOM_OTP_2_REG        0x29
#define SSD2119_GAMMA_CTRL_1_REG      0x30
#define SSD2119_GAMMA_CTRL_2_REG      0x31
#define SSD2119_GAMMA_CTRL_3_REG      0x32
#define SSD2119_GAMMA_CTRL_4_REG      0x33
#define SSD2119_GAMMA_CTRL_5_REG      0x34
#define SSD2119_GAMMA_CTRL_6_REG      0x35
#define SSD2119_GAMMA_CTRL_7_REG      0x36
#define SSD2119_GAMMA_CTRL_8_REG      0x37
#define SSD2119_GAMMA_CTRL_9_REG      0x3A
#define SSD2119_GAMMA_CTRL_10_REG     0x3B
#define SSD2119_V_RAM_POS_REG         0x44
#define SSD2119_H_RAM_START_REG       0x45
#define SSD2119_H_RAM_END_REG         0x46
#define SSD2119_X_RAM_ADDR_REG        0x4E
#define SSD2119_Y_RAM_ADDR_REG        0x4F

#define ENTRY_MODE_DEFAULT 0x6830
#define MAKE_ENTRY_MODE(x) ((ENTRY_MODE_DEFAULT & 0xFF00) | (x))

//*****************************************************************************
//
// The dimensions of the LCD panel.
//
//*****************************************************************************
#define LCD_VERTICAL_MAX 240
#define LCD_HORIZONTAL_MAX 320

//*****************************************************************************
//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color.
//
// \return Returns the display-driver specific color.
//
//*****************************************************************************
#define DPYCOLORTRANSLATE(c)    ((((c) & 0x00f80000) >> 8) |               \
        (((c) & 0x0000fc00) >> 5) |               \
        (((c) & 0x000000f8) >> 3))

//*****************************************************************************
//
// Function pointer types for low level LCD controller access functions.
//
//*****************************************************************************
typedef void (*pfnWriteData)(uint16_t ui16Data);
typedef void (*pfnWriteCommand)(uint8_t ui8Data);

//*****************************************************************************
//
// Function pointers for low level LCD controller access functions.
//
//*****************************************************************************
static void initSSI(void);
static void WriteDataGPIO(uint16_t ui16Data);
static void WriteCommandGPIO(uint8_t ui8Data);

pfnWriteData WriteData = WriteDataGPIO;
pfnWriteCommand WriteCommand = WriteCommandGPIO;

void Kentec_LED_ON(void)
{
	GPIOPinWrite(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, 0);
	GPIOPinWrite(LCD_USER_LED_PORT, LCD_USER_LED_1_PIN, LCD_USER_LED_1_PIN);
}

void Kentec_LED_OFF(void)
{
	GPIOPinWrite(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, LCD_BACKLIGHT_PIN);
	GPIOPinWrite(LCD_USER_LED_PORT, LCD_USER_LED_1_PIN, 0);
}

//*****************************************************************************
//
// Initialize SSI3 module on Boosterpack 2
//
//*****************************************************************************
static
void initSSI(void)
{
	//
	// Enable Peripherals needed for SSI3
	// GPIO Port Q
	// SSI3
	SysCtlPeripheralEnable(LCD_SDI_PERIPH);
	SysCtlPeripheralEnable(LCD_SSI_PERIPH);

    //
    // Activate alternate pin function SSI3CLK, SSI3SDI
    //
    GPIOPinConfigure(LCD_SCL_PIN_FUNCTION);
    GPIOPinConfigure(LCD_SDI_PIN_FUNCTION);


    // Set Type of SSI communication 4 wire (MOSI | CLK) and pins
	// Additional "Chip select" and "Display Command" has to be set with 
	// on extra PINs >> DONE BEFORE
	GPIOPinTypeSSI(LCD_SDI_PORT, LCD_SDI_PIN | LCD_SCL_PIN);

	//
	// Sets the data clock source for the specified SSI peripheral
	//
	SSIClockSourceSet(LCD_SSI_MODULE, SSI_CLOCK_SYSTEM);

	//
	// Configure the SSI3 Communication
	//
	SSIConfigSetExpClk(	LCD_SSI_MODULE, 		// SSI Port (SSI3)
						g_ui32SysClock, 		// ÂµController SysClock
						SSI_FRF_MOTO_MODE_0, 	// Communication Mode
						SSI_MODE_MASTER, 		// ÂµController = Master
						16000000, 				// Bit rate (6 Mhz)
						8);						// Data packet width

	//
	// Enable the SSI module
	//
	SSIEnable(LCD_SSI_MODULE);
}

//*****************************************************************************
//
// Writes a data word to the SSD2119.  This function implements the basic GPIO
// interface to the LCD display.
//
//*****************************************************************************
static void
WriteDataGPIO(uint16_t ui16Data)
{
	uint8_t ui8Data;

    //
    // Calculate the high byte to transmit.
    //
    ui8Data = (uint8_t)(ui16Data >> 8);

    //
    // Wait for the transmit buffer to become empty.
    //
    while( (SSI_TXEOT & SSIIntStatus(LCD_SSI_MODULE, false)) != SSI_TXEOT);

    //
    // Transmit the high byte.
    //
    SSIDataPut(LCD_SSI_MODULE,ui8Data);

    //
    // Calculate the low byte to transmit.
    //
    ui8Data = (uint8_t)(ui16Data & 0xff);

    //
    // Wait for the transmit buffer to become empty.
    //
    while( (SSI_TXEOT & SSIIntStatus(LCD_SSI_MODULE, false)) != SSI_TXEOT);

    //
    // Transmit the high byte.
    //
    SSIDataPut(LCD_SSI_MODULE,ui8Data);
}

//*****************************************************************************
//
// Writes a command to the SSD2119.  This function implements the basic GPIO
// interface to the LCD display.
//
//*****************************************************************************
static void
WriteCommandGPIO(uint8_t ui8Data)
{
	//
    // Wait for any SPI transmission to complete before setting the LCD_SDC signal.
    //
    while(SSIBusy(LCD_SSI_MODULE));

    //
    // Set the LCD_SDC signal low, indicating that following writes are commands.
    //
    GPIOPinWrite(LCD_SDC_PORT, LCD_SDC_PIN, 0);

    //
    // Transmit the command.
    //
    SSIDataPut(LCD_SSI_MODULE, ui8Data);

    //
    // Wait for the SPI transmission to complete before setting the LCD_SDC signal.
    //
    while(SSIBusy(LCD_SSI_MODULE));

    //
    // Set the LCD_SDC signal high, indicating that following writes are data.
    //
    GPIOPinWrite(LCD_SDC_PORT,LCD_SDC_PIN, LCD_SDC_PIN);
}

//*****************************************************************************
//
// Initializes the pins required for the GPIO-based LCD interface.
//
// This function configures the GPIO pins used to control the LCD display
// when the SPI interface is in use.  On exit, the LCD controller
// has been reset and is ready to receive command and data writes.
//
// \return None.
//
//*****************************************************************************
static void
InitGPIOLCDInterface(uint32_t ui32ClockMS)
{
	//
    // Configure the pins that connect to the LCD as GPIO outputs.
    //
	
	//
    // Initialize LCD Reset Pin.
	SysCtlPeripheralEnable(LCD_RESET_PERIPH);
	GPIOPinTypeGPIOOutput(LCD_RESET_PORT, LCD_RESET_PIN);
	GPIOPinWrite(LCD_RESET_PORT, LCD_RESET_PIN,0);

	//
    // Initialize LCD Display Command Pin.
	SysCtlPeripheralEnable(LCD_SDC_PERIPH);
	GPIOPinTypeGPIOOutput(LCD_SDC_PORT, LCD_SDC_PIN);
	GPIOPinWrite(LCD_SDC_PORT, LCD_SDC_PIN,0);

	//
    // Initialize LCD Chip Select Pin.
    SysCtlPeripheralEnable(LCD_SCS_PERIPH);
	GPIOPinTypeGPIOOutput(LCD_SCS_PORT, LCD_SCS_PIN);
	GPIOPinWrite(LCD_SCS_PORT, LCD_SCS_PIN,0);
	
	//
    // Initialize User LED Pins.
    SysCtlPeripheralEnable(LCD_USER_LED_PERIPH);
	GPIOPinTypeGPIOOutput(LCD_USER_LED_PORT, LCD_USER_LED_1_PIN | LCD_USER_LED_2_PIN);
	GPIOPinWrite(LCD_USER_LED_PORT, LCD_USER_LED_1_PIN,0);
	GPIOPinWrite(LCD_USER_LED_PORT, LCD_USER_LED_2_PIN,0);
	
	//
    // Initialize SSI peripheral.
    //
    initSSI();
	
	//
    // Set the LCD Backlight high to enable
    //
    SysCtlPeripheralEnable(LCD_BACKLIGHT_PERIPH);
    GPIOPinTypeGPIOOutput(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
    GPIOPinWrite(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, LCD_BACKLIGHT_PIN);
	
	//
    // Set the LCD control pins to their default values.
    //
    GPIOPinWrite(LCD_SDC_PORT, LCD_SDC_PIN, LCD_SDC_PIN);
    GPIOPinWrite(LCD_SCS_PORT, LCD_SCS_PIN, 0);

    //
    // Delay for 1ms.
    //
    SysCtlDelay(ui32ClockMS);

    //
    // Deassert the LCD reset signal.
    //
    GPIOPinWrite(LCD_RESET_PORT, LCD_RESET_PIN, LCD_RESET_PIN);

    //
    // Delay for 1ms.
    //
    SysCtlDelay(ui32ClockMS);
}

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! \param ui32SysClockSpeed is the system clock speed of the MCU.
//!
//! This function initializes the SSD2119 display controller on the panel,
//! preparing it to display data.
//!
//! \return None.
//
//*****************************************************************************
void
Kentec320x240x16_SSD2119Init(uint32_t ui32SysClockSpeed)
{
    uint32_t ui32ClockMS, ui32Count;

    //
    // Get the current processor clock frequency.
    //
    ui32ClockMS = ui32SysClockSpeed / (3 * 1000);

    //
    // Perform low level interface initialization depending upon how the LCD
    // is connected to the Tiva C Series microcontroller.  This varies
    // depending upon the daughter board connected it is possible that a
    // daughter board can drive the LCD directly rather than via the basic GPIO
    // interface.
    //
    {
        //
        // Initialize the GPIOs used to interface to the LCD controller.
        //
        InitGPIOLCDInterface(ui32ClockMS);
    }

    //
    // Enter sleep mode (if we are not already there).
    //
    WriteCommand(SSD2119_SLEEP_MODE_REG);
    WriteData(0x0001);

    //
    // Set initial power parameters.
    //
    WriteCommand(SSD2119_PWR_CTRL_5_REG);
    WriteData(0x00BA);
    WriteCommand(SSD2119_VCOM_OTP_1_REG);
    WriteData(0x0006);

    //
    // Start the oscillator.
    //
    WriteCommand(SSD2119_OSC_START_REG);
    WriteData(0x0001);

    //
    // Set pixel format and basic display orientation (scanning direction).
    //
    WriteCommand(SSD2119_OUTPUT_CTRL_REG);
    WriteData(0x30EF);
    WriteCommand(SSD2119_LCD_DRIVE_AC_CTRL_REG);
    WriteData(0x0600);

    //
    // Exit sleep mode.
    //
    WriteCommand(SSD2119_SLEEP_MODE_REG);
    WriteData(0x0000);

    //
    // Delay 30mS
    //
    SysCtlDelay(30 * ui32ClockMS);

    //
    // Configure pixel color format and MCU interface parameters.
    //
    WriteCommand(SSD2119_ENTRY_MODE_REG);
    WriteData(ENTRY_MODE_DEFAULT);

    //
    // Enable the display.
    //
    WriteCommand(SSD2119_DISPLAY_CTRL_REG);
    WriteData(0x0033);

    //
    // Set VCIX2 voltage to 6.1V.
    //
    WriteCommand(SSD2119_PWR_CTRL_2_REG);
    WriteData(0x0005);

    //
    // Configure gamma correction.
    //
    WriteCommand(SSD2119_GAMMA_CTRL_1_REG);
    WriteData(0x0000);
    WriteCommand(SSD2119_GAMMA_CTRL_2_REG);
    WriteData(0x0400);
    WriteCommand(SSD2119_GAMMA_CTRL_3_REG);
    WriteData(0x0106);
    WriteCommand(SSD2119_GAMMA_CTRL_4_REG);
    WriteData(0x0700);
    WriteCommand(SSD2119_GAMMA_CTRL_5_REG);
    WriteData(0x0002);
    WriteCommand(SSD2119_GAMMA_CTRL_6_REG);
    WriteData(0x0702);
    WriteCommand(SSD2119_GAMMA_CTRL_7_REG);
    WriteData(0x0707);
    WriteCommand(SSD2119_GAMMA_CTRL_8_REG);
    WriteData(0x0203);
    WriteCommand(SSD2119_GAMMA_CTRL_9_REG);
    WriteData(0x1400);
    WriteCommand(SSD2119_GAMMA_CTRL_10_REG);
    WriteData(0x0F03);

    //
    // Configure Vlcd63 and VCOMl.
    //
    WriteCommand(SSD2119_PWR_CTRL_3_REG);
    WriteData(0x0007);
    WriteCommand(SSD2119_PWR_CTRL_4_REG);
    WriteData(0x3100);

    //
    // Set the display size and ensure that the GRAM window is set to allow
    // access to the full display buffer.
    //
    WriteCommand(SSD2119_V_RAM_POS_REG);
    WriteData((LCD_VERTICAL_MAX-1) << 8);
    WriteCommand(SSD2119_H_RAM_START_REG);
    WriteData(0x0000);
    WriteCommand(SSD2119_H_RAM_END_REG);
    WriteData(LCD_HORIZONTAL_MAX-1);
    WriteCommand(SSD2119_X_RAM_ADDR_REG);
    WriteData(0x00);
    WriteCommand(SSD2119_Y_RAM_ADDR_REG);
    WriteData(0x00);

    //
    // Clear the contents of the display buffer.
    //
    WriteCommand(SSD2119_RAM_DATA_REG);
    for(ui32Count = 0; ui32Count < (320 * 240); ui32Count++)
    {
        WriteData(0x0000);
    }
}

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the pixel.
//! \param i32Y is the Y coordinate of the pixel.
//! \param ui32Value is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Kentec320x240x16_SSD2119PixelDraw(void *pvDisplayData, int32_t i32X,
        int32_t i32Y,
        uint32_t ui32Value)
{
    //
    // Set the X address of the display cursor.
    //
    WriteCommand(SSD2119_X_RAM_ADDR_REG);
    WriteData(MAPPED_X(i32X, i32Y));

    //
    // Set the Y address of the display cursor.
    //
    WriteCommand(SSD2119_Y_RAM_ADDR_REG);
    WriteData(MAPPED_Y(i32X, i32Y));

    //
    // Write the pixel value.
    //
    WriteCommand(SSD2119_RAM_DATA_REG);
    WriteData(ui32Value);
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the first pixel.
//! \param i32Y is the Y coordinate of the first pixel.
//! \param i32X0 is sub-pixel offset within the pixel data, which is valid for
//! 1 or 4 bit per pixel formats.
//! \param i32Count is the number of pixels to draw.
//! \param i32BPP is the number of bits per pixel; must be 1, 4, or 8,
//! optionally OR'ed with flags that a driver may use to aid performance.
//! \param pui8Data is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pui8Palette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Kentec320x240x16_SSD2119PixelDrawMultiple(void *pvDisplayData, int32_t i32X,
        int32_t i32Y, int32_t i32X0,
        int32_t i32Count,
        int32_t i32BPP,
        const uint8_t *pui8Data,
        const uint8_t *pui8Palette)
{
    uint32_t ui32Byte;

    //
    // Set the cursor increment to left to right, followed by top to bottom.
    //
    WriteCommand(SSD2119_ENTRY_MODE_REG);
    WriteData(MAKE_ENTRY_MODE(HORIZ_DIRECTION));

    //
    // Set the starting X address of the display cursor.
    //
    WriteCommand(SSD2119_X_RAM_ADDR_REG);
    WriteData(MAPPED_X(i32X, i32Y));

    //
    // Set the Y address of the display cursor.
    //
    WriteCommand(SSD2119_Y_RAM_ADDR_REG);
    WriteData(MAPPED_Y(i32X, i32Y));

    //
    // Write the data RAM write command.
    //
    WriteCommand(SSD2119_RAM_DATA_REG);

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
    switch(i32BPP & 0xFF)
    {
        //
        // The pixel data is in 1 bit per pixel format.
        //
        case 1:
            {
                //
                // Loop while there are more pixels to draw.
                //
                while(i32Count)
                {
                    //
                    // Get the next byte of image data.
                    //
                    ui32Byte = *pui8Data++;

                    //
                    // Loop through the pixels in this byte of image data.
                    //
                    for(; (i32X0 < 8) && i32Count; i32X0++, i32Count--)
                    {
                        //
                        // Draw this pixel in the appropriate color.
                        //
                        WriteData(((uint32_t *)pui8Palette)[(ui32Byte >>
                                    (7 - i32X0)) & 1]);
                    }

                    //
                    // Start at the beginning of the next byte of image data.
                    //
                    i32X0 = 0;
                }

                //
                // The image data has been drawn.
                //
                break;
            }

            //
            // The pixel data is in 4 bit per pixel format.
            //
        case 4:
            {
                //
                // Loop while there are more pixels to draw.  "Duff's device"
                // is used to jump into the middle of the loop if the first
                // nibble of the pixel data should not be used.  Duff's device
                // makes use of the fact that a case statement is legal
                // anywhere within a sub-block of a switch statement.  See
                // http://en.wikipedia.org/wiki/Duff's_device for detailed
                // information about Duff's device.
                //
                switch(i32X0 & 1)
                {
                    case 0:
                        while(i32Count)
                        {
                            //
                            // Get the upper nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette.
                            //
                            ui32Byte = (*pui8Data >> 4) * 3;
                            ui32Byte = (*(uint32_t *)(pui8Palette + ui32Byte) &
                                    0x00ffffff);

                            //
                            // Translate this palette entry and write it to the
                            // screen.
                            //
                            WriteData(DPYCOLORTRANSLATE(ui32Byte));

                            //
                            // Decrement the count of pixels to draw.
                            //
                            i32Count--;

                            //
                            // See if there is another pixel to draw.
                            //
                            if(i32Count)
                            {
                                case 1:
                                    //
                                    // Get the lower nibble of the next byte of
                                    // pixel data and extract the corresponding
                                    // entry from the palette.
                                    //
                                    ui32Byte = (*pui8Data++ & 15) * 3;
                                    ui32Byte = (*(uint32_t *)(pui8Palette +
                                            ui32Byte) & 0x00ffffff);

                                    //
                                    // Translate this palette entry and write
                                    // it to the screen.
                                    //
                                    WriteData(DPYCOLORTRANSLATE(ui32Byte));

                                    //
                                    // Decrement the count of pixels to draw.
                                    //
                                    i32Count--;
                            }
                        }
                }

                //
                // The image data has been drawn.
                //
                break;
            }

            //
            // The pixel data is in 8 bit per pixel format.
            //
        case 8:
            {
                //
                // Loop while there are more pixels to draw.
                //
                while(i32Count--)
                {
                    //
                    // Get the next byte of pixel data and extract the
                    // corresponding entry from the palette.
                    //
                    ui32Byte = *pui8Data++ * 3;
                    ui32Byte = *(uint32_t *)(pui8Palette + ui32Byte) &
                        0x00ffffff;

                    //
                    // Translate this palette entry and write it to the screen.
                    //
                    WriteData(DPYCOLORTRANSLATE(ui32Byte));
                }

                //
                // The image data has been drawn.
                //
                break;
            }

            //
            // We are being passed data in the display's native format.  Merely
            // write it directly to the display.  This is a special case which
            // is not used by the graphics library but which is helpful to
            // applications which may want to handle, for example, JPEG images.
            //
        case 16:
            {
                uint16_t ui16Byte;

                //
                // Loop while there are more pixels to draw.
                //
                while(i32Count--)
                {
                    //
                    // Get the next byte of pixel data and extract the
                    // corresponding entry from the palette.
                    //
                    ui16Byte = *((uint16_t *)pui8Data);
                    pui8Data += 2;

                    //
                    // Translate this palette entry and write it to the screen.
                    //
                    WriteData(ui16Byte);
                }
            }
    }
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X1 is the X coordinate of the start of the line.
//! \param i32X2 is the X coordinate of the end of the line.
//! \param i32Y is the Y coordinate of the line.
//! \param ui32Value is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Kentec320x240x16_SSD2119LineDrawH(void *pvDisplayData, int32_t i32X1,
        int32_t i32X2, int32_t i32Y,
        uint32_t ui32Value)
{
    //
    // Set the cursor increment to left to right, followed by top to bottom.
    //
    WriteCommand(SSD2119_ENTRY_MODE_REG);
    WriteData(MAKE_ENTRY_MODE(HORIZ_DIRECTION));

    //
    // Set the starting X address of the display cursor.
    //
    WriteCommand(SSD2119_X_RAM_ADDR_REG);
    WriteData(MAPPED_X(i32X1, i32Y));

    //
    // Set the Y address of the display cursor.
    //
    WriteCommand(SSD2119_Y_RAM_ADDR_REG);
    WriteData(MAPPED_Y(i32X1, i32Y));

    //
    // Write the data RAM write command.
    //
    WriteCommand(SSD2119_RAM_DATA_REG);

    //
    // Loop through the pixels of this horizontal line.
    //
    while(i32X1++ <= i32X2)
    {
        //
        // Write the pixel value.
        //
        WriteData(ui32Value);
    }
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the line.
//! \param i32Y1 is the Y coordinate of the start of the line.
//! \param i32Y2 is the Y coordinate of the end of the line.
//! \param ui32Value is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Kentec320x240x16_SSD2119LineDrawV(void *pvDisplayData, int32_t i32X,
        int32_t i32Y1,
        int32_t i32Y2, uint32_t ui32Value)
{
    //
    // Set the cursor increment to top to bottom, followed by left to right.
    //
    WriteCommand(SSD2119_ENTRY_MODE_REG);
    WriteData(MAKE_ENTRY_MODE(VERT_DIRECTION));

    //
    // Set the X address of the display cursor.
    //
    WriteCommand(SSD2119_X_RAM_ADDR_REG);
    WriteData(MAPPED_X(i32X, i32Y1));

    //
    // Set the starting Y address of the display cursor.
    //
    WriteCommand(SSD2119_Y_RAM_ADDR_REG);
    WriteData(MAPPED_Y(i32X, i32Y1));

    //
    // Write the data RAM write command.
    //
    WriteCommand(SSD2119_RAM_DATA_REG);

    //
    // Loop through the pixels of this vertical line.
    //
    while(i32Y1++ <= i32Y2)
    {
        //
        // Write the pixel value.
        //
        WriteData(ui32Value);
    }
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ui32Value is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both i16XMin
//! and i16XMax are drawn, along with i16YMin and i16YMax).
//!
//! \return None.
//
//*****************************************************************************
static void
Kentec320x240x16_SSD2119RectFill(void *pvDisplayData, const tRectangle *pRect,
        uint32_t ui32Value)
{
    int32_t i32Count;

    //
    // Write the Y extents of the rectangle.
    //
    WriteCommand(SSD2119_ENTRY_MODE_REG);
    WriteData(MAKE_ENTRY_MODE(HORIZ_DIRECTION));

    //
    // Write the X extents of the rectangle.
    //
    WriteCommand(SSD2119_H_RAM_START_REG);
#if (defined PORTRAIT) || (defined LANDSCAPE)
    WriteData(MAPPED_X(pRect->i16XMax, pRect->i16YMax));
#else
    WriteData(MAPPED_X(pRect->i16XMin, pRect->sMin));
#endif

    WriteCommand(SSD2119_H_RAM_END_REG);
#if (defined PORTRAIT) || (defined LANDSCAPE)
    WriteData(MAPPED_X(pRect->i16XMin, pRect->i16YMin));
#else
    WriteData(MAPPED_X(pRect->i16XMax, pRect->i16YMax));
#endif

    //
    // Write the Y extents of the rectangle
    //
    WriteCommand(SSD2119_V_RAM_POS_REG);
#if (defined LANDSCAPE_FLIP) || (defined PORTRAIT)
    WriteData(MAPPED_Y(pRect->i16XMin, pRect->i16YMin) |
            (MAPPED_Y(pRect->i16XMax, pRect->i16YMax) << 8));
#else
    WriteData(MAPPED_Y(pRect->i16XMax, pRect->i16YMax) |
            (MAPPED_Y(pRect->i16XMin, pRect->i16YMin) << 8));
#endif

    //
    // Set the display cursor to the upper left of the rectangle (in
    // application coordinate space).
    //
    WriteCommand(SSD2119_X_RAM_ADDR_REG);
    WriteData(MAPPED_X(pRect->i16XMin, pRect->i16YMin));

    WriteCommand(SSD2119_Y_RAM_ADDR_REG);
    WriteData(MAPPED_Y(pRect->i16XMin, pRect->i16YMin));

    //
    // Tell the controller we are about to write data into its RAM.
    //
    WriteCommand(SSD2119_RAM_DATA_REG);

    //
    // Loop through the pixels of this filled rectangle.
    //
    for(i32Count = ((pRect->i16XMax - pRect->i16XMin + 1) *
                (pRect->i16YMax - pRect->i16YMin + 1));
            i32Count >= 0; i32Count--)
    {
        //
        // Write the pixel value.
        //
        WriteData(ui32Value);
    }

    //
    // Reset the X extents to the entire screen.
    //
    WriteCommand(SSD2119_H_RAM_START_REG);
    WriteData(0x0000);
    WriteCommand(SSD2119_H_RAM_END_REG);
    WriteData(0x013F);

    //
    // Reset the Y extent to the full screen
    //
    WriteCommand(SSD2119_V_RAM_POS_REG);
    WriteData(0xEF00);
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ui32Value is the 24-bit RGB color.  The least-significant byte is
//! the blue channel, the next byte is the green channel, and the third byte is
//! the red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static uint32_t
Kentec320x240x16_SSD2119ColorTranslate(void *pvDisplayData,
        uint32_t ui32Value)
{
    //
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    //
    return(DPYCOLORTRANSLATE(ui32Value));
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.  For the SSD2119
//! driver, the flush is a no operation.
//!
//! \return None.
//
//*****************************************************************************
static void
Kentec320x240x16_SSD2119Flush(void *pvDisplayData)
{
    //
    // There is nothing to be done.
    //
}

//*****************************************************************************
//
//! The display structure that describes the driver for the Kentec
//! K350QVG-V2-F TFT panel with an SSD2119 controller.
//
//*****************************************************************************
const tDisplay g_sKentec320x240x16_SSD2119 =
{
    sizeof(tDisplay),
    0,
#if defined(PORTRAIT) || defined(PORTRAIT_FLIP)
    240,
    320,
#else
    320,
    240,
#endif
    Kentec320x240x16_SSD2119PixelDraw,
    Kentec320x240x16_SSD2119PixelDrawMultiple,
    Kentec320x240x16_SSD2119LineDrawH,
    Kentec320x240x16_SSD2119LineDrawV,
    Kentec320x240x16_SSD2119RectFill,
    Kentec320x240x16_SSD2119ColorTranslate,
    Kentec320x240x16_SSD2119Flush
};
