/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   This file contains the headers of the interrupt handlers.
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
#ifndef __STM32F2xx_IT_H
#define __STM32F2xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
 /*----------------------------------------------------------------------------
   Notes:
   The length of the receive and transmit buffers must be a power of 2.
   Each buffer has a next_in and a next_out index.
   If next_in = next_out, the buffer is empty.
   (next_in - next_out) % buffer_size = the number of characters in the buffer.
  *----------------------------------------------------------------------------*/
 #define TBUF_SIZE   256	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
 #define RBUF_SIZE   256      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

 /*----------------------------------------------------------------------------
  *----------------------------------------------------------------------------*/
 #if TBUF_SIZE < 2
 #error TBUF_SIZE is too small.  It must be larger than 1.
 #elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
 #error TBUF_SIZE must be a power of 2.
 #endif

 #if RBUF_SIZE < 2
 #error RBUF_SIZE is too small.  It must be larger than 1.
 #elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
 #error RBUF_SIZE must be a power of 2.
 #endif

 /*----------------------------------------------------------------------------
  *----------------------------------------------------------------------------*/


 /* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USARTx_IRQHandler(void);


void USART3_buffer_Init (void);
int USART3_GetKey (void);
int USART3_SendChar (int c);
void USART3_SendString(char *mass,int size);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F2xx_IT_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
