//---------------------------------------------------------------------------
#ifndef __GNUG__
       #include <vcl.h>
#endif


#include <iostream>
#include <stdint.h>

#include <conio.h>
#include <ctype.h>

#pragma hdrstop

#include "libusb0.h"

#include "../src/include/appdefs.h"

const char*  sd_messge[]=
  {
    "SD_UNKNOWN",
    /* SDIO specific error defines */
    "SD_CMD_CRC_FAIL",
    "SD_DATA_CRC_FAIL",
    "SD_CMD_RSP_TIMEOUT",
    "SD_DATA_TIMEOUT",
    "SD_TX_UNDERRUN",
    "SD_RX_OVERRUN",
    "SD_START_BIT_ERR",
    "SD_CMD_OUT_OF_RANGE",
    "SD_ADDR_MISALIGNED",
    "SD_BLOCK_LEN_ERR",
    "SD_ERASE_SEQ_ERR",
    "SD_BAD_ERASE_PARAM",
    "SD_WRITE_PROT_VIOLATION",
    "SD_LOCK_UNLOCK_FAILED",
    "SD_COM_CRC_FAILED",
    "SD_ILLEGAL_CMD",
    "SD_CARD_ECC_FAILED",
    "SD_CC_ERROR",
    "SD_GENERAL_UNKNOWN_ERROR",
    "SD_STREAM_READ_UNDERRUN",
    "SD_STREAM_WRITE_OVERRUN",
    "SD_CID_CSD_OVERWRITE",
    "SD_WP_ERASE_SKIP",
    "SD_CARD_ECC_DISABLED",
    "SD_ERASE_RESET",
    "SD_AKE_SEQ_ERROR",
    "SD_INVALID_VOLTRANGE",
    "SD_ADDR_OUT_OF_RANGE",
    "SD_SWITCH_ERROR",
    "SD_SDIO_DISABLED",
    "SD_SDIO_FUNCTION_BUSY",
    "SD_SDIO_FUNCTION_FAILED",
    "SD_SDIO_UNKNOWN_FUNCTION",
    "D_INTERNAL_ERROR",
    "SD_NOT_CONFIGURED",
    "SD_REQUEST_PENDING",
    "SD_REQUEST_NOT_APPLICABLE",
    "SD_INVALID_PARAMETER",
    "SD_UNSUPPORTED_FEATURE",
    "SD_UNSUPPORTED_HW",
    "SD_ERROR",
    "SD_OK"
  } ;

//---------------------------------------------------------------------------

#pragma argsused

using namespace std ;


int main(int argc, char* argv[])
{
  TLibUsb0* LibUsb0 = new TLibUsb0();
  LibUsb0->VID = 0xFF02 ;
  LibUsb0->PID = 0x0005 ;

  try
  {

  LibUsb0->Open() ;

  cout << endl << "------USB dev descriptors---------" << endl ;
  vector<string>::iterator str = LibUsb0->DescriptorStrings.begin() ;
  
  if ( str != LibUsb0->DescriptorStrings.end() ) 
   {
     cout << (*str++).c_str() << endl ;
   } 

  LibUsb0->SetConfiguration(1) ;
  LibUsb0->Timeout = 60000 ;
  LibUsb0->Claim(0);

  // чтение версий компонент firmware
  char versions[512]  ;
  LibUsb0->CtrlSetup->setup.bRequest = vridLibVersion ;
  LibUsb0->CtrlSetup->setup.bRequestType = USB_TYPE_VENDOR | USB_ENDPOINT_IN ;
  LibUsb0->CtrlSetup->setup.wValue = 0 ;
  LibUsb0->CtrlSetup->setup.wIndex = 0 ;
  LibUsb0->CtrlSetup->setup.wLength = sizeof (versions);
  LibUsb0->CtrlSetup->setup_data = versions ;
  LibUsb0->ControlMsg();
  cout << endl << "------- firmware lib versions ---" << endl ;
  cout << versions << endl  ;

   typedef struct
   {
      uint32_t passed ;
      uint32_t CardBlockSize ;
      uint32_t CardCapacity ;
      uint32_t CardType ;
    } TSDIOTestResult ;

   TSDIOTestResult  sdio_test_result ;

   LibUsb0->CtrlSetup->setup.bRequest = vridGetTestSDIO ;
   LibUsb0->CtrlSetup->setup.bRequestType = USB_TYPE_VENDOR | USB_ENDPOINT_IN ;
   LibUsb0->CtrlSetup->setup.wValue = 0 ;
   LibUsb0->CtrlSetup->setup.wIndex = 0 ;
   LibUsb0->CtrlSetup->setup.wLength = sizeof (sdio_test_result);
   LibUsb0->CtrlSetup->setup_data = &sdio_test_result ;
   LibUsb0->ControlMsg();
   cout << endl << "---------  SDIO Test  ------------" << endl ;
   cout << "test result      : " << ((sdio_test_result.passed == 1) ? "PASSED" : "FAILED") << endl ;
   cout << "cad block size   : " << sdio_test_result.CardBlockSize << " bytes"<< endl  ;
   cout << "card capasity    : " << sdio_test_result.CardCapacity / (1024*1024.0) << " MB" << endl  ;
   cout << "card type        : " << sdio_test_result.CardType << endl  ;

   float adc_test_result[2]  ;
   LibUsb0->CtrlSetup->setup.bRequest = vridGetTestADC ;
   LibUsb0->CtrlSetup->setup.bRequestType = USB_TYPE_VENDOR | USB_ENDPOINT_IN ;
   LibUsb0->CtrlSetup->setup.wValue = 0 ;
   LibUsb0->CtrlSetup->setup.wIndex = 0 ;
   LibUsb0->CtrlSetup->setup.wLength = sizeof (adc_test_result);
   LibUsb0->CtrlSetup->setup_data = adc_test_result ;
   LibUsb0->ControlMsg();
   cout << endl << "---------  ADC Test  ------------" << endl ;
   cout << "test result      : " << ((adc_test_result[0] >= 1.0) ? "PASSED" : "FAILED") << endl ;
   cout << "Vdd/3 voltage    : " << adc_test_result[1] << " V"<< endl  ;

   uint32_t can_test_result  ;
   LibUsb0->CtrlSetup->setup.bRequest = vridGetTestCAN ;
   LibUsb0->CtrlSetup->setup.bRequestType = USB_TYPE_VENDOR | USB_ENDPOINT_IN ;
   LibUsb0->CtrlSetup->setup.wValue = 0 ;
   LibUsb0->CtrlSetup->setup.wIndex = 0 ;
   LibUsb0->CtrlSetup->setup.wLength = sizeof (can_test_result);
   LibUsb0->CtrlSetup->setup_data = &can_test_result ;
   LibUsb0->ControlMsg();
   cout << endl << "---------  CAN Test  ------------" << endl ;
   cout << "test result      : " << ((can_test_result == 1.0) ? "PASSED" : "FAILED") << endl ;


   cout << endl << "---------------------------------" << endl ;
   if ( sdio_test_result.passed && (uint32_t)adc_test_result[0] && can_test_result )
     cout << "test programm PASSED, device is OK" << endl ;
   else
     cout << "test programm FAILED, see log above .." << endl ;

   LibUsb0->Close() ;

   }
   catch ( const char* mess )
   {
     cout << mess << endl ;
   }
   
   delete LibUsb0 ;
   
   cout << "press any key to exit" << endl ;
   getch();
}
//---------------------------------------------------------------------------
 
