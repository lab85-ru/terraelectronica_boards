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

#include "main.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_HyperTerminal_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/

CanRxMsg RxMessage1,RxMessage2;


struct buf_st {
  unsigned int in;                                // Next In Index
  unsigned int out;                               // Next Out Index
  char buf [RBUF_SIZE];                           // Buffer
};


static struct buf_st rbuf = { 0, 0, };
#define SIO_RBUFLEN ((unsigned short)(rbuf.in - rbuf.out))

static struct buf_st tbuf = { 0, 0, };
#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))

static struct buf_st tbuf1 = { 0, 0, };
#define SIO_TBUFLEN1 ((unsigned short)(tbuf1.in - tbuf1.out))

static unsigned int tx_restart = 1;               // NZ if TX restart is required
static unsigned int tx_restart1 = 1;              // NZ if TX restart is required

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
	Time_Update();
}

/******************************************************************************/
/*            STM32F2xx Peripherals Interrupt Handlers                        */
/******************************************************************************/



void USART2_IRQHandler(void)
{
	  volatile unsigned int IIR;
	  struct buf_st *p;

	    IIR = USART2->SR;
	    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
	      USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt

	      p = &rbuf;

	      if (((p->in - p->out) & ~(RBUF_SIZE-1)) == 0) {
	        p->buf [p->in & (RBUF_SIZE-1)] = (USART2->DR & 0x1FF);
	        p->in++;
	      }
	    }

	    if (IIR & USART_FLAG_TXE) {
	      USART2->SR &= ~USART_FLAG_TXE;	          // clear interrupt

	      p = &tbuf1;

	      if (p->in != p->out) {
	        USART2->DR = (p->buf [p->out & (TBUF_SIZE-1)] & 0x1FF);
	        p->out++;
	        tx_restart1 = 0;
	      }
	      else {
	        tx_restart1 = 1;
			USART2->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send

	      }
	    }
}


/**
  * @brief  This function handles USARTx global interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
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



/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	int count;
	struct buf_st *p;

	p = &rbuf;

	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage1);

	for (count=0;count<8;count++) {
		if (((p->in - p->out) & ~(RBUF_SIZE-1)) == 0) {
			p->buf [p->in & (RBUF_SIZE-1)] = RxMessage1.Data[count];
			p->in++;
		}
	}
}
/* USE_CAN1 */


/**
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
  CAN_Receive(CAN2, CAN_FIFO1, &RxMessage2);

  if ((RxMessage2.StdId == 0x321)&&(RxMessage2.IDE == CAN_ID_STD) && (RxMessage2.DLC == 1))
  {
  //  LED_Display(RxMessage.Data[0]);
  //  KeyNumber = RxMessage.Data[0];
  }
}

void CAN2_RX1_IRQHandler(void)
{
	int count;
	struct buf_st *p;

	p = &rbuf;

	CAN_Receive(CAN2, CAN_FIFO1, &RxMessage2);

	for (count=0;count<8;count++) {
		if (((p->in - p->out) & ~(RBUF_SIZE-1)) == 0) {
			p->buf [p->in & (RBUF_SIZE-1)] = RxMessage2.Data[count];
			p->in++;
		}
	}
}


/*------------------------------------------------------------------------------
  buffer_Init
  initialize the buffers
 *------------------------------------------------------------------------------*/
void USART3_buffer_Init (void) {

  tbuf.in = 0;                                    // Clear com buffer indexes
  tbuf1.in = 0;
  tbuf.out = 0;
  tbuf1.out = 0;
  tx_restart = 1;
  tx_restart1 = 1;


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
  SenChar
  transmit a character
 *------------------------------------------------------------------------------*/
int USART2_SendChar (int c) {
  struct buf_st *p = &tbuf1;

                                                  // If the buffer is full, return an error value
  if (SIO_TBUFLEN1 >= TBUF_SIZE)
    return (-1);

  p->buf [p->in & (TBUF_SIZE - 1)] = c;           // Add data to the transmit buffer.
  p->in++;

  if (tx_restart1) {                               // If transmit interrupt is disabled, enable it
    tx_restart1 = 0;
	USART2->CR1 |= USART_FLAG_TXE;		          // enable TX interrupt
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



void USART2_SendString(char *mass,int size) {

	while (size > 0) {
		USART2_SendChar((int )*mass++);
	size--;
	}
}

void USART3_SendString(char *mass,int size) {

	while (size > 0) {
		USART3_SendChar((int )*mass++);
	size--;
	}
}


/**
  * @brief  This function handles External line 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
  {
    //Eth_Link_ITHandler(DP83848_PHY_ADDRESS);
    /* Clear interrupt pending bit */
   //XTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
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
