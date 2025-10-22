
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "appdefs.h"

void SystemStartup(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void Reset_Device(void);
uint32_t GetCpuClock();

#endif  /*__HW_CONFIG_H*/


