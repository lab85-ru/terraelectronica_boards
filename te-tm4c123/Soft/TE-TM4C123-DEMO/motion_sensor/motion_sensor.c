//*****************************************************************************
//
// motion_sensor.c - Run motion sensor and displays it on the screen.
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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/systick.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"
#include "drivers/NOKIA128X160.h"
#include "drivers/LIS302DL.h"
#include "utils/ustdlib.h"

//*****************************************************************************
//
// Define an off-screen buffer and display structure.  This is used by the
// strip chart widget for drawing a scrolling display.
//
//*****************************************************************************
#define OFFSCREEN_BUF_SIZE      GrOffScreen4BPPSize(128, 160)
uint8_t g_pui8OffscreenBuf[OFFSCREEN_BUF_SIZE];
tDisplay g_sOffscreenDisplay;


//*****************************************************************************
//
// Compute and display a sine wave.
//
//*****************************************************************************
int
main(void)
{
    tRectangle sRect;
    tContext sContext;
    uint8_t DataX, DataY, DataZ;
    char pcBuffer[12];

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
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // Initialize the display driver.
    //
    NOKIA128X160Init();

    //
    // Initialize the graphics context.
    //
    GrContextInit(&sContext, &g_sNOKIA128X160);

    DpyLineDrawH(&g_sNOKIA128X160,0,127,0,ClrWhite);
    DpyLineDrawV(&g_sNOKIA128X160,0,0,159,ClrWhite);
    DpyLineDrawH(&g_sNOKIA128X160,0,127,159,ClrWhite);
    DpyLineDrawV(&g_sNOKIA128X160,127,0,159,ClrWhite);
    //
    // Fill the top part of the screen with red to create the banner.
    //
    sRect.i16XMin = 1;
    sRect.i16YMin = 1;
    sRect.i16XMax = 126;
    sRect.i16YMax = 20;
    GrContextForegroundSet(&sContext, ClrRed);
    GrRectFill(&sContext, &sRect);

    //
    // Change foreground for white text.
    //
    GrContextForegroundSet(&sContext, ClrWhite);

    //
    // Put the application name in the middle of the banner.
    //
    GrContextFontSet(&sContext, g_psFontCmss12);
    GrStringDrawCentered(&sContext, "LIS302DL sample", -1, GrContextDpyWidthGet(&sContext) / 2, 10, 0);
    GrContextForegroundSet(&sContext, ClrWhite);

		
    // Initialize the motion driver.
    //
    LIS302DL_Init();
    LIS302DL_Write_Reg(CTRL_REG1, CTRL_REG1_Xen|CTRL_REG1_Yen|CTRL_REG1_Zen|CTRL_REG1_PD);
 
    //
    // 
    //
    while(1)
    {
    if (LIS302DL_Read_Reg (STATUS_REG) == 0xFF)
	{
	DataX=LIS302DL_Read_Reg (OUT_X);
	DataY=LIS302DL_Read_Reg (OUT_Y);
	DataZ=LIS302DL_Read_Reg (OUT_Z);
	}

    sRect.i16XMin = 20;
    sRect.i16XMax = 107;
    sRect.i16YMin = 50;
    sRect.i16YMax = 100;
    GrContextForegroundSet(&sContext, ClrBlack);
    GrRectFill(&sContext, &sRect);

    GrContextFontSet(&sContext, g_psFontFixed6x8);

    GrContextForegroundSet(&sContext, ClrRed);

    usprintf(pcBuffer, "X=0x%02x", DataX);
    GrStringDrawCentered(&sContext, pcBuffer, -1, GrContextDpyWidthGet(&sContext) / 2, 60, 1);
    usprintf(pcBuffer, "Y=0x%02x", DataY);
    GrStringDrawCentered(&sContext, pcBuffer, -1, GrContextDpyWidthGet(&sContext) / 2, 75, 1);
    usprintf(pcBuffer, "Z=0x%02x", DataZ);
    GrStringDrawCentered(&sContext, pcBuffer, -1, GrContextDpyWidthGet(&sContext) / 2, 90, 1);
    SysCtlDelay(SysCtlClockGet() / 10);
    }
}
