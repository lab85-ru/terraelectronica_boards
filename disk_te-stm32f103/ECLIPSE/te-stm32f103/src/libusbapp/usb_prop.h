#ifndef __usb_prop_H
#define __usb_prop_H



#define USB_DEV_GetConfiguration          NOP_Process
//#define USB_DEV_SetConfiguration     NOP_Process
#define USB_DEV_GetInterface              NOP_Process
#define USB_DEV_SetInterface              NOP_Process
#define USB_DEV_GetStatus                 NOP_Process
//#define USB_DEV_ClearFeature              NOP_Process
#define USB_DEV_SetEndPointFeature        NOP_Process
#define USB_DEV_SetDeviceFeature          NOP_Process
//#define USB_DEV_SetDeviceAddress          NOP_Process



/* Exported functions ------------------------------------------------------- */
void usb_dev_Init(void);
void usb_dev_Reset(void);
void usb_dev_SetConfiguration(void);
void usb_dev_SetDeviceAddress (void);
void usb_dev_Status_In (void);
void usb_dev_Status_Out (void);
RESULT usb_dev_Data_Setup(u8);
RESULT usb_dev_NoData_Setup(u8);
RESULT usb_dev_Get_Interface_Setting(u8 Interface, u8 AlternateSetting);
u8 *usb_dev_GetDeviceDescriptor(u16 );
u8 *usb_dev_GetConfigDescriptor(u16);
u8 *usb_dev_GetStringDescriptor(u16);

void USB_DEV_SetConfiguration(void);
void USB_DEV_ClearFeature(void);
void USB_DEV_SetDeviceAddress (void);

#endif /* __usb_prop_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

