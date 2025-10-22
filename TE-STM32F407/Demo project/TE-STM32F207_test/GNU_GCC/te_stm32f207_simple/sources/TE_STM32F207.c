 
  
/* Includes ------------------------------------------------------------------*/

#include "stm32f2xx_sdio.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_i2c.h"
#include "stm32f2xx_conf.h"
#include "TE_STM32F207.h"


GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK};

USART_TypeDef* COM_USART[COMn] = {EVAL_COM1}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM1_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM1_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN};
 
const uint8_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM1_TX_SOURCE};

const uint8_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM1_RX_SOURCE};
 
const uint8_t COM_TX_AF[COMn] = {EVAL_COM1_TX_AF};
 
const uint8_t COM_RX_AF[COMn] = {EVAL_COM1_RX_AF};

DMA_InitTypeDef    sEEDMA_InitStructure; 
NVIC_InitTypeDef   NVIC_InitStructure;
uint32_t 	ADCConvertedValue;

const uint8_t Escalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};


void DAC_Ch1_EscalatorConfig(void) {


	/* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;

	DAC_InitTypeDef  DAC_InitStructure;


	/* DMA1 clock and GPIOA clock enable (to be used with DAC) */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOA, ENABLE);

	/* DAC Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	/* DAC channel 1 & 2 (DAC_OUT1 = PA.4)(DAC_OUT2 = PA.5) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	 /* DAC channel2 Configuration */
	  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;//;DAC_WaveGeneration_Triangle;//
	  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
	  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	  /* Enable DAC Channel2 */
	  DAC_Cmd(DAC_Channel_1, ENABLE);

	  /* Set DAC channel2 DHR12RD register */
	  DAC_SetChannel1Data(DAC_Align_12b_R, 0x2000);


}


void TIM6_Config(void)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 0x0F;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}


void ADC1_ChTempSensor_Init(void) {


	  ADC_InitTypeDef       ADC_InitStructure;
	  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	  DMA_InitTypeDef       DMA_InitStructure;

	  void ADC_DeInit(void);

	  /* Enable peripheral clocks *************************************************/
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	  /* DMA2_Stream0 channel0 configuration **************************************/
	  DMA_DeInit(DMA2_Stream0);
	  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCConvertedValue;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	  DMA_InitStructure.DMA_BufferSize = 1;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	  /* DMA2_Stream0 enable */
	  DMA_Cmd(DMA2_Stream0, ENABLE);

	  /* ADC Common Init **********************************************************/
	  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	  ADC_CommonInit(&ADC_CommonInitStructure);

	  /* ADC1 Init ****************************************************************/
	  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_NbrOfConversion = 1;
	  ADC_Init(ADC1, &ADC_InitStructure);

	  /* Enable ADC1 DMA */
	  ADC_DMACmd(ADC1, ENABLE);

	  /* ADC1 regular channel18 (VBAT) configuration ******************************/
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_15Cycles);

	  /* Enable VBAT channel */
	  ADC_TempSensorVrefintCmd(ENABLE);

	  /* Enable DMA request after last transfer (Single-ADC mode) */
	  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	  /* Enable ADC1 **************************************************************/
	  ADC_Cmd(ADC1, ENABLE);


}


void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);


  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval None
  */
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4 
  * @retval None
  */
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];  
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval None
  */
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}


void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

  if (COM == COM1)
  {
    /* Enable UART clock */
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);
    
  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}

