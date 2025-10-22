#include "SDRAM.h"
#include "LPC2478.h"
#include <stdint.h>

#define TRP                       (2)
#define TRAS                      (3)
#define TSREX                     (7)
#define TAPR                      (2)
#define TDAL                      (5)
#define TWR                       (1)
#define TRC                       (5)
#define TRFC                      (5)
#define TXSR                      (7)
#define TRRD                      (1)
#define TMRD                      (2)
#define TREF                      (0x001C)

void SDRAM_Initialize(){
  EMCDynamicRP    = TRP;
  EMCDynamicRAS   = TRAS;
  EMCDynamicSREX  = TSREX;
  EMCDynamicAPR   = TAPR;
  EMCDynamicDAL   = TDAL;
  EMCDynamicWR    = TWR;
  EMCDynamicRC    = TRC;
  EMCDynamicRFC   = TRFC;
  EMCDynamicXSR   = TXSR;
  EMCDynamicRRD   = TRRD;
  EMCDynamicMRD   = TMRD;
  EMCDynamicReadConfig = 1;
  EMCDynamicRasCas0 = 0x00000303;

  EMCDynamicConfig0 = 0x00000680;

  volatile int i;
  EMCDynamicControl = 0x00000183; // NOP
  for(i = 100 * 72; i; i--);
  EMCDynamicControl = 0x00000103; // PRECHARGE ALL
  EMCDynamicRefresh = 1;
  for(i = 128; i; i--);
  EMCDynamicRefresh = TREF;
  EMCDynamicControl = 0x00000083; // MODE
  volatile uint32_t temp;
  temp = *(volatile uint32_t *)(0xA0000000 | (0x33 << 12));
  EMCDynamicControl = 0x00000000;
  EMCDynamicConfig0 |= 0x00080000;
}

