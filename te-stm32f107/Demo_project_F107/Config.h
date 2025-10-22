#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <stdbool.h>

#define __ON                            (true)
#define __OFF                           (false)

#define __PLL_CONFIG                    __ON
#define __SYSTICK                       __ON
#define __PORT_OUT                      __ON
#define __ADC                           __ON
#define __DAC                           __ON
#define __USART                         __ON

#define __NOISE                         1
#define __TRIANGLE                      3

#define __GENERATION                    __NOISE

#define CALC_TIMRELOAD(time_us, freq_hz)  (((time_us) * ((freq_hz) / 1000000)) - 1)

#define HSE_OSC                         (25000000UL)

#endif // CONFIG_H

