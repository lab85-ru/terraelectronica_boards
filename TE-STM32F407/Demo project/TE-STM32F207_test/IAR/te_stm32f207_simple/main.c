/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
char Message[256];
static __IO uint32_t TimingDelay;
//extern  uint32_t 	ADCConvertedValue;

/* Private function prototypes -----------------------------------------------*/
void NVIC_Config(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{

  RCC_ClocksTypeDef RCC_Clocks;
  GPIO_InitTypeDef  GPIO_InitStructure;
  float temp;

  /* NVIC configuration */
  NVIC_Config();
  
  
   /* Enable the SHUTDOWN/PC13 Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  /* Enable the SHUTDOWN/PC13 Clock */
  GPIO_ResetBits(GPIOC,GPIO_Pin_13);

  
  STM_EVAL_LEDInit(LED1);

  ADC1_ChTempSensor_Init();
  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConv(ADC1);

  TIM6_Config();
  DAC_Ch1_EscalatorConfig();

  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);


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

  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);

  USART3_SendString((char *)Message,sprintf(Message,"\n \r Temperature sensor : "));

  while (1)
  {

   Delay(1000);
   STM_EVAL_LEDToggle(LED1);
   temp= (((3.3/4096)*(ADCConvertedValue))-0.76)/(0.0025);
   USART3_SendString((char *)Message,sprintf(Message,"\n \r %.1f",temp));

  }
}

/**
  * @brief  Configures the nested vectored interrupt controller.
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
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
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
  {
  }
}
#endif

