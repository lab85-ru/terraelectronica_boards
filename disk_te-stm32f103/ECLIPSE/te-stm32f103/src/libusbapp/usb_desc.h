#ifndef __USB_DESC_H
#define __USB_DESC_H

#include <stdint.h>

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define USB_DEV_DEVICE_DESC_SIZE                18
extern const uint8_t usb_dev_DeviceDescriptor[USB_DEV_DEVICE_DESC_SIZE];
//-----------------------------------------------------------------------------
#define USB_DEV_CONFIG_DESC_SIZE                32
extern const uint8_t usb_dev_ConfigDescriptor[USB_DEV_CONFIG_DESC_SIZE];
//-----------------------------------------------------------------------------
#define USB_DEV_STRING_LANGID_SIZE              4
extern const uint8_t usb_dev_StringLangID[USB_DEV_STRING_LANGID_SIZE];
//-----------------------------------------------------------------------------
#define USB_DEV_STRING_VENDOR_SIZE              50
extern const uint8_t usb_dev_StringVendor[USB_DEV_STRING_VENDOR_SIZE];
//-----------------------------------------------------------------------------
#define USB_DEV_STRING_PRODUCT_SIZE             52
extern const uint8_t usb_dev_StringProduct[USB_DEV_STRING_PRODUCT_SIZE];
//-----------------------------------------------------------------------------
#define USB_DEV_STRING_SERIAL_SIZE              10
extern const uint8_t    usb_dev_StringSerial[USB_DEV_STRING_SERIAL_SIZE];
//-----------------------------------------------------------------------------
#define USB_DEV_STRING_INTERFACE_SIZE           20
extern const uint8_t    usb_dev_StringInterface[USB_DEV_STRING_INTERFACE_SIZE];

#endif /* __USB_DESC_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
