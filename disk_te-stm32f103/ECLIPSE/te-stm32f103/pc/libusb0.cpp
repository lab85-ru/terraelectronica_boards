//---------------------------------------------------------------------------
#include "libusb0.h"


TLibUsb0::TLibUsb0()
{
  usb_init();
  handle = NULL ;
  Configuration = -1 ;
  AltInterface =- 1 ;
  CtrlSetup = new TCtrlSetup  ;
}
//---------------------------------------------------------------------------
TLibUsb0::~TLibUsb0()
{
  delete CtrlSetup ;
}
//---------------------------------------------------------------------------
void TLibUsb0::ScanUSB(void)
{
  BusCount = usb_find_busses();    
  DeviceCount = usb_find_devices(); 
  USB_bus = usb_get_busses();       
}
//---------------------------------------------------------------------------
bool TLibUsb0::IsOpened()
{
   return (bool) handle ;
}
//---------------------------------------------------------------------------
void TLibUsb0::Open()
{
  struct usb_device *dev;
  ScanUSB() ;
  handle = NULL ;
  for(USB_bus ; USB_bus ; USB_bus = USB_bus->next)
    {
      dev = USB_bus->devices ;
      for(dev ; dev ; dev = dev->next)
        {
          if(  dev->descriptor.idVendor == VID  &&
               dev->descriptor.idProduct == PID)
            {
              handle = usb_open(dev);
              if (handle)
               {
                 DescriptorStrings.clear();
                 int StringIndex = 0 ;
                 while (1)
                  {
                   string str ;
                   str.resize(512,0);
                   if ( usb_get_string_simple(handle, StringIndex++ , (char*)str.c_str() , 512 )<0 )
                    {
                     break ;
                    }
                   DescriptorStrings.push_back(str) ;
                  }
               }
            }
        }
    }
 if ( !handle ) throw ( "Device is not plugged" ) ;
}
//--------------------------------------------------------------------------
void  TLibUsb0::Close()
{
  usb_close(handle) ;
  handle = NULL ;
  DescriptorStrings.clear();
}
//-------------------------------------------------------------------------
void TLibUsb0::SetConfiguration(int configuration)
{
 if ( usb_set_configuration (handle , configuration) < 0 )
  {
    Configuration = -1 ;
    throw  StrError() ;
  }
 Configuration = configuration ;
}
//-------------------------------------------------------------------------

void TLibUsb0::Claim(int interface)
{
 if ( usb_claim_interface (handle , interface) < 0 )
  {
    throw StrError() ;
  }
}
//-------------------------------------------------------------------------
void TLibUsb0::Release(int interface)
{
 if ( usb_release_interface( handle , interface) < 0 )
  {
    throw  StrError() ;
  }
}
//-------------------------------------------------------------------------
void TLibUsb0::SetAltInterface(int altinterface)
{
  if ( usb_set_altinterface( handle , altinterface) < 0 )
   {
     AltInterface =- 1 ;
     throw  StrError() ;
   }
  AltInterface = altinterface ;
}
//-------------------------------------------------------------------------
void TLibUsb0::ResetEndPoint ( unsigned int ep )
{
  if ( usb_resetep( handle , ep) < 0 )
  {
    throw  StrError() ;
  }
}
//-------------------------------------------------------------------------
void TLibUsb0::ClearHaltEndPoint ( unsigned int ep )
{
  if ( usb_clear_halt( handle , ep) < 0 )
  {
    throw  StrError() ;
  }
}
//-------------------------------------------------------------------------
void TLibUsb0::Reset ()
{
  if ( usb_reset( handle) < 0 )
  {
    throw  StrError() ;
  }
  handle = NULL ;
}
//------------------------------------------------------------------------
int TLibUsb0::DescriptorString(int index, int langid , char* buff , int length)
{
  return usb_get_string(handle, index, langid, buff, length);
}
//-------------------------------------------------------------------------
int TLibUsb0::ControlMsg () // команда управдения
{
  FBytes =  usb_control_msg(handle,   CtrlSetup->setup.bRequestType ,
                                      CtrlSetup->setup.bRequest,
                                      CtrlSetup->setup.wValue,
                                      CtrlSetup->setup.wIndex,
                               (char*)CtrlSetup->setup_data,
                                      CtrlSetup->setup.wLength,
                                      Timeout);
  if ( FBytes < 0 ) throw  StrError() ;
  return FBytes ;
}
//------------------------------------------------------------------------
int TLibUsb0::BulkWrite(char *bytes, int size) //запись в bulk endpoint
{
  FBytes = usb_bulk_write(handle,BulkOutEP,bytes,size,Timeout) ;
  if ( FBytes < 0 ) throw  StrError() ;
  return FBytes ;
}
//------------------------------------------------------------------------
int TLibUsb0::BulkRead (char *bytes, int size)  //чтение из bulk endpoint
{
  FBytes = 0  ;
  while ( FBytes < size )
   {
      bytes += FBytes ;
      FBytes += usb_bulk_read(handle,BulkInEP,bytes,size,Timeout) ;
      if ( FBytes < 0 ) throw  StrError() ;

   }
  return FBytes ;
}
//------------------------------------------------------------------------
int TLibUsb0::InterruptWrite(char *bytes, int size) // запись в interrupt endpoint
{
  FBytes = usb_interrupt_write(handle,IntOutEP,bytes,size,Timeout) ;
  if ( FBytes < 0 ) throw  StrError() ;
  return FBytes ;
}
//------------------------------------------------------------------------
int TLibUsb0::InterruptRead (char *bytes, int size)  // чтение из interrupt endpoint
{
  FBytes = usb_interrupt_read(handle,IntInEP,bytes,size,Timeout) ;
  if ( FBytes < 0 ) throw  StrError() ;
  return FBytes ;
}
//------------------------------------------------------------------------
char*  TLibUsb0::StrError()  // результат крайнего вызова API драйвера
{
  return usb_strerror();
}
