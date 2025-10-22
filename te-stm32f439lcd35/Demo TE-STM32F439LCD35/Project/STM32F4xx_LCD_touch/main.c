/**
  ******************************************************************************
  * @file    I2C/I2C_IOExpander/main.c 
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

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_IOExpander
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
#define MESSAGE1   "X=%d    "
#define MESSAGE2   "Y=%d    "
#define MESSAGE3   "Z=%d    "
    
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
   uint8_t aTextBuffer[50];
   uint16_t pX, pY;
   static TS_STATE* TS_State;
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
         
  /* Initialize LEDs and push-buttons mounted on STM324x9I-EVAL board */
  STM_EVAL_LEDInit(LED1);

  /* Select the Button test mode (polling or interrupt) BUTTON_MODE in main.h */
//  STM_EVAL_PBInit(BUTTON_WAKEUP, BUTTON_MODE);
//  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE);

  /* Initialize the LCD */
  SDRAM_Init();
  LCD_Init();
 
  /* Enable LCD display */
  LCD_DisplayOn();
  
  /* Set foreground layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  
  /* Clear the LCD */ 
  LCD_Clear(White);
  /* Set the LCD Back Color */
  LCD_SetBackColor(White);
  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue);    
 
  
 
 
  IOE_Config();
  IOE_TSITConfig();
  IOE_IsOperational(); 

   while(1)
  {
    
    
    /* Set the LCD Text Color */
   

    /* Update the structure with the current position */
    TS_State = IOE_TS_GetState();  
    I2C_WriteDeviceRegister(IOE_REG_INT_STA, 0xFF); 
    if(TS_State->TouchDetected)
    {
    LCD_SetTextColor(White); 
    LCD_DrawCircle(pX, (240-pY), 7);

    pX=TS_State->X;
    pY=TS_State->Y;
    
    LCD_SetTextColor(Blue); 
    LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"     TE-STM32F439LCD35     ");
    LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"   Example run STMPE811    ");
    sprintf((char*)aTextBuffer, MESSAGE1, (TS_State->X));
    LCD_DisplayStringLine(LCD_LINE_4, aTextBuffer);
    sprintf((char*)aTextBuffer, MESSAGE2, (TS_State->Y));
    LCD_DisplayStringLine(LCD_LINE_5, aTextBuffer);
    sprintf((char*)aTextBuffer, MESSAGE3, (TS_State->Z));
    LCD_DisplayStringLine(LCD_LINE_6, aTextBuffer);
    LCD_SetTextColor(Red); 
    LCD_DrawCircle((TS_State->X), (240-(TS_State->Y)), 7);
    }
    else
    {
    LCD_SetTextColor(White); 
    LCD_DrawCircle(pX, (240-pY), 7);
    LCD_SetTextColor(Blue); 
    LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"     TE-STM32F439LCD35     ");
    LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"   Example run STMPE811    ");
    sprintf((char*)aTextBuffer, MESSAGE1, (TS_State->X));
    LCD_DisplayStringLine(LCD_LINE_4, aTextBuffer);
    sprintf((char*)aTextBuffer, MESSAGE2, (TS_State->Y));
    LCD_DisplayStringLine(LCD_LINE_5, aTextBuffer);
    sprintf((char*)aTextBuffer, MESSAGE3, (TS_State->Z));
    LCD_DisplayStringLine(LCD_LINE_6, aTextBuffer);
    } 
    
//#ifdef BUTTON_POLLING_MODE
    /* Insert 100 ms delay */
    Delay(5);
    
//#endif
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(uint32_t nTime)
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

  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"assert_param error!!");
  
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
