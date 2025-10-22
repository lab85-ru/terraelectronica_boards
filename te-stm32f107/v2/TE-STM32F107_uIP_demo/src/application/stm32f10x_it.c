/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "appdefs.h"
#include "stm32f10x_it.h"
#include "stm32_eth.h"
#include "clock.h"
#include "uip_handler.h"

void NMIExceptionHandler(void)
{
}

void __attribute__((naked)) HardFaultExceptionHandler(void)
{
  // чтение PSP и сохранение адреса возврата из стека
  asm volatile (
	             "TST LR, #4              \n"
	             "ITE EQ                  \n"
	             "MRSEQ R0, MSP           \n"
	             "MRSNE R0, PSP           \n"
		         "LDR   R10, [R0,#24]     \n"
	    );
  while (1)
  {
    asm volatile  ("nop");
  }
}

void __attribute__((naked)) MemManageExceptionHandler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
	  asm volatile  ("nop");
  }
}

void __attribute__((naked)) BusFaultExceptionHandler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
	  asm volatile  ("nop");
  }
}

void __attribute__((naked)) UsageFaultExceptionHandler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
	  asm volatile  ("nop");
  }
}

void __attribute__((naked)) DebugMonitorHandler(void)
{}

#if (USE_FREERTOS==0)
  void SVCHandler(void)
   {}
  void PendSVCHandler(void)
   {}
//---------------------------------------------------------------------
  void SysTickHandler(void)
   {
	// инкремент времени
	clock_time_val++ ;
	// установка флага запроса
	uip_irqs |= uiPeriodic ;
   }
#endif
//---------------------------------------------------------------------
void WWDG_IRQHandler(void)
{}

void PVD_IRQHandler(void)
{}

void TAMPER_IRQHandler(void)
{}

void RTC_IRQHandler(void)
{}

void FLASH_IRQHandler(void)
{}

void RCC_IRQHandler(void)
{}

void EXTI0_IRQHandler(void)
{}

void EXTI1_IRQHandler(void)
{}

void EXTI2_IRQHandler(void)
{}

void EXTI3_IRQHandler(void)
{}

void EXTI4_IRQHandler(void)
{}

void DMA1_Channel1_IRQHandler(void)
{}

void DMA1_Channel2_IRQHandler(void)
{}

void DMA1_Channel3_IRQHandler(void)
{}

void DMA1_Channel4_IRQHandler(void)
{}

void DMA1_Channel5_IRQHandler(void)
{}

void DMA1_Channel6_IRQHandler(void)
{}

void DMA1_Channel7_IRQHandler(void)
{}
void ADC1_2_IRQHandler(void)
{}
//----------------------------------------------------------------
void CAN1_TX_IRQHandler(void)
{}
//----------------------------------------------------------------
void CAN1_RX0_IRQHandler(void)
{
	  // чтение пакета из FIFO
	  CanRxMsg RxMessage;
	  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	  // валидация принятого пакета
	  if ((RxMessage.StdId == 0x321)    &&
	      (RxMessage.IDE == CAN_ID_STD) &&
	      (RxMessage.DLC == 1)          &&
	      (RxMessage.Data[0] == 0x55))
	   {
		  extern volatile uint32_t can_test_result ;
		  can_test_result ++ ;
	   }
	  CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
}
//----------------------------------------------------------------
void CAN1_RX1_IRQHandler(void)
{}
//----------------------------------------------------------------
void CAN1_SCE_IRQHandler(void)
{}
//----------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{}
void TIM1_BRK_IRQHandler(void)
{}
void TIM1_UP_IRQHandler(void)
{}
void TIM1_TRG_COM_IRQHandler(void)
{}
void TIM1_CC_IRQHandler(void)
{}
#if (USE_FREERTOS==0)
  void TIM2_IRQHandler(void)
   {}
#endif
void TIM3_IRQHandler(void)
{}
void TIM4_IRQHandler(void)
{}
void I2C1_EV_IRQHandler(void)
{}
void I2C1_ER_IRQHandler(void)
{}
void I2C2_EV_IRQHandler(void)
{}
void I2C2_ER_IRQHandler(void)
{}
void SPI1_IRQHandler(void)
{}
void SPI2_IRQHandler(void)
{}
void USART1_IRQHandler(void)
{}
void USART2_IRQHandler(void)
{}
void USART3_IRQHandler(void)
{}
void EXTI15_10_IRQHandler(void)
{}
void RTCAlarm_IRQHandler(void)
{}
void OTG_FS_WKUP_IRQHandler(void)
{}
void TIM5_IRQHandler(void)
{}
void SPI3_IRQHandler(void)
{}
void UART4_IRQHandler(void)
{}
void UART5_IRQHandler(void)
{}
void TIM6_IRQHandler(void)
{}
void TIM7_IRQHandler(void)
{}
void DMA2_Channel1_IRQHandler(void)
{}
void DMA2_Channel2_IRQHandler(void)
{}
void DMA2_Channel3_IRQHandler(void)
{}
void DMA2_Channel4_IRQHandler(void)
{}
void DMA2_Channel5_IRQHandler(void)
{}
//----------------------------------------------------------------
void ETH_IRQHandler(void)
{
	// сброс  Eth DMA Rx IT pending флагов
	ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);

	#if (USE_FREERTOS==1)
	   extern xTaskHandle uip_recv_task_handle ;
	   xTaskResumeFromISR(uip_recv_task_handle);
    #else
	   // установка флага запроса
	   uip_irqs |= uitRecv ;
    #endif


}
//----------------------------------------------------------------
void ETH_WKUP_IRQHandler(void)
{}
void CAN2_TX_IRQHandler(void)
{}
void CAN2_RX0_IRQHandler(void)
{
  // чтение пакета из FIFO
  CanRxMsg RxMessage;
  CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
  // валидация принятого пакета
  if ((RxMessage.StdId == 0x321)    &&
      (RxMessage.IDE == CAN_ID_STD) &&
      (RxMessage.DLC == 1)          &&
      (RxMessage.Data[0] == 0x55))
   {
	  extern volatile uint32_t can_test_result ;
	  can_test_result ++ ;
   }
  CAN_ClearITPendingBit(CAN2, CAN_IT_FF0);
}
void CAN2_RX1_IRQHandler(void)
{}
void CAN2_SCE_IRQHandler(void)
{}
void OTG_FS_IRQHandler(void)
{}
void BootRAM(void)     // @0x1E0. This is for boot in RAM mode for
{}

