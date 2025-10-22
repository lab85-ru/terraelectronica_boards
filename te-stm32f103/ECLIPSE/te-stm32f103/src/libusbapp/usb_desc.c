
#include "usb_lib.h"
#include "usb_desc.h"
/* USB Standard Device Descriptor */

#define LE_WORD(x)              ((x)&0xFF),((x)>>8)

const uint8_t usb_dev_DeviceDescriptor[USB_DEV_DEVICE_DESC_SIZE] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x00,
    0x02,   /* bcdUSB = 2.00 */
    0xFF,   /* bDeviceClass: vendor */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 */
    LE_WORD(0xFF02),   /* idVendor = 0x0483  */
    LE_WORD(0x0005),   /* idProduct = 0x7540 */
    LE_WORD(0x0002),   /* bcdDevice = 2.00   */
    1,      /* Index of string descriptor describing manufacturer */
    2,      /* Index of string descriptor describing product */
    3,      /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };

const uint8_t usb_dev_ConfigDescriptor[USB_DEV_CONFIG_DESC_SIZE] =
  {
      0x09,   /* bLength: Configuation Descriptor size */
      USB_CONFIGURATION_DESCRIPTOR_TYPE,   /* bDescriptorType: Configuration */
      USB_DEV_CONFIG_DESC_SIZE,
      0x00,
      0x01,   /* bNumInterfaces: 1 interface */
      0x01,   /* bConfigurationValue: */
      /*      Configuration value */
      0x00,   /* iConfiguration: */
      /*      Index of string descriptor */
      /*      describing the configuration */
      0xC0,   /* bmAttributes: */
      /*      bus powered */
      0x32,   /* MaxPower 100 mA */

      /******************** Descriptor interface ********************/
      /* 09 */
      0x09,   /* bLength: Interface Descriptor size */
      USB_INTERFACE_DESCRIPTOR_TYPE,   /* bDescriptorType: */
      /*      Interface descriptor type */
      0x00,   /* bInterfaceNumber: Number of Interface */
      0x00,   /* bAlternateSetting: Alternate setting */
      0x02,   /* bNumEndpoints*/
      0xFF,   /* bInterfaceClass: vendor Class */
      0x00,   /* bInterfaceSubClass : SCSI transparent*/
      0x00,   /* nInterfaceProtocol */
      0x00,          /* iInterface: */

      // Bulk-IN endpoint , total 18 */
      0x07,   /*Endpoint descriptor length = 7*/
      0x05,   /*Endpoint descriptor type */
      0x81,   /*Endpoint address (IN, address 1) */
      0x02,   /*Bulk endpoint type */
      0x40,   /*Maximum packet size (64 bytes) */
      0x00,
      0x00,   /*Polling interval in milliseconds */


      // Bulk-OUT endpoint , total 25 */
      0x07,   /*Endpoint descriptor length = 7 */
      0x05,   /*Endpoint descriptor type */
      0x02,   /*Endpoint address (OUT, address 2) */
      0x02,   /*Bulk endpoint type */
      0x40,   /*Maximum packet size (64 bytes) */
      0x00,
      0x00     /*Polling interval in milliseconds*/
      /*32*/
  };

/* USB String Descriptors */
const uint8_t usb_dev_StringLangID[USB_DEV_STRING_LANGID_SIZE] =
  {
    USB_DEV_STRING_LANGID_SIZE,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

const uint8_t usb_dev_StringVendor[USB_DEV_STRING_VENDOR_SIZE] =
  {
    USB_DEV_STRING_VENDOR_SIZE,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer:  */
    'K', 0, 'l', 0, 'e', 0, 'n', 0, 'T', 0, 'e', 0, 'c', 0, 'h', 0,
    '©', 0, ' ', 0, 't', 0, 'o', 0, 'o', 0, 'l', 0, 's', 0, ' ', 0,
    'f', 0, 'a', 0, 'c', 0, 't', 0, 'o', 0 ,'r', 0, 'y', 0, ' ', 0
  };

const uint8_t usb_dev_StringProduct[USB_DEV_STRING_PRODUCT_SIZE] =
  {
    USB_DEV_STRING_PRODUCT_SIZE,          // bLength
    USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
    'T', 0, 'E', 0, '-', 0, 'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0,
    'F', 0, '1', 0, '0', 0, '3', 0, ' ', 0, 'f', 0, 'i', 0, 'r', 0,
    'w', 0, 'a', 0, 'r', 0, 'e', 0, ' ', 0, 'v', 0, '0', 0, '.', 0,
    '1', 0
  };

const uint8_t usb_dev_StringSerial[USB_DEV_STRING_SERIAL_SIZE] =
  {
    USB_DEV_STRING_SERIAL_SIZE,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,           /* bDescriptorType */
    'A', 0, '0', 0, '0', 0, '0', 0
  };

const uint8_t usb_dev_StringInterface[USB_DEV_STRING_INTERFACE_SIZE] =
  {
    USB_DEV_STRING_INTERFACE_SIZE,
    USB_STRING_DESCRIPTOR_TYPE,
    /* Interface 0: */
    'U', 0, 'S', 0, 'B', 0, ' ', 0, ' ', 0, 'l', 0, 'i', 0,'n',0,'k',0
  };


