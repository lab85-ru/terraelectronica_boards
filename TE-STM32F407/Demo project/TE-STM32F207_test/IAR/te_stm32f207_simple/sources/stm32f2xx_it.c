/**
  ******************************************************************************
  * @file    USART/HyperTerminal_Interrupt/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "TE_STM32F207.h"
#include "main.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_HyperTerminal_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USARTx_IRQHANDLER   USART3_IRQHandler


/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/

struct buf_st {
  unsigned int in;                                // Next In Index
  unsigned int out;                               // Next Out Index
  char buf [RBUF_SIZE];                           // Buffer
};


static struct buf_st rbuf = { 0, 0, };
#define SIO_RBUFLEN ((unsigned short)(rbuf.in - rbuf.out))

static struct buf_st tbuf = { 0, 0, };
#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))

static unsigned int tx_restart = 1;               // NZ if TX restart is required


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/******************************************************************************/
/*            STM32F2xx Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USARTx global interrupt request.
  * @param  None
  * @retval None
  */
void USARTx_IRQHANDLER(void)
{
	  volatile unsigned int IIR;
	  struct buf_st *p;

	    IIR = USART3->SR;
	    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
	      USART3->SR &= ~USART_FLAG_RXNE;	          // clear interrupt

	      p = &rbuf;

	      if (((p->in - p->out) & ~(RBUF_SIZE-1)) == 0) {
	        p->buf [p->in & (RBUF_SIZE-1)] = (USART3->DR & 0x1FF);
	        p->in++;
	      }
	    }

	    if (IIR & USART_FLAG_TXE) {
	      USART3->SR &= ~USART_FLAG_TXE;	          // clear interrupt

	      p = &tbuf;

	      if (p->in != p->out) {
	        USART3->DR = (p->buf [p->out & (TBUF_SIZE-1)] & 0x1FF);
	        p->out++;
	        tx_restart = 0;
	      }
	      else {
	        tx_restart = 1;
			USART3->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send

	      }
	    }
}

/*------------------------------------------------------------------------------
  buffer_Init
  initialize the buffers
 *------------------------------------------------------------------------------*/
void USART3_buffer_Init (void) {

  tbuf.in = 0;                                    // Clear com buffer indexes
  tbuf.out = 0;
  tx_restart = 1;

  rbuf.in = 0;
  rbuf.out = 0;
}

/*------------------------------------------------------------------------------
  SenChar
  transmit a character
 *------------------------------------------------------------------------------*/
int USART3_SendChar (int c) {
  struct buf_st *p = &tbuf;

                                                  // If the buffer is full, return an error value
  if (SIO_TBUFLEN >= TBUF_SIZE)
    return (-1);

  p->buf [p->in & (TBUF_SIZE - 1)] = c;           // Add data to the transmit buffer.
  p->in++;

  if (tx_restart) {                               // If transmit interrupt is disabled, enable it
    tx_restart = 0;
	USART3->CR1 |= USART_FLAG_TXE;		          // enable TX interrupt
  }

  return (0);
}

/*------------------------------------------------------------------------------
  GetKey
  receive a character
 *------------------------------------------------------------------------------*/
int USART3_GetKey (void) {
  struct buf_st *p = &rbuf;

  if (SIO_RBUFLEN == 0)
    return (-1);

  return (p->buf [(p->out++) & (RBUF_SIZE - 1)]);
}




void USART3_SendString(char *mass,int size) {

	while (size > 0) {
		USART3_SendChar((int )*mass++);
	size--;
	}
}



/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
