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
  //������������� ��������� ���������
  SystemStartup();

    // ��������� ����� ������  CAN
  usb_can_mode = ucmCan ;
  can_activity = FALSE  ;

  // ������������� ������� ������������
  DebugLedInit();

  // ���� uSD ����������� � ������ - ����������� ��� ����� CAN ������
  // ����� ����������� ������� ��� ����� CAN ������
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

      // ��������� ����� ������  USB
      usb_can_mode = ucmUsb ;
      // �������������  � ������ ������ USB
      UsbStartup();
    }

  // ��������� �������� �� ���� USB ���������� �� ����������
  while(1)
   {
    // �������� ������������� � �������� ~200 ��
    Delay(200);
    DebugLedsToggle();
   }
  return 1 ;
}

