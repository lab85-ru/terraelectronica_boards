//*****************************************************************************
//
// NOKIA128X160.c - Display driver for the NOKIA LCD display.
//                  This version uses an SSI interface to the display controller.
//
// Copyright (c) 2011-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 1.1 of the DK-TM4C123G Firmware Package.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"
#include "drivers/NOKIA128X160.h"

//*****************************************************************************
//
// Defines the SSI and GPIO peripherals that are used for this display.
//
//*****************************************************************************
#define DISPLAY_SSI_PERIPH          SYSCTL_PERIPH_SSI2
#define DISPLAY_SSI_GPIO_PERIPH     SYSCTL_PERIPH_GPIOH
#define DISPLAY_RST_GPIO_PERIPH     SYSCTL_PERIPH_GPIOG

//*****************************************************************************
//
// Defines the GPIO pin configuration macros for the pins that are used for
// the SSI function.
//
//*****************************************************************************
#define DISPLAY_PINCFG_SSICLK       GPIO_PH4_SSI2CLK
#define DISPLAY_PINCFG_SSIFSS       GPIO_PH5_SSI2FSS
#define DISPLAY_PINCFG_SSITX        GPIO_PH7_SSI2TX

//*****************************************************************************
//
// Defines the port and pins for the SSI peripheral.
//
//*****************************************************************************
#define DISPLAY_SSI_PORT            GPIO_PORTH_BASE
#define DISPLAY_SSI_PINS            (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7)

//*****************************************************************************
//
// Defines the port and pins for the display voltage enable signal.
//
//*****************************************************************************
#define DISPLAY_ENV_PORT            GPIO_PORTG_BASE
#define DISPLAY_ENV_PIN             GPIO_PIN_0

//*****************************************************************************
//
// Defines the port and pins for the display reset signal.
//
//*****************************************************************************
#define DISPLAY_RST_PORT            GPIO_PORTG_BASE
#define DISPLAY_RST_PIN             GPIO_PIN_1

//*****************************************************************************
//
// Defines the port and pins for the display Data/Command (D/C) signal.
//
//*****************************************************************************
#define DISPLAY_D_C_PORT            GPIO_PORTH_BASE
#define DISPLAY_D_C_PIN             GPIO_PIN_6

//*****************************************************************************
//
// Defines the SSI peripheral used and the data speed.
//
//*****************************************************************************
#define DISPLAY_SSI_BASE            SSI2_BASE // SSI2
#define DISPLAY_SSI_CLOCK           20000000


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
// 24-bit format: XXXX XXXX RRRR RRRR GGGG GGGG BBBB BBBB
// 16-bit format: ---- ---- ---- ---- RRRR RGGG GGGB BBBB
//  8-bit format: ---- ---- ---- ---- ---- ---- RRRG GGBB
//
//
//*****************************************************************************
#define DPYCOLORTRANSLATE16(c)  ((((c) & 0x00f80000) >> 8) |                  \
                                 (((c) & 0x0000fc00) >> 5) |                  \
                                 (((c) & 0x000000f8) >> 3))
#define DPYCOLORTRANSLATE8(c)   ((((c) & 0x00e00000) >> 16) |                 \
                                 (((c) & 0x0000e000) >> 11) |                 \
                                 (((c) & 0x000000c0) >> 6))
#define DPYCOLORTRANSLATE DPYCOLORTRANSLATE16

//*****************************************************************************
//
//! Write a set of command bytes to the display controller.
//
//! \param pi8Cmd is a pointer to a set of command bytes.
//! \param ui32Count is the count of command bytes.
//!
//! This function provides a way to send multiple command bytes to the display
//! controller.  It can be used for single commands, or multiple commands
//! chained together in a buffer.  It will wait for any previous operation to
//! finish, and then copy all the command bytes to the controller.  It will
//! not return until the last command byte has been written to the SSI FIFO,
//! but data could still be shifting out to the display controller when this
//! function returns.
//!
//! \return None.
//
//*****************************************************************************
static void
NOKIA128X160WriteCommand(uint32_t Cmd)
{
     while(ROM_SSIBusy(DISPLAY_SSI_BASE))
    {
    }

    // Set the D/C bit(9) low to indicate command
    // Send all the command bytes to the display
    //
        ROM_SSIDataPut(DISPLAY_SSI_BASE, (Cmd &= ~(0x00000100)));
}

//*****************************************************************************
//
//! Write a set of data bytes to the display controller.
//
//! \param pi8Data is a pointer to a set of data bytes, containing pixel data.
//! \param ui32Count is the count of command bytes.
//!
//! This function provides a way to send a set of pixel data to the display.
//! The data will draw pixels according to whatever the most recent col, row
//! settings are for the display.  It will wait for any previous operation to
//! finish, and then copy all the data bytes to the controller.  It will
//! not return until the last data byte has been written to the SSI FIFO,
//! but data could still be shifting out to the display controller when this
//! function returns.
//!
//! \return None.
//
//*****************************************************************************
static void
NOKIA128X160WriteData(uint32_t Data)
{
	    while(ROM_SSIBusy(DISPLAY_SSI_BASE))
    {
    }

    // Set the D/C bit(9) high to indicate command
    // Send all the command bytes to the display
    //
        ROM_SSIDataPut(DISPLAY_SSI_BASE, (Data |= 0x00000100));
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
NOKIA128X160PixelDraw(void *pvDisplayData, int32_t lX, int32_t lY,
                      uint32_t ulValue)
{
    unsigned char ucColor[2];
		ucColor[0] = ((ulValue & 0x0000FF00) >> 8);
		ucColor[1] = ulValue;

    NOKIA128X160WriteCommand(0x2A);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lX+2);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lX+2);
    NOKIA128X160WriteCommand(0x2B);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lY+1);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lY+1);
    NOKIA128X160WriteCommand(0x2C);
    NOKIA128X160WriteData(ucColor[0]);
    NOKIA128X160WriteData(ucColor[1]);
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the first pixel.
//! \param i32Y is the Y coordinate of the first pixel.
//! \param i32X0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param i32Count is the number of pixels to draw.
//! \param i32BPP is the number of bits per pixel; must be 1, 4, or 8 optionally
//! ORed with various flags unused by this driver.
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
NOKIA128X160PixelDrawMultiple(void *pvDisplayData, int32_t lX, int32_t lY, int32_t lX0,
                              int32_t lCount, int32_t lBPP,
                              const uint8_t *pucData,
                              const uint8_t *pucPalette)
{
    unsigned long ulByte;

    NOKIA128X160WriteCommand(0x2A);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lX+2);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(127+2);
    NOKIA128X160WriteCommand(0x2B);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lY+1);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(159+1);
    NOKIA128X160WriteCommand(0x2C);

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
    switch(lBPP)
    {
        //
        // The pixel data is in 1 bit per pixel format.
        //
        case 1:
        {
            //
            // Loop while there are more pixels to draw.
            //
            while(lCount)
            {
                //
                // Get the next byte of image data.
                //
                ulByte = *pucData++;

                //
                // Loop through the pixels in this byte of image data.
                //
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    //
                    // Draw this pixel in the appropriate color.
                    //
                    unsigned char ucBPP = ((unsigned long *)pucPalette)
                                            [(ulByte >> (7 - lX0)) & 1];
                    NOKIA128X160WriteData(ucBPP);
                    NOKIA128X160WriteData(ucBPP);
                }

                //
                // Start at the beginning of the next byte of image data.
                //
                lX0 = 0;
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
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            //
            switch(lX0 & 1)
            {
                case 0:
                    while(lCount)
                    {
                        unsigned int ucColor;

                        //
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the
                        // palette.
                        //
                        ulByte = (*pucData >> 4) * 3;
                        ulByte = (*(unsigned long *)(pucPalette + ulByte) &
                                  0x00ffffff);

                        //
                        // Translate this palette entry and write it to the
                        // screen.
                        //
                        ucColor = DPYCOLORTRANSLATE(ulByte);
                        NOKIA128X160WriteData((ucColor & 0x0000FF00) >> 8);
                        NOKIA128X160WriteData(ucColor);

                        //
                        // Decrement the count of pixels to draw.
                        //
                        lCount--;

                        //
                        // See if there is another pixel to draw.
                        //
                        if(lCount)
                        {
                case 1:
                            //
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette.
                            //
                            ulByte = (*pucData++ & 15) * 3;
                            ulByte = (*(unsigned long *)(pucPalette + ulByte) &
                                      0x00ffffff);

                            //
                            // Translate this palette entry and write it to the
                            // screen.
                            //
                            ucColor = DPYCOLORTRANSLATE(ulByte);
                            NOKIA128X160WriteData((ucColor & 0x0000FF00) >> 8);
                            NOKIA128X160WriteData(ucColor);

                            //
                            // Decrement the count of pixels to draw.
                            //
                            lCount--;
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
            while(lCount--)
            {
                unsigned char ucColor;

                //
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette.
                //
                ulByte = *pucData++ * 3;
                ulByte = *(unsigned long *)(pucPalette + ulByte) & 0x00ffffff;

                //
                // Translate this palette entry and write it to the screen.
                //
                ucColor = DPYCOLORTRANSLATE(ulByte);
                NOKIA128X160WriteData((ucColor & 0x0000FF00) >> 8);
                NOKIA128X160WriteData(ucColor);
            }

            //
            // The image data has been drawn.
            //
            break;
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
NOKIA128X160LineDrawH(void *pvDisplayData, int32_t lX1, int32_t lX2, int32_t lY,
                      uint32_t ulValue)
{
    unsigned int uIdx;

    //
    // Send command for starting row and column
    //
    NOKIA128X160WriteCommand(0x2A);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lX1+2);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lX2+2);
    NOKIA128X160WriteCommand(0x2B);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lY+1);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lY+1);
    NOKIA128X160WriteCommand(0x2C);

    //
    // Use buffer of pixels to draw line, so multiple bytes can be sent at
    // one time.  Fill the buffer with the line color.
    //

    uIdx = (lX2 - lX1);
    uIdx += 1;
    while(uIdx)
    {

    NOKIA128X160WriteData((ulValue & 0x0000FF00) >> 8);
    NOKIA128X160WriteData(ulValue);
    uIdx -= 1;
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
NOKIA128X160LineDrawV(void *pvDisplayData, int32_t lX, int32_t lY1, int32_t lY2,
                      uint32_t ulValue)
{
    unsigned int uIdx;

    //
    // Send command for starting row and column
    //
    NOKIA128X160WriteCommand(0x2A);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lX+2);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lX+2);
    NOKIA128X160WriteCommand(0x2B);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lY1+1);
    NOKIA128X160WriteData(0);
    NOKIA128X160WriteData(lY2+1);
    NOKIA128X160WriteCommand(0x2C);

    //
    // Use buffer of pixels to draw line, so multiple bytes can be sent at
    // one time.  Fill the buffer with the line color.
    //

    uIdx = (lY2 - lY1);
    uIdx += 1;
    while(uIdx)
    {

    NOKIA128X160WriteData((ulValue & 0x0000FF00) >> 8);
    NOKIA128X160WriteData(ulValue);
    uIdx -= 1;
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
//! rectangle specification is fully inclusive (in other words, both i16XMin and
//! i16XMax are drawn, aint32_t with i16YMin and i16YMax).
//!
//! \return None.
//
//*****************************************************************************
static void
NOKIA128X160RectFill(void *pvDisplayData, const tRectangle *pRect,
                     uint32_t ui32Value)
{
    unsigned int uY;

    for(uY = pRect->i16YMin; uY <= pRect->i16YMax; uY++)
    {
        NOKIA128X160LineDrawH(0, pRect->i16XMin, pRect->i16XMax, uY, ui32Value);
    }
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ui32Value is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static uint32_t
NOKIA128X160ColorTranslate(void *pvDisplayData, uint32_t ui32Value)
{
    //
    // Translate from a 24-bit RGB color to a 3-3-2 RGB color.
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
//! flush would copy the local frame buffer to the display.  Since no memory
//! based frame buffer is used for this driver, the flush is a no operation.
//!
//! \return None.
//
//*****************************************************************************
static void
NOKIA128X160Flush(void *pvDisplayData)
{
    //
    // There is nothing to be done.
    //
}

//*****************************************************************************
//
//! The display structure that describes the NOKIA LCD driver
//
//*****************************************************************************
const tDisplay g_sNOKIA128X160 =
{
    sizeof(tDisplay),
    0,
    128,
    160,
    NOKIA128X160PixelDraw,
    NOKIA128X160PixelDrawMultiple,
    NOKIA128X160LineDrawH,
    NOKIA128X160LineDrawV,
    NOKIA128X160RectFill,
    NOKIA128X160ColorTranslate,
    NOKIA128X160Flush
};

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! \return None.
//
//*****************************************************************************
void
NOKIA128X160Init(void)
{
    tRectangle sRect;

    //
    // Enable the peripherals used by this driver
    //
    ROM_SysCtlPeripheralEnable(DISPLAY_SSI_PERIPH);
    ROM_SysCtlPeripheralEnable(DISPLAY_SSI_GPIO_PERIPH);
    ROM_SysCtlPeripheralEnable(DISPLAY_RST_GPIO_PERIPH);

    //
    // Select the SSI function for the appropriate pins
    //
    ROM_GPIOPinConfigure(DISPLAY_PINCFG_SSICLK);
    ROM_GPIOPinConfigure(DISPLAY_PINCFG_SSIFSS);
    ROM_GPIOPinConfigure(DISPLAY_PINCFG_SSITX);


    //
    // Configure the pins for the SSI function
    //
    ROM_GPIOPinTypeSSI(DISPLAY_SSI_PORT, DISPLAY_SSI_PINS);

    //
    // Configure display control pins as GPIO output
    //
    ROM_GPIOPinTypeGPIOOutput(DISPLAY_RST_PORT, DISPLAY_RST_PIN);
    ROM_GPIOPinTypeGPIOOutput(DISPLAY_ENV_PORT, DISPLAY_ENV_PIN);

    //
    // Drive the reset pin low while we do other stuff
    //
    ROM_GPIOPinWrite(DISPLAY_RST_PORT, DISPLAY_RST_PIN, 0);

    //
    // Configure the SSI port
    //
    ROM_SSIDisable(DISPLAY_SSI_BASE);
    ROM_SSIConfigSetExpClk(DISPLAY_SSI_BASE, ROM_SysCtlClockGet(),
                           SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER,
                           DISPLAY_SSI_CLOCK, 9);
    ROM_SSIEnable(DISPLAY_SSI_BASE);

    //
    // Take the display out of reset
    //
    ROM_SysCtlDelay(1000);
    ROM_GPIOPinWrite(DISPLAY_RST_PORT, DISPLAY_RST_PIN, DISPLAY_RST_PIN);
    ROM_SysCtlDelay(4000);

    //
    // Enable display power supply backlight
    //
    ROM_GPIOPinWrite(DISPLAY_ENV_PORT, DISPLAY_ENV_PIN, DISPLAY_ENV_PIN);
    ROM_SysCtlDelay(1000);

    //
    // Send the initial configuration command bytes to the display
    //
    NOKIA128X160WriteCommand(0x11);
    NOKIA128X160WriteCommand(0x3A);
    NOKIA128X160WriteData(0x05);
    NOKIA128X160WriteCommand(0x36);
    NOKIA128X160WriteData(0xC0);
    NOKIA128X160WriteCommand(0x29);
    NOKIA128X160WriteCommand(0x20);
    NOKIA128X160WriteCommand(0x13);
    ROM_SysCtlDelay(1000);

    //
    // Fill the entire display with a black rectangle, to clear it.
    //
    sRect.i16XMin = 0;
    sRect.i16XMax = 127;
    sRect.i16YMin = 0;
    sRect.i16YMax = 159;
    NOKIA128X160RectFill(0, &sRect, 0);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
