#ifndef __DEBUG_LED_H__
#define __DEBUG_LED_H__

#include "appdefs.h"

void DebugLedInit() __RAM__ ;
void DebugLedsToggle() ;
void DebugLedInitIndication( TestStatus can_state , TestStatus adc_state , TestStatus sd_state ) ;

#endif /*__DEBUG_LED_H__*/

