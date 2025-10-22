#ifndef __APPDEFS_H__
#define __APPDEFS_H__

#include <stdint.h>
#include <string.h>

#define _BV(val) (1<<val)

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

typedef enum
{
  stsSocketEmpty       ,
  stsCardInSocket      ,
  stsSDItitialize      ,
  stsCardInfo          ,
  stsSDSelectDeselect  ,
  stsSDEnableWideBusOperation  ,
  stsSDSetDeviceMode   ,
  stsFaFsMount         ,
  stsFatFsOpen         ,
  stsFatFsWrite        ,
  stsFatFsClose

} TSdioTestStage ;

typedef enum
{ vridMemoryAccess = 0  ,  // ������/������ ������ �� ����������� ������
  vridLibVersion        ,  // ������ ������ ���������
  //-------------------------------------------------------
  vridGetTestSDIO       ,  // ������ ���������� ������������ SDIO
  vridGetTestADC        ,  // ������ ���������� ������������ ADC
  vridGetTestCAN        ,  // ������ ���������� ������������ CAN
  vridGetCpuClock        ,
  //-------------------------------------------------------
  vridRequestCount
} VendorRequestId ;

#ifdef __arm__

  #include "platform_config.h"
  #include "stm32f10x_conf.h"
  #include "stm32f10x.h"

  #define  NOP()   asm volatile ("nop")
  #define  NULL    (void*)0
  #define __RAM__  __attribute__ ((section(".fast")))

  inline void Delay(uint32_t msec)
  {
    uint32_t tmp = 7000 * msec  ;
    while( tmp-- )
     {
       NOP();
     }
  }

  typedef enum
  {
    ucmCan ,
    ucmUsb
  }TUsbModeMode ;

  extern volatile TUsbModeMode usb_can_mode ;
  extern volatile uint32_t can_activity ;

#endif


  #endif /*__APPDEFS_H__*/
