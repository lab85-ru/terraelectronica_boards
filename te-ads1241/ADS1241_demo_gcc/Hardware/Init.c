#include "LPC2478.h"
#include "Config.h"
#include "ExRAM.h"
#include "LCD.h"
#include "ADS1241.h"

static void ClockInitialize();
static void PinsInitialize();

void __init(){
  PCONP = _BV(PCONP_PCEMC) | _BV(PCONP_PCLCD) | _BV(PCONP_PCSSP0) | _BV(PCONP_PCTIM1);
  MAMTIM = MAM_FETCH;
  MAMCR = MAM_MODE;
  MEMMAP = MEMMAP_VAL;
  SCS |= _BV(SCS_GPIOM);
  ClockInitialize();
  PinsInitialize();
  EMCControl = 0x00000001;
  ExRAMInitialize();
  LCD_Initialize();
  ADS1241_Initialize();
  T1TCR = _BV(T1TCR_CounterEnable) | _BV(T1TCR_CounterReset);
  T1PR = ((CCLK / 4) / 1000000) - 1;
  T1TCR = _BV(T1TCR_CounterEnable);
}

static inline __attribute__((always_inline)) void PLLFeed(){
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
}

static void ClockInitialize(){
  // Отключаем PLL
  if(PLLSTAT & _BV(PLLSTAT_PLLC)){
    PLLCON = _BV(PLLCON_PLLE);
    PLLFeed();
  }
  PLLCON = 0;
  PLLFeed();
  SCS |= _BV(SCS_OSCEN);
  while(!(SCS & _BV(SCS_OSCSTAT)));
  CLKSRCSEL = 0x01;
  PLLCFG = (MSEL_VAL << PLLCFG_MSEL) | (NSEL_VAL << PLLCFG_NSEL);
  PLLFeed();
  PLLCON = _BV(PLLCON_PLLE);
  PLLFeed();
  CCLKCFG = CCLKCFG_VAL;
  while(PLLSTAT & _BV(PLLSTAT_PLOCK));
  PLLCON = _BV(PLLCON_PLLE) | _BV(PLLCON_PLLC);
  PLLFeed();
}

static void PinsInitialize(){
  PINSEL0 = 0x03055550;
  PINSEL1 = 0x00000000;
  PINSEL2 = 0x0280A8A0;
  PINSEL3 = 0xC5555500;
  PINSEL4 = 0x050FFFFF;
  PINSEL5 = 0x05F13115;
  PINSEL6 = 0x55555555;
  PINSEL8 = 0x55555555;
  PINSEL9 = 0x1A055555;
  PINSEL11 = 0x0000000B;
}

