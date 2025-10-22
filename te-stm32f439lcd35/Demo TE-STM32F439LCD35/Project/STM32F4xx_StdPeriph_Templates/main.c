/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.2.0RC2
  * @date    20-February-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#if defined (USE_STM324xG_EVAL)
  #define MESSAGE1   "     STM32F40xx     "
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM324xG-EVAL    "

#elif defined (USE_STM324x7I_EVAL) 
  #define MESSAGE1   "     STM32F427x     "
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "  STM324x7I-EVAL    "

#else
  #define MESSAGE1   "          STM32F439X          " 
  #define MESSAGE2   "      Device running on       " 
  #define MESSAGE3   "        STM324x9I-EVAL        "
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40xx.s/startup_stm32f427x.s/startup_stm32f429x.s)
       before to branch to application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */  

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
  /* Initialize LEDs and LCD available on EVAL board **************************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);  
  /* Initialize the LCD */
  LCD_Init();

  /* Display message on LCD ***************************************************/
#if defined (USE_STM324x9I_EVAL) 
  /* Enable The Display */
  LCD_DisplayOn(); 
  /* Set LCD Background Layer  */
  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  /* Clear the Background Layer */ 
  LCD_Clear(LCD_COLOR_MAGENTA);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_MAGENTA);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  
  /* Display LCD messages */
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)MESSAGE1);
  LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)MESSAGE2);
  LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *)MESSAGE3);

  /* Set LCD Foreground Layer  */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

  /* Configure the transparency for foreground */
  LCD_SetTransparency(0);
#endif /* USE_STM324x9I_EVAL */
  
  /* Clear the Foreground Layer */ 
  LCD_Clear(LCD_COLOR_BLUE);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_RED);
  
  /* Display LCD messages */
  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)MESSAGE1);
  LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)MESSAGE2);
  LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)MESSAGE3);

  /* Turn on LEDs *************************************************************/
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);

  /* Add your application code here */
  LCD_SetTextColor(LCD_COLOR_YELLOW);
  LCD_DrawCircle(240, 130, 75);    
  /* Infinite loop */
  int i=0;
  while (1)
  {
    for(i=0;i<255;i++)
    {
    LCD_SetTransparency(i);
    Delay(1);
    }
    for(i=255;i!=0;i--)
    {
    LCD_SetTransparency(i);
    Delay(1);
    }
    
    /* Toggle LD1 */
    STM_EVAL_LEDToggle(LED1);
    /* Insert 50 ms delay */
    Delay(1);

    /* Toggle LD2 */
    STM_EVAL_LEDToggle(LED2);
    /* Insert 50 ms delay */
    Delay(1);
    
    /* Toggle LD3 */
    STM_EVAL_LEDToggle(LED3);
    /* Insert 50 ms delay */
    Delay(1);

    /* Toggle LD4 */    
    STM_EVAL_LEDToggle(LED4);
    /* Insert 50 ms delay */
    Delay(1);
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
