#include "main.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
volatile uint8_t out_nand[TEST_BUFFER_SIZE];
volatile uint8_t in_nand[TEST_BUFFER_SIZE];

static uint32_t PSRAM_test()
{
  volatile uint32_t* psram = (uint32_t*)PSRAM_TEST_START;
  unsigned int size = 100 ;
  unsigned int i;

  for (i = 0; i < size; i++)
    psram[i] = 0x11;

  for (i = 0; i < size; i++)
    if (psram[i] !=  0x11)
      return (uint32_t)-PSRAM_TEST_FAIL;
	
	return 0;
}


static uint32_t NAND_test(void )
{
	volatile NAND_IDTypeDef id ;
	volatile uint8_t status;

	uint32_t count;
  uint32_t index = 0;

	NAND_ReadID((NAND_IDTypeDef *)&id) ;

	if (id.Maker_ID  != 0xEC || id.Device_ID != 0xDA || id.Third_ID  != 0x10 ||
       id.Fourth_ID != 0x95 )
		return (uint32_t)-NAND_READ_ID_FAIL;

	for (count = 0 ; count< TEST_BUFFER_SIZE ; count++)
		out_nand[count] = count;
  /* Prepare for write : erase block */
  status = NAND_EraseBlock(0);
	if (status)
		return (uint32_t)-NAND_ERASE_BLOCK_FAIL;
	
	/* Prepare out buffer */
	for(index = 0; index < TEST_BUFFER_SIZE; index++)
      out_nand[index] = index;
				
	/* Write page to NAND memory */
	status = NAND_WritePage(0, (uint8_t *)out_nand, TEST_BUFFER_SIZE);
	if (status)
		return (uint32_t)-NAND_WRITE_PAGE_FAIL;

	NAND_ReadPage(0, (uint8_t *)in_nand, TEST_BUFFER_SIZE);
  status = memcmp((void *)in_nand, (void *)out_nand, TEST_BUFFER_SIZE);
	
	if (status)
		return (uint32_t)-NAND_COMPARE_FAIL;

	return 0;
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
	uint32_t error;
	uint32_t delay;
	
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDOn(LED1);

	/* Initialize FSMC: enable PSRAM and NAND */
	INIT_FSMC_pins();
	INIT_FSMC_PSRAM();
	INIT_FSMC_NAND_bank3();
	
	error = PSRAM_test();
	if (!error)
		error = NAND_test();
	
	switch (error) {
		
	case (uint32_t)(-PSRAM_TEST_FAIL):
		delay = 10;
		break;
	case (uint32_t)(-NAND_READ_ID_FAIL) :
		delay = 15;
		break;
	case (uint32_t)(-NAND_ERASE_BLOCK_FAIL) :
		delay = 20;
		break;
	case (uint32_t)(-NAND_WRITE_PAGE_FAIL) :
		delay = 25;	
		break;
	case (uint32_t)(-NAND_COMPARE_FAIL) :
		delay = 30;
	default : 	
	/* NO error */
	delay = 100;
	}

  /* Infinite loop */
  while (1)
  {
    /* Toggle LD4 */
    STM_EVAL_LEDToggle(LED1);

    /* Insert 50 ms delay */
    Delay(delay);
  }
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
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
