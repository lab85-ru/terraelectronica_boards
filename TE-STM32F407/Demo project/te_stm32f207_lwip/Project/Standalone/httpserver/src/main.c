/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   Main program body
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
#include "stm32f2x7_eth.h"
#include "stm32f2xx_it.h"
#include "netconf.h"
#include "main.h"
#include "httpd.h"
#include "serial_debug.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;

/* Private function prototypes -----------------------------------------------*/

void USART_Configuration(void);
void NVIC_Config(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured to 
       120 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */  
  
#ifdef SERIAL_DEBUG
  DebugComPort_Init();
#endif

/**
 *  инициализация порта ввод-вывода к которому подключен светодиод VD2
 *  реализация в файле sources\TE_STM32F207.c
 */
  STM_EVAL_LEDInit(LED1);
 
  /**
 * Инициализация USART2,3 реализация находится частично в этом файле,
 * частично в sources\TE_STM32F207.c
 */
  USART_Configuration();
/**
 * Инициализация CAN1,2 реализация в файле sources\TE_STM32F207.c
 */
  CAN_Config();
/**
 * Инициализация таймера общего назначения TIM6 : этот таймер используется
 * при работе ЦАП при генерации сигнала для динамика
 * реализация в файле sources\TE_STM32F207.c
 */
  TIM6_Config();
/**
 * Инициализация ЦАП в режиме автогенерации
 * реализация в файле sources\TE_STM32F207.c
 */
  DAC_Ch1_EscalatorConfig();
/**
 * Инициализация всторенного полупроводникового температурного датчика
 * реализация в файле sources\TE_STM32F207.c
 */
  ADC1_ChTempSensor_Init();

 /* Start ADC1 Software Conversion */
/**
 * Разрешение работы ADC1 (запуск)
 * Бибилиотечная функция, файл STM32F2xx_StdPeriph_Driver\stm32f2xx_adc.c
 */
  ADC_SoftwareStartConv(ADC1);


  /* NVIC configuration */
/**
 * 	Инициализация прерываний от USART2,3 CAN1,2
 * 	Реализация в этом модуле, ниже по коду
 */
  NVIC_Config();

  
  /* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
  ETH_BSP_Config();
 
  /* Initilaize the LwIP stack */
  LwIP_Init();
  
  /* Http webserver Init */
  httpd_init();
  
  Delay(50);  
  
  /* Infinite loop */
  while (1)
  { 
  
    /* check if any packet received */
    if (ETH_CheckFrameReceived())
    { 
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);
  }   
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


void USART_Configuration(void) {

  USART_InitTypeDef USART_InitStructure;

/* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - Two Stop Bit
        - Odd parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART3_buffer_Init();
  STM_EVAL_COMInit(COM1, &USART_InitStructure);
  STM_EVAL_COMInit(COM2, &USART_InitStructure);

  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);

  USART_ITConfig(EVAL_COM2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(EVAL_COM2, USART_IT_TXE, ENABLE);


}


/**
 * @brief  Настройка прерываний от USART2,3 и CAN1,2
 * @param  None
 * @retval None
 */
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
