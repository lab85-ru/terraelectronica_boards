/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
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
#include "bsp.h"
#include "timers.h "
#include "test_program.h"
#include <stddef.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Background_Task_PRIO    ( tskIDLE_PRIORITY  + 10 )
#define Background_Task_STACK   ( 512 )

#define Demo_Task_PRIO          ( tskIDLE_PRIORITY  + 9 )
#define Demo_Task_STACK         ( 3048 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
xTaskHandle                   Task_Handle;
xTaskHandle                   Demo_Handle;
xTimerHandle                  TouchScreenTimer;

uint32_t demo_mode = 1;

extern WM_HWIN  ALARM_hWin;

FATFS microSD_fatfs;
uint8_t microSD_OK = 0;

char* pDirectoryFiles[MAX_BMP_FILES];

uint8_t  ubNumberOfFiles = 0;
uint32_t uwBmplen = 0;

/* Private function prototypes -----------------------------------------------*/
static void Background_Task(void * pvParameters);
static void Demo_Task(void * pvParameters);
static void vTimerCallback( xTimerHandle pxTimer );
extern void DEMO_Starup(void);
extern void DEMO_MainMenu(void);
extern void ALARM_BackgroundProcess (void);
static void SDCard_Config(void);
static void FileSystem_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 
int main(void)
{ 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* Setup SysTick Timer for 1 msec interrupts.*/
  if (SysTick_Config(SystemCoreClock / 1800))
  { 
    /* Capture error */ 
    while (1);
  }
  
  /* Create background task */
  xTaskCreate(Background_Task,
              (signed char const*)"BK_GND",
              Background_Task_STACK,
              NULL,
              Background_Task_PRIO,
              &Task_Handle);

  /* Start the FreeRTOS scheduler */
  vTaskStartScheduler();
}


/**
  * @brief  Background task
  * @param  pvParameters not used
  * @retval None
  */
static void Background_Task(void * pvParameters)
{
  uint32_t ticks = 0;
    
  /* Initialize the BSP layer */
  LowLevel_Init();
  
  /* SD Card Configuration */
  SDCard_Config();
    
  /* Configure the File System */
  FileSystem_Config();    

  /* Init the STemWin GUI Library */
  GUI_Init();
    
  /* Create demo task */
  xTaskCreate(Demo_Task,
              (signed char const*)"GUI_DEMO",
              Demo_Task_STACK,
              NULL,
              Demo_Task_PRIO,
              &Demo_Handle);
  
  /* Launch Touchscreen Timer */
  TouchScreenTimer = xTimerCreate ("Timer", 50, pdTRUE, ( void * ) 1, vTimerCallback );
  
  if( TouchScreenTimer != NULL )
  {
    if( xTimerStart( TouchScreenTimer, 0 ) != pdPASS )
    {
      /* The timer could not be set into the Active state. */
    }
  }
    
  /* Run the background task */
  while (1)
  {
    if(ticks++ > 10)
    {
      ticks = 0;
      /* toggle LED3 each 100ms */
      STM_EVAL_LEDToggle(LED);

      if (SD_Detect() == SD_PRESENT)
      {
        if (microSD_OK != 1)
        {
          SDCard_Config();
          FileSystem_Config();    
        }
      }
      else (microSD_OK = 0);
        

    }
    
    /* Handle ALARM background process */
    ALARM_BackgroundProcess();
    /* This task is handled periodically, each 10 mS */
    vTaskDelay(10);
  }
}


/**
  * @brief  Demonstration task
  * @param  pvParameters not used
  * @retval None
  */
static void Demo_Task(void * pvParameters)
{  
  /* Change Skin */
  PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
  RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
  SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
  SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
  SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
  BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
  
  /* Setup layer configuration during startup */
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_SelectLayer(1);
  GUI_Clear();
  GUI_SetBkColor(GUI_TRANSPARENT); 
  GUI_SelectLayer(0);

  /* Run the Startup frame */
//  DEMO_Starup();
  
  /* Show the main menu */
  DEMO_MainMenu();
}


/**
  * @brief  Timer callback
  * @param  pxTimer 
  * @retval None
  */
static void vTimerCallback( xTimerHandle pxTimer )
{
   BSP_Pointer_Update();
}

/**
  * @brief  Error callback function
  * @param  None
  * @retval None
  */
void vApplicationMallocFailedHook( void )
{
  while (1)
  {}
}

static void FileSystem_Config(void)
{
  uint32_t counter = 0;
  
  /* Check the mounted device */
  if ( f_mount(0, &microSD_fatfs ) != FR_OK )
  {
    microSD_OK =0;
  }  
      
  /* Initialize the Directory Files pointers (heap) */
  for (counter = 0; counter < MAX_BMP_FILES; counter++)
  {
    pDirectoryFiles[counter] = malloc(11); 
  }
}

/**
  * @brief  SD Card Configuration.
  * @param  None
  * @retval None
  */
static void SDCard_Config(void)
{
  uint32_t error = 0;
  uint32_t counter = 0x100;
  
  /* SDCard initialisation */
  SD_Init();
  
  if (SD_Detect() == SD_PRESENT)
  {
    microSD_OK = 1;
  }
  
  /* FAT Initialization */
  do
  {
    /* SDCARD Initialisation */
    error = Storage_Init();                                                    
  }
  while ((error != 0) && (counter-- != 0));

  /* SD Card not formatted */
  if (counter == 0)
  {
    microSD_OK = 0;
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
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
