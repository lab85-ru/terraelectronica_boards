#include "appdefs.h"

#include "usb_startup.h"
#include "hw_config.h"

#include "adc.h"
#include "can.h"
#include "sdio.h"
#include "debug_led.h"

volatile TUsbModeMode usb_can_mode ;
volatile uint32_t can_activity  ;
//--------------------------------------------------------------------------
int main()
{
  //инициализация системной периферии
  SystemStartup();

    // установка флага режима  CAN
  usb_can_mode = ucmCan ;
  can_activity = FALSE  ;

  // инициализация выводов свветодиодов
  DebugLedInit();

  // если uSD отсутствует в сокете - принимается что плата CAN сервер
  // иначе принимается решение что плата CAN клиент
  bool disk_sd_present();
  if ( !disk_sd_present() )
	  can_test( cgtServer ) ;
  else
    {
	  can_test(cgtClient);
	  sdio_test();
      adc_test();
	  DebugLedInitIndication((can_get_test_result() || can_activity) ,
			                  adc_get_test_result()->result ,
			                  sdio_get_test_result()) ;

      // установка флага режима  USB
      usb_can_mode = ucmUsb ;
      // инициализация  и запуск модуля USB
      UsbStartup();
    }

  // обработка запросов по шине USB происходит по прерыванию
  while(1)
   {
    // моргание светодиодамим с периодом ~200 мс
    Delay(200);
    DebugLedsToggle();
   }
  return 1 ;
}

