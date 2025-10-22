/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithPFC/main.c 
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

/** @addtogroup DMA2D_MemToMemWithPFC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 
/* DMA2D output address and input for LCD */
uint32_t aBufferResult[18000];

/* Private function prototypes -----------------------------------------------*/
static void DMA2D_Config(void);
static void LCD_Config(void);
static void LCD_AF_GPIOConfig(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    /* At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f429x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */     
       
  /* Example description.
     ------------------------------------------
  This example provides a description of how to configure DMA2D periph in 
  Memory_to_Memory with pixel format conversion transfer mode and display the 
  result on LCD.
  In this case two images will be displayed on the LCD, original image before 
  the pixel format conversion and the transferred data after pixel format conversion
  In this example the pixel format conversion is done from ARGB8888 to ARGB4444 
  and you can see on LCD the difference between the two images
   */  
  
  /* DMA2D configuration */
  DMA2D_Config();
  
  /* Start Transfer */
  DMA2D_StartTransfer(); 
  
  /* Wait for CTC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  }  

  /* Configure LCD */
  LCD_Config();
  
  /* Enable Layer1 */
  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  
  /* Enable Layer2 */
  LTDC_LayerCmd(LTDC_Layer2, ENABLE);
  
    
  /* Reload configuration of Layer1 & Layer2 */
  LTDC_ReloadConfig(LTDC_IMReload);
  
  /* Enable The LCD */
  LTDC_Cmd(ENABLE);

  while (1)
  {
  }
}

/**
  * @brief DMA2D configuration.
  * @note  This function Configure tha DMA2D peripheral :
  *        1) Configure the transfer mode : memory to memory W/ pixel format conversion
  *        2) Configure the output color mode as ARGB4444
  *        3) Configure the output memory address at SRAM memory  
  *        4) Configure the data size : 320x120 (pixels)  
  *        5) Configure the input color mode as ARGB8888
  *        6) Configure the input memory address at FLASH memory 
  * @retval
  *  None
  */

static void DMA2D_Config(void)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;
  DMA2D_FG_InitTypeDef   DMA2D_FG_InitStruct;

  /* Enable the DMA2D Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);
  
  /* Transfer mode => Memory to memory with pixel format conversion*/
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_M2M_PFC; 
  /* Output color mode */
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_ARGB4444; 
  /* Output Address at SRAM memory */
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = (uint32_t)&aBufferResult;
  /* Initialize the alpha and RGB values */
  DMA2D_InitStruct.DMA2D_OutputGreen = 0;
  DMA2D_InitStruct.DMA2D_OutputBlue = 0;
  DMA2D_InitStruct.DMA2D_OutputRed = 0;
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0;
  /* Initialize the output offset */
  DMA2D_InitStruct.DMA2D_OutputOffset = 0;
  /* Number of lines : height */
  DMA2D_InitStruct.DMA2D_NumberOfLine = 120; 
  /* Number of pixel per line : width */
  DMA2D_InitStruct.DMA2D_PixelPerLine = 300;
  
  /* Initialize DMA2D */
  DMA2D_Init(&DMA2D_InitStruct);

  /* Configure default values for foreground */
  DMA2D_FG_StructInit(&DMA2D_FG_InitStruct);
  
  /* Input Address at flash memory */
  DMA2D_FG_InitStruct.DMA2D_FGMA = (uint32_t)&ARGB8888_300x120;
  /* Input color mode */
  DMA2D_FG_InitStruct.DMA2D_FGCM = CM_ARGB8888;
  
  /* Initialize foreground */
  DMA2D_FGConfig(&DMA2D_FG_InitStruct);
}

/**
  * @brief LCD configuration.
  * @note  This function Configure tha LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - ARGB8888 as pixel format  
  *           - The frame buffer is located at internal RAM : The output of DMA2D transfer
  *           - The Layer size configuration : 300x120
  *        4) Configure the LTDC Layer 2 :
  *           - ARGB4444 as pixel format  
  *           - The frame buffer is located at internal RAM : The output of DMA2D transfer
  *           - The Layer size configuration : 300x120    
  * @retval
  *  None
  */

static void LCD_Config(void)
{
  LTDC_InitTypeDef       LTDC_InitStruct;
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
  
/* Enable Pixel Clock --------------------------------------------------------*/
  /* Configure PLLSAI prescalers for LCD */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/8 = 8 Mhz */
  RCC_PLLSAIConfig(192, 7, 3);
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
  
  /* Enable PLLSAI Clock */
  RCC_PLLSAICmd(ENABLE);
  /* Wait for PLLSAI activation */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
  {
  }  
  
  /* Enable the LTDC Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);  
  
  /* Configure the LCD Control pins */
  LCD_AF_GPIOConfig();
  
  /* Sets LCD backlight and display On */
  GPIO_SetBits(GPIOA, GPIO_Pin_8);
  GPIO_SetBits(GPIOC, GPIO_Pin_6);

  /* LCD Configuration  (more details about LCD configuration in LCD examples)*/
  
  /* Timing configuration */
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;       
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;      
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;    
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;     
  LTDC_InitStruct.LTDC_HorizontalSync = 40;                
  LTDC_InitStruct.LTDC_VerticalSync = 9;                  
  LTDC_InitStruct.LTDC_AccumulatedHBP = 42;                
  LTDC_InitStruct.LTDC_AccumulatedVBP = 11;                
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 522;           
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 283;           
  LTDC_InitStruct.LTDC_TotalWidth = 524;                   
  LTDC_InitStruct.LTDC_TotalHeigh = 285;                   
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0;           
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;           
  LTDC_Init(&LTDC_InitStruct);
  
  /* Layer 1 Configuration */ 
  /* Windowing configuration */  
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = (90 + 43);
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (390 + 43 - 1); 
  LTDC_Layer_InitStruct.LTDC_VarticalStart = (15 + 12);
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (135 + 12 - 1);
  /* Pixel Format */           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB8888;
  /* Default Color */          
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;    
  /* Alpha Value */
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;            
  /* Blending Factors */    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
  /* Input Address */    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)&ARGB8888_300x120;
  /* Width */
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((300 * 4) + 3); 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (300 * 4);  
  /* Height */
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 120; 
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
  
  /* Layer 2 Configuration */
  /* Windowing configuration */  
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = (90 + 43);
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (390 + 43 - 1); 
  LTDC_Layer_InitStruct.LTDC_VarticalStart = (137 + 12);
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (257 + 12 - 1);
  /* Pixel Format */           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB4444;
 /* Blending Factors */    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;  
  /* Input Address */    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)&aBufferResult;
  /* Width */
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((300 * 2) + 3); 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (300 * 2);  
  /* Height */
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 120; 
  LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);
  
}    

/**
  * @brief GPIO configuration for LTDC.
  * @retval
  *  None
  */
static void LCD_AF_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOJ | \
                         RCC_AHB1Periph_GPIOK | RCC_AHB1Periph_GPIOC | \
                         RCC_AHB1Periph_GPIOA,  ENABLE);

/* GPIOs Configuration */
/*
 +------------------------+-----------------------+----------------------------+
 +                       LCD pins assignment                                   +
 +------------------------+-----------------------+----------------------------+
 |  LCD_TFT R0 <-> PI.15  |  LCD_TFT G0 <-> PJ.07 |  LCD_TFT B0 <-> PJ.12      |
 |  LCD_TFT R1 <-> PJ.00  |  LCD_TFT G1 <-> PJ.08 |  LCD_TFT B1 <-> PJ.13      |
 |  LCD_TFT R2 <-> PJ.01  |  LCD_TFT G2 <-> PJ.09 |  LCD_TFT B2 <-> PJ.14      |
 |  LCD_TFT R3 <-> PJ.02  |  LCD_TFT G3 <-> PJ.10 |  LCD_TFT B3 <-> PJ.15      |
 |  LCD_TFT R4 <-> PJ.03  |  LCD_TFT G4 <-> PJ.11 |  LCD_TFT B4 <-> PK.03      |
 |  LCD_TFT R5 <-> PJ.04  |  LCD_TFT G5 <-> PK.00 |  LCD_TFT B5 <-> PK.04      |
 |  LCD_TFT R6 <-> PJ.05  |  LCD_TFT G6 <-> PK.01 |  LCD_TFT B6 <-> PK.05      |
 |  LCD_TFT R7 <-> PJ.06  |  LCD_TFT G7 <-> PK.02 |  LCD_TFT B7 <-> PK.06      |
 -------------------------------------------------------------------------------
          |  LCD_TFT HSYNC <-> PI.12  | LCDTFT VSYNC <->  PI.13 |
          |  LCD_TFT CLK   <-> PI.14  | LCD_TFT DE   <->  PK.07 |
           -----------------------------------------------------
          |LCD_TFT backlight <-> PA.08| LCD_ENABLE <-> PC.06    |
           -----------------------------------------------------
*/

 /* GPIOI configuration */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource14, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource15, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOI, &GPIO_InitStruct);
  
 /* GPIOJ configuration */  
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource2, GPIO_AF_LTDC); 
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource8, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource14, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource15, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                             GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                             GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
                             GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  
  GPIO_Init(GPIOJ, &GPIO_InitStruct);

 /* GPIOI configuration */
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource2, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource7, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                             GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOK, &GPIO_InitStruct);
  
  /* GPIOC configuration */ 
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* GPIOA configuration */ 
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
     
  GPIO_Init(GPIOA, &GPIO_InitStruct);  
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
