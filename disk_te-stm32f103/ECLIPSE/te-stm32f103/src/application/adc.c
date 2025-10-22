#include <stdlib.h>
#include <math.h>

#include "appdefs.h"
#include "adc.h"

#define RESISTIVE_DIVADER_RATIO 0.33
//--------------------------------------------------------------------------------
AdcTestResult adc_test_result = { FAILED, 0.0};
uint16_t   ADCConvertedValue[256];
//--------------------------------------------------------------------------------
AdcTestResult* adc_get_test_result()
	{
	 return &adc_test_result ;
    }
//--------------------------------------------------------------------------------
void adc_init()
  {
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    // Установка PC.04 (ADC Channel14) как аналоговый вход
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // включение тактирования модуля DMA1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // включение тактирования модуля ADC1 и GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

    /* конфигурация DMA1 channel1 ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(((ADC_TypeDef*)ADC1_BASE)->DR) ;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 256 ;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    // включение DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // конфигурация ADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    // настройка ADC1 regular channel14
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);

    // разрешение ADC1 режима DMA
    ADC_DMACmd(ADC1, ENABLE);

    // разрешение ADC1
    ADC_Cmd(ADC1, ENABLE);

    // сброс калиброви ADC1
    ADC_ResetCalibration(ADC1);
    // ожидание сброса калибровки ADC1
    while(ADC_GetResetCalibrationStatus(ADC1));

    // запуск калибровки ADC1
    ADC_StartCalibration(ADC1);
    // ожидание завершения калибровки ADC1
    while(ADC_GetCalibrationStatus(ADC1));

  }
//----------------------------------------------------------------------
void adc_start()
  {
    // запуск ADC1
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  }
//----------------------------------------------------------------------
void adc_stop()
  {
    // останов ADC1
    ADC_Cmd(ADC1, DISABLE);
  }

TestStatus adc_test()
  {
	memset( ADCConvertedValue , 0 , 256 * 2 ) ;
    adc_init();
    adc_start();
    Delay(100) ;
    adc_stop();

    // анализ полученных данных
    uint32_t val = 0 ;
    for (uint32_t sample = 0 ; sample < 256 ; sample++)
      val += ADCConvertedValue[sample] ;
    ADCConvertedValue[0] = val >> 8 ;

    adc_test_result.voltage =  ADCConvertedValue[0] * (3.3 / 4096.0 ) ;

    if ( fabs(adc_test_result.voltage - RESISTIVE_DIVADER_RATIO * 3.3) < 0.2 )
    {
    	adc_test_result.result = PASSED ;
    }
    return adc_test_result.result  ;
  }
