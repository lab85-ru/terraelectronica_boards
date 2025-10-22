#include "appdefs.h"
#include "sys_init.h"
#include "uip_handler.h"
#include "can_test.h"

#if (USE_FREERTOS==1)
//-----------------------------------------------------------------
xSemaphoreHandle uip_semaphore = (xSemaphoreHandle)-1 ;
xTaskHandle uip_recv_task_handle ;
void uip_recv_task( void *pvParameters )
{
	vSemaphoreCreateBinary( uip_semaphore );
	if ( !uip_semaphore ) vTaskSuspend( NULL ) ;
	while (1)
	   {
		if ( xSemaphoreTake( uip_semaphore, ( portTickType ) 0 ) == pdTRUE )
		  {
		    uip_handler_recv() ;
		    xSemaphoreGive( uip_semaphore ) ;
		    vTaskSuspend( NULL ) ;
		  }
	   }
}
//-----------------------------------------------------------------
xTaskHandle uip_periodic_task_handle ;
void uip_periodic_task( void *pvParameters )
{
	while ( uip_semaphore == (xSemaphoreHandle)-1 ) ;
	if ( !uip_semaphore ) vTaskSuspend( NULL ) ;
	while(1)
	  {
		if ( xSemaphoreTake( uip_semaphore, ( portTickType ) 0 ) == pdTRUE )
		{
		  uip_handler_periodic();
		  xSemaphoreGive( uip_semaphore ) ;
		}
		vTaskDelay(5) ;
	  }
}
//-----------------------------------------------------------------
uint32_t uip_task_recv_stack_free = 0 ;
uint32_t uip_task_periodic_stack_free = 0 ;
void vApplicationIdleHook()
{
	uip_task_recv_stack_free = uxTaskGetStackHighWaterMark( uip_recv_task_handle) ;
	uip_task_periodic_stack_free = uxTaskGetStackHighWaterMark( uip_periodic_task_handle) ;
}
//-----------------------------------------------------------------
void vApplicationTickHook()
{
}
//----------------------------------------------------------------
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
  while(1)
  {
	  NOP();
	  NOP();
  }
}
#else
//---------------------------------------------------------------------------------
  // флаги запросов обработки обработки по прерываниям
  volatile uint32_t uip_irqs ;
#endif

//-----------------------------------------------------------------
int main(void)
{
  // инициализация системы
  sys_init();

  // инициализация uIP и запуск сетевых служб
  uip_startup();

  #if (USE_FREERTOS==1)
    // вариант приложения с использованием FreeRTOS
    xTaskCreate( uip_periodic_task, ( signed char * ) "uip_pereodic", 416, ( void * ) NULL, 1, &uip_periodic_task_handle );
    xTaskCreate( uip_recv_task,     ( signed char * ) "uip_recv"    , 416, ( void * ) NULL, 1, &uip_recv_task_handle );

    vSetupHighFrequencyTimer();
    vTaskStartScheduler();

  #else
  // простой вариант приложения
    while(1)
       {
	    if (uip_irqs & uitRecv)
	     {
	  	  // сброс флага запроса
	  	   uip_irqs &= ~uitRecv ;
		   uip_handler_recv() ;
	     }
	    if (uip_irqs & uiPeriodic)
	     {
           // сброс флага запроса
           uip_irqs &= ~uiPeriodic ;
           uip_handler_periodic();
	     }
        // основной цикл приложения
        //......
       }
  #endif
}



