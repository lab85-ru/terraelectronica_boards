#include "main.h"

/* номер страницы, используемой в примере  */
#define PAGE_ADRESS 100
/* размер страницы в байтах дл€ dataflash установленной на TE-STM32F207*/
#define PAGE_SIZE	528/8

/* переменна€ используема€ дл€ формировани€ задержек во времени*/
static __IO uint32_t TimingDelay;

/* test_page - буфер дл€ записи в пам€ть   */
char test_page[PAGE_SIZE],read_page[PAGE_SIZE];

/* структура, определенна€ в dataflash.h хран€ща€ конфигурацию dataflash */
AT91S_DF pDf;

/**
  * @brief   Main program
  *
  * ќсновна€ программа. ƒемонстрирует работу с dataflash.
  * јлгоритм : стрираетс€ страница номер PAGE_ADRESS, в страницу с номеров
  * PAGE_ADRESS пишетс€ тестовый буфер test_page, из страница с номером
  * PAGE_ADRESS читаютс€ данные в буфер read_page и сравниваютс€ с ранее
  * записанными. ≈сли в процессе исполнени€ не произошло ошибки то мигает
  * светодиод D4. ≈сли была ошибка, то светодиод не мигает.
  */
int main(void)
{

  RCC_ClocksTypeDef RCC_Clocks;
  int count;

  /* »нициализаци€ системного таймера */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

  /* инициализаци€ test_page буфера */
  for (count=0;count<PAGE_SIZE;count++) test_page[count]=(char )count;

  /* инициализаци€ управлени€ светодиодом D4 */
  STM_EVAL_LEDInit(LED1);

  /* инициализаци€ интерфейса SPI3*/
  sFLASH_Init();
  /*  проверка наличи€ и работоспособности dataflash */
  if (df_init(&pDf)) {
	  Delay(1000);
	  STM_EVAL_LEDToggle(LED1);
	  Delay(1000);
	  STM_EVAL_LEDToggle(LED1);
	  Delay(500);
	  /* стирание странциы  PAGE_ADRESS */
	  df_page_erase(&pDf,PAGE_ADRESS);
	  Delay(500);
	  /* запись страницы в dataflash пам€ть */
	  df_page_write(&pDf, &test_page[0], PAGE_SIZE, PAGE_ADRESS);
	  Delay(500);
	  /* чтение страницы из dataflash пам€ти */
	  df_page_read(&pDf, &read_page[0], PAGE_SIZE, PAGE_ADRESS);
	  Delay(500);

	  for (count=0;count<PAGE_SIZE;count++) test_page[count]=(char )count;

	  /* проверка соотвестви€ записанной информации с прочитанной информацией */
	  for (count=0;count<PAGE_SIZE;count++) if (test_page[count]!=
			  read_page[count]) for (;;);

  } else for(;;);


  while(1) {
	  STM_EVAL_LEDToggle(LED1);
	  Delay(1000);
  };
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

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
