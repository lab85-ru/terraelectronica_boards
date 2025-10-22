/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : clock-arch.c
 *    Description : Implementation of architecture-specific clock functionality
 *
 *    History :
 *    1. Date        : October 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 59014 $
**************************************************************************/
#include "clock-arch.h"

extern volatile clock_time_t Ticks;
extern void TMR0_Init(uint32_t tps);
/*************************************************************************
 * Function Name: clock_init
 * Parameters: uint32_t IntrPriority
 *
 * Return: none
 *
 * Description: Timer init
 *
 *************************************************************************/
void clock_init(uint32_t IntrPriority)
{
  TMR0_Init(TICK_PER_SEC);
}

/*************************************************************************
 * Function Name: clock_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: The current clock time, measured in system ticks
 *
 *************************************************************************/
clock_time_t clock_time(void)
{
  return(Ticks);
}
