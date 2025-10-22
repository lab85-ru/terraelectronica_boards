//*****************************************************************************
//
// 	LIS302DLH.c - Driver for MEMS digital output motion sensor.
//	This version uses an SSI interface to the motion sensor.
//
//*****************************************************************************

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "drivers/LIS302DLH.h"

//*****************************************************************************
//
// Defines the SSI and GPIO peripherals that are used for this motion sensor.
//
//*****************************************************************************
#define MOTION_SSI_PERIPH          SYSCTL_PERIPH_SSI3
#define MOTION_SSI_GPIO_PERIPH     SYSCTL_PERIPH_GPIOK

//*****************************************************************************
//
// Defines the GPIO pin configuration macros for the pins that are used for
// the SSI function.
//
//*****************************************************************************
#define MOTION_PINCFG_SSICLK       GPIO_PK0_SSI3CLK
#define MOTION_PINCFG_SSIFSS       GPIO_PK1_SSI3FSS
#define MOTION_PINCFG_SSIRX        GPIO_PK2_SSI3RX
#define MOTION_PINCFG_SSITX        GPIO_PK3_SSI3TX

//*****************************************************************************
//
// Defines the port and pins for the SSI peripheral.
//
//*****************************************************************************
#define MOTION_SSI_PORT            GPIO_PORTK_BASE
#define MOTION_SSI_PINS            (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)


//*****************************************************************************
//
// Defines the SSI peripheral used and the data speed.
//
//*****************************************************************************
#define MOTION_SSI_BASE            SSI3_BASE
#define MOTION_SSI_CLOCK           16000000


//*****************************************************************************
//
//! Write register.
//!
//! \return None.
//
//*****************************************************************************
 void
LIS302DLH_Write_Reg(unsigned char Adr_Reg, unsigned char Data)
{
	while(ROM_SSIBusy(MOTION_SSI_BASE));
  
	ROM_SSIDataPut(MOTION_SSI_BASE, ((Adr_Reg<<8) | Data));
}	
//*****************************************************************************
//
//! Read register.
//!
//! \return None.
//
//*****************************************************************************
unsigned long LIS302DLH_Read_Reg(unsigned char Adr_Reg)
{
    unsigned long ulDataRx;
		while(ROM_SSIBusy(MOTION_SSI_BASE));
		while(SSIDataGetNonBlocking(MOTION_SSI_BASE, &ulDataRx));
    ROM_SSIDataPut(MOTION_SSI_BASE, ((Adr_Reg | 0x80)<<8));
    ROM_SSIDataGet(MOTION_SSI_BASE, &ulDataRx);
		return (ulDataRx &= 0xFF);
}	
//*****************************************************************************
//
//! Initializes the motion driver.
//!
//! \return None.
//
//*****************************************************************************
void
LIS302DLH_Init(void)
{
    //
    // Enable the peripherals used by this driver
    //
    ROM_SysCtlPeripheralEnable(MOTION_SSI_PERIPH);
    ROM_SysCtlPeripheralEnable(MOTION_SSI_GPIO_PERIPH);

    //
    // Select the SSI function for the appropriate pins
    //
    ROM_GPIOPinConfigure(MOTION_PINCFG_SSICLK);
    ROM_GPIOPinConfigure(MOTION_PINCFG_SSIFSS);
    ROM_GPIOPinConfigure(MOTION_PINCFG_SSITX);
    ROM_GPIOPinConfigure(MOTION_PINCFG_SSIRX);


    //
    // Configure the pins for the SSI function
    //
    ROM_GPIOPinTypeSSI(MOTION_SSI_PORT, MOTION_SSI_PINS);


    //
    // Configure the SSI port
    //
    ROM_SSIDisable(MOTION_SSI_BASE);
    ROM_SSIConfigSetExpClk(MOTION_SSI_BASE, ROM_SysCtlClockGet(),
                           SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER,
                           MOTION_SSI_CLOCK, 16);
    ROM_SSIEnable(MOTION_SSI_BASE);
		
		

}

