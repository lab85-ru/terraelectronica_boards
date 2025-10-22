//*****************************************************************************
//
// adc_pe7.c - Example demonstrating how to configure the ADC for
//                  single ended operation.
//
// Copyright (c) 2010-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
// 
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the  
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// This is part of revision 1.1 of the Tiva Firmware Development Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "grlib/grlib.h"
#include "drivers/NOKIA128X160.h"
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
//! \addtogroup adc_examples_list
//! <h1>Single Ended ADC (single_ended)</h1>
//!
//! This example shows how to setup ADC0 as a single ended input and take a
//! single sample on AIN0/PE7.
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - ADC0 peripheral
//! - GPIO Port E peripheral (for AIN0 pin)
//! - AIN0 - PE7
//!
//! The following UART signals are configured only for displaying console
//! messages for this example.  These are not required for operation of the
//! ADC.
//! - UART0 peripheral
//! - GPIO Port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - None.
//
//*****************************************************************************

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void
InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// Configure ADC0 for a single-ended input and a single sample.  Once the
// sample is ready, an interrupt flag will be set.  Using a polling method,
// the data will be read then displayed on the console via UART0.
//
//*****************************************************************************
int
main(void)
{
    tRectangle sRect;
    tContext sContext;
    char pcBuffer[12];


 		//
    // This array is used for storing the data read from the ADC FIFO. It
    // must be as large as the FIFO for the sequencer in use.  This example
    // uses sequence 2 and step 2 which has a FIFO depth of 2.  If another sequence
    // was used with a deeper FIFO, then the array size must be changed.
    //
    uint32_t pui32ADC0Value[2];
    //
    // Set the clocking to run at 80 MHz (200 MHz / 2.5) using the PLL.  When
    // using the ADC, you must either use the PLL or supply a 16 MHz clock
    // source.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

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
    GrStringDrawCentered(&sContext, "ADC sample", -1, GrContextDpyWidthGet(&sContext) / 2, 10, 0);
    GrContextForegroundSet(&sContext, ClrWhite);
    //
    // Set up the serial console to use for displaying messages.  This is
    // just for this example program and is not needed for ADC operation.
    //
    InitConsole();

    //
    // Display the setup on the console.
    //
    UARTprintf("ADC ->\n");
    UARTprintf("  Type: Dual Ended\n");
    UARTprintf("  Samples: dual\n");
    UARTprintf("  Update Rate: 1s\n");
    UARTprintf("  Input Pin: AIN20/PE7\n");
    UARTprintf("  Fclk= %dHz\n\n", SysCtlClockGet());

    //
    // The ADC0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //
    // For this example ADC0 is used with AIN20 on port E7.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.  GPIO port E needs to be enabled
    // so these pins can be used.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Select the analog ADC function for these pins.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_7);

    //
    // Enable sample sequence 2 with a processor signal trigger.  Sequence 2
    // will do a 4 sample when the processor sends a signal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 2.
    //
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 and 1 on sequence 2.  Sample channel 0 (ADC_CTL_CH20) in
    //
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH20); //step 0
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END); //step1
		ADCHardwareOversampleConfigure(ADC0_BASE,64);
    //
    // Since sample sequence 2 is now configured, it must be enabled.
    //
    ADCSequenceEnable(ADC0_BASE, 2);

    //
    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    //
    ADCIntClear(ADC0_BASE, 2);

    //
    // Display the value on the console.
    //
    while(1)
    {
        int32_t TempC, Temp_intC;

        //
        // Trigger the ADC conversion.
        //
        ADCProcessorTrigger(ADC0_BASE, 2);

        //
        // Wait for conversion to be completed.
        //
        while(!ADCIntStatus(ADC0_BASE, 2, false))
        {
        }

        //
        // Clear the ADC interrupt flag.
        //
        ADCIntClear(ADC0_BASE, 2);

        //
        // Read ADC Value.
        //
        ADCSequenceDataGet(ADC0_BASE, 2, pui32ADC0Value);

        //
        // Convert temperature.
        //
        TempC=(1866300 - ((220000 * pui32ADC0Value[0]) / 273)) / 1169;
        Temp_intC=1475 - ((165 * pui32ADC0Value[1]) / 273);
        //
        // Display value on the console.
        //
        UARTprintf("AIN20 = 0x%04x\n", pui32ADC0Value[0]);
        UARTprintf("TS = 0x%04x\n", pui32ADC0Value[1]);
	usprintf(pcBuffer, "T_int = %d.%01uC \n", Temp_intC/10, Temp_intC%10 );
	UARTprintf(pcBuffer);
	usprintf(pcBuffer, "T_ext = %d.%01uC\n\n", TempC/10, TempC%10);
	UARTprintf(pcBuffer);

        //
        // Display value on the LCD.
        //
	sRect.i16XMin = 1;
	sRect.i16XMax = 126;
	sRect.i16YMin = 60;
	sRect.i16YMax = 80;
	GrContextForegroundSet(&sContext, ClrBlack);
	GrRectFill(&sContext, &sRect);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	GrContextForegroundSet(&sContext, ClrRed);


	usprintf(pcBuffer, "T_int = %d.%01uC", Temp_intC/10, Temp_intC%10);
	GrStringDrawCentered(&sContext, pcBuffer, -1, GrContextDpyWidthGet(&sContext) / 2, 60, 1);
	usprintf(pcBuffer, "T_ext = %d.%01uC", TempC/10, TempC%10 );
	GrStringDrawCentered(&sContext, pcBuffer, -1, GrContextDpyWidthGet(&sContext) / 2, 80, 1);
				
				//
        // This function provides a means of generating a constant length
        // delay.
        //
        SysCtlDelay(SysCtlClockGet() / 5);
    }
}
