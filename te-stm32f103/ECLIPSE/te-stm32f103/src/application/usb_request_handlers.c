#include "appdefs.h"

#include "usb_lib.h"

#include "hw_config.h"
#include "usb_request_handlers.h"

#include "sdio.h"
#include "adc.h"
#include "can.h"

#include "ff.h"
#include "sdcard.h"

#include <stdio.h>
#include <string.h>


//-----------------------------------------------------------------------
uint8_t* MemoryAccess(uint16_t length)
  {
    // фзический адрес обмена
    static uint8_t* addr = 0 ;
    if (length == 0)
      {
        // запись размера пакета
        pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w ;
        // запись физического адреса
        addr = (uint8_t*)(( pInformation->USBwIndexs.w  << 16 ) | pInformation->USBwValues.w ) ;
        return NULL;
      }
    else
        return addr + pInformation->Ctrl_Info.Usb_wOffset ;
  }
//-----------------------------------------------------------------------
uint8_t* GetLibVersion (uint16_t length)  // чтение версии FreeRTOS
  {
    // фзический адрес обмена
    static char lib_ver[512] = {0} ;
    char buff[128];
    sprintf(lib_ver , "Stm32 FWlib\tv%i.%i.%i\r\n" , __STM32F10X_STDPERIPH_VERSION_MAIN , __STM32F10X_STDPERIPH_VERSION_SUB1 , __STM32F10X_STDPERIPH_VERSION_SUB2 );
    sprintf(buff , "CMSIS\t\t\tv%i.%i\n\r" , __CM3_CMSIS_VERSION_MAIN , __CM3_CMSIS_VERSION_SUB ) ;
    strcat(lib_ver,buff);
    sprintf(buff , "Stm32 USB FS\tv%i.%i.%i\r\n" , __STM32F10X_USB_FS_VERSION_MAIN , __STM32F10X_USB_FS_VERSION_SUB1 , __STM32F10X_USB_FS_VERSION_SUB2 );
    strcat(lib_ver,buff);
    sprintf(buff , "Stm32 SDCard\tv%s\n\r" , __STM32F10X_SDCARD_LAYER_VESION_SRING );
    strcat(lib_ver,buff);
    sprintf(buff , "Chan FatFS\t%s\n\r" , FATFS_VERSION_STRING ) ;
    strcat(lib_ver,buff);


    if (length == 0)
      {
       // запись размера пакета
       pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w ; // lолжно быть 512
       // запись физического адреса
       return NULL;
      }
    else
       return (uint8_t*)lib_ver + pInformation->Ctrl_Info.Usb_wOffset ;
  }
//-----------------------------------------------------------------------
uint8_t* GetTestSDIO (uint16_t length)  //
  {

	// фзический адрес обмена
      static uint32_t sdio_test_result[4]  ;

      if (length == 0)
        {
          // запись размера пакета
          pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w ; // должно быть sizeof(sdio_test_result)
          SD_CardInfo* disk_sd_get_card_info ();
          SD_CardInfo* info = disk_sd_get_card_info ();
          sdio_test_result[0] = sdio_get_test_result() ;
          sdio_test_result[1] = info->CardBlockSize ;
          sdio_test_result[2] = info->CardCapacity ;
          sdio_test_result[3] = info->CardType ;
          return NULL;
        }
      else
          return  ((uint8_t*)sdio_test_result) + pInformation->Ctrl_Info.Usb_wOffset ;
  }
//-----------------------------------------------------------------------
uint8_t* GetTestADC (uint16_t length)  //
  {
      if (length == 0)
        {
          // запись размера пакета
          pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w ; // должно быть 8
          return NULL;
        }
      else
          return  ((uint8_t*)adc_get_test_result()) + pInformation->Ctrl_Info.Usb_wOffset ;
  }
//-----------------------------------------------------------------------
uint8_t* GetTestCAN (uint16_t length)  //
  {
      // фзический адрес обмена
	  static uint32_t tmp ;
      if (length == 0)
        {
          // запись размера пакета
          pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w ; // должно быть sizeof(sdio_test_result)
          tmp = can_get_test_result();
          return NULL;
        }
      else
          return  ((uint8_t*)&tmp) + pInformation->Ctrl_Info.Usb_wOffset ;
  }
//----------------------------------------------------------------------
uint8_t* GetTestCpuClock (uint16_t length)  //
  {
      // фзический адрес обмена
	  static uint32_t cpu_clock ;
	  if (length == 0)
        {
          // запись размера пакета
          pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w ; // должно быть sizeof(uint32_t)
          cpu_clock = GetCpuClock();
          return NULL;
        }
      else
          return  ((uint8_t*)&cpu_clock) + pInformation->Ctrl_Info.Usb_wOffset ;
  }
//----------------------------------------------------------------------
REQUEST_ROUTINE CopyRoutineTable[vridRequestCount] =
   {
     MemoryAccess                ,  // доступ к физической памяти
     //------------------------------------------------------------------
     GetLibVersion               ,  // чтение версии компонентов
     //------------------------------------------------------------------
     GetTestSDIO                 ,  // чтение результата тестирования SDIO
     GetTestADC                  ,  // чтение результата тестирования ADC
     GetTestCAN                  ,  // чтение результата тестирования CAN
     //------------------------------------------------------------------
     GetTestCpuClock             ,  // чтение значения тактовой частоты процессора
 } ;
//-----------------------------------------------------------------------
