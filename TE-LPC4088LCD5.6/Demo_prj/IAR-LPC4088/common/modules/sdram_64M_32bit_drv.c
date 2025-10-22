/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sdram_64M_32bit_drv.c
 *    Description : SDRAM driver K4S561632E-TC(L)75 16MBx16bitx2
 *
 *    History :
 *    1. Date        : 29, October 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 60270 $
 **************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "LPC407x_8x.h"

#define SYS_FREQ  120000000

extern uint32_t SDRAM_BASE_ADDR;

#if     SYS_FREQ == (120000000)
#define SDRAM_PERIOD          8.3   // 120MHz
#elif   SYS_FREQ == (96000000)
#define SDRAM_PERIOD          10.4  // 96MHz
#elif   SYS_FREQ == (72000000)
#define SDRAM_PERIOD          13.8  // 72MHz
#elif   SYS_FREQ == (57000000)
#define SDRAM_PERIOD          17.4  // 57.6MHz
#elif   SYS_FREQ == (48000000)
#define SDRAM_PERIOD          20.8  // 48MHz
#elif   SYS_FREQ == (36000000)
#define SDRAM_PERIOD          27.8  // 36MHz
#elif   SYS_FREQ == (24000000)
#define SDRAM_PERIOD          41.7  // 24MHz
#elif   SYS_FREQ == (12000000)
#define SDRAM_PERIOD          83.3  // 24MHz
#else
#error Frequency not defined
#endif

#define P2C(Period)           (((Period<SDRAM_PERIOD)?0:(uint32_t)((float)Period/SDRAM_PERIOD)))

#define SDRAM_REFRESH         7813
#define SDRAM_TRP             20
#define SDRAM_TRAS            45
#define SDRAM_TAPR            1
#define SDRAM_TDAL            2
#define SDRAM_TWR             2
#define SDRAM_TRC             65
#define SDRAM_TRFC            65
#define SDRAM_TXSR            67
#define SDRAM_TRRD            15
#define SDRAM_TMRD            3

/*************************************************************************
 * Function Name: SDRAM_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: SDRAM controller and memory init
 *
 *************************************************************************/
void SDRAM_Init (void)
{
  /* Enable EMC clock*/
  LPC_SC->PCONP |= (1<<11);
  /*The EMC uses the same clock as the CPU*/
  LPC_SC->EMCCLKSEL = 0;
  /*Assign pins to SDRAM controller*/
  LPC_IOCON->P2_16 = 0x21;
  LPC_IOCON->P2_17 = 0x21;
  LPC_IOCON->P2_18 = 0x21;
  LPC_IOCON->P2_20 = 0x21;
  LPC_IOCON->P2_24 = 0x21;
  LPC_IOCON->P2_28 = 0x21;
  LPC_IOCON->P2_29 = 0x21;
  LPC_IOCON->P2_30 = 0x21;
  LPC_IOCON->P2_31 = 0x21;
  LPC_IOCON->P3_0  = 0x21;
  LPC_IOCON->P3_1  = 0x21;
  LPC_IOCON->P3_2  = 0x21;
  LPC_IOCON->P3_3  = 0x21;
  LPC_IOCON->P3_4  = 0x21;
  LPC_IOCON->P3_5  = 0x21;
  LPC_IOCON->P3_6  = 0x21;
  LPC_IOCON->P3_7  = 0x21;
  LPC_IOCON->P3_8  = 0x21;
  LPC_IOCON->P3_9  = 0x21;
  LPC_IOCON->P3_10 = 0x21;
  LPC_IOCON->P3_11 = 0x21;
  LPC_IOCON->P3_12 = 0x21;
  LPC_IOCON->P3_13 = 0x21;
  LPC_IOCON->P3_14 = 0x21;
  LPC_IOCON->P3_15 = 0x21;
  LPC_IOCON->P3_16 = 0x21;
  LPC_IOCON->P3_17 = 0x21;
  LPC_IOCON->P3_18 = 0x21;
  LPC_IOCON->P3_19 = 0x21;
  LPC_IOCON->P3_20 = 0x21;
  LPC_IOCON->P3_21 = 0x21;
  LPC_IOCON->P3_22 = 0x21;
  LPC_IOCON->P3_23 = 0x21;
  LPC_IOCON->P3_24 = 0x21;
  LPC_IOCON->P3_25 = 0x21;
  LPC_IOCON->P3_26 = 0x21;
  LPC_IOCON->P3_27 = 0x21;
  LPC_IOCON->P3_28 = 0x21;
  LPC_IOCON->P3_29 = 0x21;
  LPC_IOCON->P3_30 = 0x21;
  LPC_IOCON->P3_31 = 0x21;
  LPC_IOCON->P4_0  = 0x21;
  LPC_IOCON->P4_1  = 0x21;
  LPC_IOCON->P4_2  = 0x21;
  LPC_IOCON->P4_3  = 0x21;
  LPC_IOCON->P4_4  = 0x21;
  LPC_IOCON->P4_5  = 0x21;
  LPC_IOCON->P4_6  = 0x21;
  LPC_IOCON->P4_7  = 0x21;
  LPC_IOCON->P4_8  = 0x21;
  LPC_IOCON->P4_9  = 0x21;
  LPC_IOCON->P4_10 = 0x21;
  LPC_IOCON->P4_11 = 0x21;
  LPC_IOCON->P4_12 = 0x21;
  LPC_IOCON->P4_13 = 0x21;
  LPC_IOCON->P4_14 = 0x21;
  LPC_IOCON->P4_25 = 0x21;
  /*Init SDRAM controller*/
  LPC_SC->EMCDLYCTL = (0xF<<0) |  /* CMDDLY */
                     (0x15<<8) |  /* FBCLKDLY */
                      (0x0<<16);  /* CLKOUT0DLY */

  LPC_EMC->Control      = 1;      /* enable EMC */
  LPC_EMC->DynamicReadConfig = 1;
  LPC_EMC->DynamicRasCas0 = 0x303;
  LPC_EMC->DynamicRP = P2C(SDRAM_TRP);
  LPC_EMC->DynamicRAS = P2C(SDRAM_TRAS);
  LPC_EMC->DynamicSREX = P2C(SDRAM_TXSR);
  LPC_EMC->DynamicAPR = SDRAM_TAPR;
  LPC_EMC->DynamicDAL = SDRAM_TDAL+P2C(SDRAM_TRP);
  LPC_EMC->DynamicWR = SDRAM_TWR;
  LPC_EMC->DynamicRC = P2C(SDRAM_TRC);
  LPC_EMC->DynamicRFC = P2C(SDRAM_TRFC);
  LPC_EMC->DynamicXSR = P2C(SDRAM_TXSR);
  LPC_EMC->DynamicRRD = P2C(SDRAM_TRRD);
  LPC_EMC->DynamicMRD = SDRAM_TMRD;
  LPC_EMC->DynamicConfig0 = 0x0004680;        // 13 row, 9 - col, SDRAM
  // JEDEC General SDRAM Initialization Sequence
  // DELAY to allow power and clocks to stabilize ~100 us
  // NOP
  LPC_EMC->DynamicControl = 0x0183;
  for(volatile uint32_t i = 200*30; i;i--);
  // PALL
  LPC_EMC->DynamicControl = 0x0103;
  LPC_EMC->DynamicRefresh = 1;
  for(volatile uint32_t i= 256; i; --i); // > 128 clk
  LPC_EMC->DynamicRefresh = P2C(SDRAM_REFRESH) >> 4;
  // COMM
  LPC_EMC->DynamicControl = 0x0083;
  // Burst 4, Sequential, CAS-3
  volatile unsigned long Dummy = *(volatile unsigned short *)
                                  ((uint32_t)&SDRAM_BASE_ADDR + (0x32UL << (13)));
  // NORM
  LPC_EMC->DynamicControl = 0x0000;
  LPC_EMC->DynamicConfig0 = 0x00084680;
  for(volatile uint32_t i = 100000; i;i--);
}
