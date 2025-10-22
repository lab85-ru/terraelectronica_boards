/*
 * adc.h
 *
 *  Created on: 05.11.2009
 *      Author: klen
 */

#ifndef __ADC_H__
#define __ADC_H__

#include "appdefs.h"

typedef struct
		{
	      TestStatus result ;
	      float voltage ;
		} AdcTestResult ;

AdcTestResult* adc_get_test_result();
TestStatus adc_test();

#endif /* __ADC_H__ */
