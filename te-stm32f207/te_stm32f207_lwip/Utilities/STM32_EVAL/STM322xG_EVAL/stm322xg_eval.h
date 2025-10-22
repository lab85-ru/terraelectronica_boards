/**
  ******************************************************************************
  * @file    stm322xg_eval.h
  * @author  MCD Application Team
  * @version V4.6.1
  * @date    18-April-2011
  * @brief   This file contains definitions for STM322xG_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM322xG_EVAL_H
#define __STM322xG_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f2xx_dma.h"


 typedef enum
 {
   LED1 = 0,
 } Led_TypeDef;


 typedef enum
 {
   COM1 = 0,
   COM2 = 1
 } COM_TypeDef;


#define LEDn                             1

#define LED1_PIN                         GPIO_Pin_9
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_AHB1Periph_GPIOB
  

#define COMn                             2

#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)


/**
 * @brief Definition for COM port1, connected to USART3
 */ 
#define EVAL_COM1                        USART3
#define EVAL_COM1_CLK                    RCC_APB1Periph_USART3
#define EVAL_COM1_TX_PIN                 GPIO_Pin_8
#define EVAL_COM1_TX_GPIO_PORT           GPIOD
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource8
#define EVAL_COM1_TX_AF                  GPIO_AF_USART3
#define EVAL_COM1_RX_PIN                 GPIO_Pin_9
#define EVAL_COM1_RX_GPIO_PORT           GPIOD
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_RX_AF                  GPIO_AF_USART3
#define EVAL_COM1_IRQn                   USART3_IRQn


#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCC_APB1Periph_USART2
#define EVAL_COM2_TX_PIN                 GPIO_Pin_5
#define EVAL_COM2_TX_GPIO_PORT           GPIOD
#define EVAL_COM2_TX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM2_TX_SOURCE              GPIO_PinSource5
#define EVAL_COM2_TX_AF                  GPIO_AF_USART2
#define EVAL_COM2_RX_PIN                 GPIO_Pin_6
#define EVAL_COM2_RX_GPIO_PORT           GPIOD
#define EVAL_COM2_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM2_RX_SOURCE              GPIO_PinSource6
#define EVAL_COM2_RX_AF                  GPIO_AF_USART2
#define EVAL_COM2_IRQn                   USART2_IRQn


#define CAN1_CLK                    RCC_APB1Periph_CAN1
#define CAN1_RX_PIN                 GPIO_Pin_0
#define CAN1_TX_PIN                 GPIO_Pin_1
#define CAN1_GPIO_PORT              GPIOD
#define CAN1_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define CAN1_AF_PORT                GPIO_AF_CAN1
#define CAN1_RX_SOURCE              GPIO_PinSource0
#define CAN1_TX_SOURCE              GPIO_PinSource1


#define CAN2_CLK                    (RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
#define CAN2_RX_PIN                 GPIO_Pin_5
#define CAN2_TX_PIN                 GPIO_Pin_6
#define CAN2_GPIO_PORT              GPIOB
#define CAN2_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define CAN2_AF_PORT                GPIO_AF_CAN2
#define CAN2_RX_SOURCE              GPIO_PinSource5
#define CAN2_TX_SOURCE              GPIO_PinSource6



void DAC_Ch1_EscalatorConfig(void);
void TIM6_Config(void);
void ADC1_ChTempSensor_Init(void);
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);

void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct); 
void CAN_Config(void);

uint32_t GetADCConvertedValue(void);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM322xG_EVAL_H */
