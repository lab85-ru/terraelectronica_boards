#include "appdefs.h"

#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "usb_mem.h"

#include "usb_request_handlers.h"

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    usb_dev_Init,
    usb_dev_Reset,
    usb_dev_Status_In,
    usb_dev_Status_Out,
    usb_dev_Data_Setup,
    usb_dev_NoData_Setup,
    usb_dev_Get_Interface_Setting,
    usb_dev_GetDeviceDescriptor,
    usb_dev_GetConfigDescriptor,
    usb_dev_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    USB_DEV_GetConfiguration ,
    USB_DEV_SetConfiguration ,
    USB_DEV_GetInterface ,
    USB_DEV_SetInterface ,
    USB_DEV_GetStatus ,
    USB_DEV_ClearFeature ,
    USB_DEV_SetEndPointFeature ,
    USB_DEV_SetDeviceFeature ,
    USB_DEV_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  { (u8*)usb_dev_DeviceDescriptor, USB_DEV_DEVICE_DESC_SIZE };

ONE_DESCRIPTOR Config_Descriptor =
  { (u8*)usb_dev_ConfigDescriptor, USB_DEV_CONFIG_DESC_SIZE };

ONE_DESCRIPTOR String_Descriptor[5] =
  {
    {(u8*)usb_dev_StringLangID,    USB_DEV_STRING_LANGID_SIZE},
    {(u8*)usb_dev_StringVendor,    USB_DEV_STRING_VENDOR_SIZE},
    {(u8*)usb_dev_StringProduct,   USB_DEV_STRING_PRODUCT_SIZE},
    {(u8*)usb_dev_StringSerial,    USB_DEV_STRING_SERIAL_SIZE},
    {(u8*)usb_dev_StringInterface, USB_DEV_STRING_INTERFACE_SIZE}
  };
//---------------------------------------------------------------------------
void usb_dev_Init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/

  //GetSerialNum();

  pInformation->Current_Configuration = 0;

  /* Connect the device */
   PowerOn();

  /* USB interrupts initialization */
  /* clear pending interrupts */
  _SetISTR(0);
  wInterrupt_Mask = IMR_MSK;
  /* set interrupts mask */
  _SetCNTR(wInterrupt_Mask);



  bDeviceState = UNCONNECTED;
}
//---------------------------------------------------------------------------
void usb_dev_Reset(void)
{
  /* Set device as not configured */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = usb_dev_ConfigDescriptor[7];

  /* Set device with the default Interface*/
  pInformation->Current_Interface = 0;

  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_NAK);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);


  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_BULK);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, Device_Property.MaxPacketSize);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPRxStatus(ENDP1, EP_RX_DIS);

  /* Initialize Endpoint 2 */
  SetEPType(ENDP2, EP_BULK);
  SetEPRxAddr(ENDP2, ENDP2_RXADDR);
  SetEPRxCount(ENDP2, Device_Property.MaxPacketSize);
  SetEPRxStatus(ENDP2, EP_RX_VALID);
  SetEPTxStatus(ENDP2, EP_TX_DIS);




  /* Set this device to response on default address */
  SetDeviceAddress(0);

  bDeviceState = ATTACHED;
}
//---------------------------------------------------------------------------
void usb_dev_Status_In(void)
{
  NOP();
}
//---------------------------------------------------------------------------
void usb_dev_Status_Out(void)
{
  NOP();
}
//---------------------------------------------------------------------------
RESULT usb_dev_Data_Setup(u8 RequestNo)
{
  if ( Type_Recipient != VENDOR_REQUEST ) return USB_UNSUPPORT;
  if ( RequestNo >= vridRequestCount ) return USB_UNSUPPORT;
  pInformation->Ctrl_Info.CopyData = CopyRoutineTable[RequestNo] ;
  pInformation->Ctrl_Info.Usb_wOffset = 0;

  pInformation->USBwIndexs.w = ByteSwap(pInformation->USBwIndexs.w);
  pInformation->USBwValues.w = ByteSwap(pInformation->USBwValues.w);
  CopyRoutineTable[RequestNo](0);
  return USB_SUCCESS;
}
//---------------------------------------------------------------------------
RESULT usb_dev_NoData_Setup(u8 RequestNo)
{
  if ( Type_Recipient != VENDOR_REQUEST ) return USB_UNSUPPORT;
  if ( RequestNo >= vridRequestCount ) return USB_UNSUPPORT;

  // коррекция pInformation (перестановка байт)
  pInformation->USBwIndexs.w = ByteSwap(pInformation->USBwIndexs.w);
  pInformation->USBwValues.w = ByteSwap(pInformation->USBwValues.w);
  // вызов обработчика
  CopyRoutineTable[RequestNo](0);
  return USB_SUCCESS;
}
//---------------------------------------------------------------------------
RESULT usb_dev_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT; /* in this application we don't have AlternateSetting*/
  }
  else if (Interface > 0) /*in this application we have only 1 interfaces*/
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}
//---------------------------------------------------------------------------
u8 *usb_dev_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}
//---------------------------------------------------------------------------
u8 *usb_dev_GetConfigDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}
//---------------------------------------------------------------------------
u8 *usb_dev_GetStringDescriptor(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4) // число строковых дескрипторов в String_Descriptor[]
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void usb_dev_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_DEV_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : Mass_Storage_SetConfiguration
* Description    : Handle the SetConfiguration request.
* Input          : None.
* Output         : None.
* Return         : None. */
void USB_DEV_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;

    ClearDTOG_TX(ENDP1);
    ClearDTOG_RX(ENDP2);
  }
}

/*******************************************************************************
* Function Name  : Mass_Storage_ClearFeature
* Description    : Handle the ClearFeature request.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_DEV_ClearFeature(void)
{
  NOP();
}

