#ifndef __APPDEFS_H__
#define __APPDEFS_H__

#include <stdint.h>
#include <string.h>

#define _BV(val) (1<<val)

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#ifdef __arm__

  #include "platform_config.h"
  #include "stm32f10x_conf.h"
  #include "stm32f10x.h"

  #if (USE_FREERTOS==1)
    #include "freertos"
  #endif

  #define  NOP()   asm volatile ("nop")
  #define  NULL    (void*)0
  #define __RAM__  __attribute__ ((section(".fast")))

  inline void DelayMsec(uint32_t msec)
  {
    uint32_t tmp = 7000 * msec  ;
    while( tmp-- )
     {
       NOP();
     }
  }

#endif



#endif /*__APPDEFS_H__*/
