/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : usb_des.h
 *    Description : Define standard USB descriptors module
 *
 *    History :
 *    1. Date        : August 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 57747 $
 **************************************************************************/
#ifndef __USB_DESC_H
#define __USB_DESC_H

#define UsbConfigPower_mA(mA)   ((mA)/2)
#define UsbEpOut(addr)          ((addr) | 0x00)
#define UsbEpIn(addr)           ((addr) | 0x80)

#define UsbDescLengthOffset     0
#define UsbDescDescriptorOffset 1

typedef enum _UsbDescriptorType_t
{
  UsbDescriptorDevice = 1, UsbDescriptorConfiguration, UsbDescriptorString,
  UsbDescriptorInterface, UsbDescriptorEp, UsbDescriptorDeviceQualifier,
  UsbDescriptorOtherSeedConfiguration, UsbDescriptorInterfacePower
} UsbDescriptorType_t;

typedef enum _UsbStringsIndex_t
{
  iLanguagesStr = 0, iManufacturerStr, iProductStr, iSerialNumberStr,
  iConfigurationStr,
  iInterface,
} UsbStringsIndex_t;

typedef enum _UsbDeviceClassses_t
{
/* USB Device Classes */
  UsbDeviceClassReserved = 0, UsbDeviceClassAudio,
  UsbDeviceClassCommunications,UsbDeviceClassHumanInterface,
  UsbDeviceClassMonitor,UsbDeviceClassPhysicalInterface,
  UsbDeviceClassPower,UsbDeviceClassPrinter,
  UsbDeviceClassStorage,UsbDeviceClassHub,
  UsbDeviceClassVendorSpecific = 0xFF
} UsbDeviceClassses_t;

typedef union _CnfgArtr_t
{
  uint8_t Data;
  struct
  {
    uint8_t Reserved      : 5;
    uint8_t RemoteWakeup  : 1;
    uint8_t SelfPowered   : 1;
    uint8_t MustBeOne     : 1;
  };
} CnfgArtr_t, * pCnfgArtr_t;

#define USBPowerMask          0x40
#define USBRemoteWakeUpMask   0x20

typedef enum _UsbConfigurationAttr_t
{
  UsbRemoteWakeup = 0x20, UsbSelfPowered = 0x40,  UsbConfigurationCommmonAttr = 0x80
} UsbConfigurationAttr_t;

typedef enum _UsbEpTransferType_t
{
  UsbEpTransferControl = 0, UsbEpTransferIsochronous,
  UsbEpTransferBulk,UsbEpTransferInterrupt
}UsbEpTransferType_t;

typedef enum _UsbEpSynchronizationType_t
{
  UsbEpSynchNoSynchronization = 0, UsbEpSynchAsynchronous = 1<<2,
  UsbEpSynchAdaptive = 2<<2, UsbEpSynchSynchronous = 3<<2
}UsbEpSynchronizationType_t;

typedef enum _UsbEpUsageType_t
{
  UsbEpUsageData = 0, UsbEpUsageFeedback = 1<<4,
  UsbEpUsageImplicit = 2<<4, UsbEpUsageReserved = 3<<4
}UsbEpUsageType_t;

typedef union _EpAttr_t
{
  uint8_t Data;
  struct
  {
    uint8_t TransferType        : 2;
    uint8_t SynchronizationType : 2;
    uint8_t UsageType           : 2;
  };
} EpAttr_t, * pEpAttr_t;

#pragma pack(1)
typedef struct _UsbStandardDeviceDescriptorStr_t
{
  uint8_t       bLength;
  uint8_t       bDescriptorType;
  uint16_t      bcdUSB;
  uint8_t       bDeviceClass;
  uint8_t       bDeviceSubClass;
  uint8_t       bDeviceProtocol;
  uint8_t       bMaxPacketSize0;
  uint16_t      idVendor;
  uint16_t      idProduct;
  uint16_t      bcdDevice;
  uint8_t       iManufacturer;
  uint8_t       iProduct;
  uint8_t       iSerialNumber;
  uint8_t       bNumConfigurations;
} UsbStandardDeviceDescriptorStr_t, * pUsbStandardDeviceDescriptorStr_t;

typedef struct _UsbStandardConfigurationDescriptor_t
{
  uint8_t       bLength;
  uint8_t       bDescriptorType;
  uint16_t      wTotalLength;
  uint8_t       bNumInterfaces;
  uint8_t       bConfigurationValue;
  uint8_t       iConfiguration;
  CnfgArtr_t  bmAttributes;
  uint8_t       bMaxPower;
} UsbStandardConfigurationDescriptor_t, * pUsbStandardConfigurationDescriptor_t;

typedef struct _UsbStandardInterfaceDescriptor_t
{
  uint8_t       bLength;
  uint8_t       bDescriptorType;
  uint8_t       bInterfaceNumber;
  uint8_t       bAlternateSetting;
  uint8_t       bNumEndpoints;
  uint8_t       bInterfaceClass;
  uint8_t       bInterfaceSubClass;
  uint8_t       bInterfaceProtocol;
  uint8_t       iInterface;
} UsbStandardInterfaceDescriptor_t, * pUsbStandardInterfaceDescriptor_t;

typedef struct _UsbStandardEpDescriptor_t
{
  uint8_t       bLength;
  uint8_t       bDescriptorType;
  uint8_t       bEndpointAddress;
  EpAttr_t    bmAttributes;
  uint16_t      wMaxPacketSize;
  uint8_t       bInterval;
} UsbStandardEpDescriptor_t, * pUsbStandardEpDescriptor_t;

#if USB_HIGH_SPEED > 0
typedef struct _UsbStandardDeviceQualifierDescriptor_t
{
  uint8_t       bLength;
  uint8_t       bDescriptorType;
  uint16_t      bcdUSB;
  uint8_t       bDeviceClass;
  uint8_t       bDeviceSubClass;
  uint8_t       bDeviceProtocol;
  uint8_t       bMaxPacketSize0;
  uint8_t       bNumConfigurations;
  uint8_t       bReserve;
} UsbStandardDeviceQualifierDescriptor_t, *pUsbStandardDeviceQualifierDescriptor_t;
#endif // USB_HIGH_SPEED > 0
#pragma pack()

#endif //__USB_DESC_H
