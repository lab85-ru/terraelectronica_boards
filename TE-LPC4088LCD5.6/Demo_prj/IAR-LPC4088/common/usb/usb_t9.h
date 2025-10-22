/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : usb_t9.h
 *    Description : Define usb chapter 9 (device framework) module
 *
 *    History :
 *    1. Date        : June 17, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 57747 $
 **************************************************************************/
#ifndef __USB_T9_H
#define __USB_T9_H

#ifdef  USB_T9_GLOBAL
#define USB_T9_EXTERN
#else
#define USB_T9_EXTERN  extern
#endif

typedef enum _UsbCommStatus_t
{
  UsbPass = 0, UsbFault, UsbNotSupport,
} UsbCommStatus_t;

typedef enum _UsbT9DevState_t
{
  UsbDevStatusUnknow = 0, UsbDevStatusAttached, UsbDevStatusPowered,
  UsbDevStatusDefault, UsbDevStatusAddress, UsbDevStatusConfigured
} UsbT9DevState_t;

typedef enum _UsbSetupType_t
{
  UsbTypeStandart = 0,UsbTypeClass,UsbTypeVendor,UsbTypeReserved
} UsbSetupType_t;

typedef enum _UsbSetupStandatRecuest_t
{
  GET_STATUS = 0,CLEAR_FEATURE, Reserved0, SET_FEATURE, Reserved1,
  SET_ADDRESS, GET_DESCRIPTOR, SET_DESCRIPTOR, GET_CONFIGURATION,
  SET_CONFIGURATION, GET_INTERFACE, SET_INTERFACE, SYNCH_FRAME
} UsbSetupStandatRecuest_t;

typedef enum _UsbSetupRecipient_t
{
  UsbRecipientDevice = 0,UsbRecipientInterface,UsbRecipientEndpoint,
  UsbRecipientOther,UsbRecipientReserved
} UsbSetupRecipient_t;

typedef enum _UsbFeatureSelector_t
{
  UsbEpHaltSelector= 0, UsbDeviceRemoteWakeupSelector, UsbTestModeSelector
} UsbFeatureSelector_t;

typedef enum _UsbCoreReqType_t
{
  UsbCoreReqDevState = 0, UsbCoreReqDevSusState, UsbCoreReqConfiquration,
  UsbCoreReqInterface,  UsbCoreReqDevOwnAddress,
  UsbCoreReqWakeUpEnableStatus, UsbCoreReqSelfPoweredStatus,
  UsbCoreReqSpeed,
} UsbCoreReqType_t;

typedef enum _UsbUserFuncResponse_t
{
  UsbUserError = 0, UsbUserSendPacket, UsbUserReceivePacket, UsbUserSendAckn,
  UsbUserStallCtrlEp,
} UsbUserFuncResponse_t;

typedef enum _UsbDataState_t
{
  UsbHost2Device = 0, UsbDevice2Host
} UsbDataState_t;

#define UsbDevStatusNoSuspend 0
#define UsbDevStatusSuspend   1

#pragma pack(1)
typedef struct _UsbDevState_t
{
  uint8_t DS          : 3;
  uint8_t             : 4;
  uint8_t Suspend     : 1;
} UsbDevState_t, * pUsbDevState_t;

typedef union _UsbRequestType_t
{
  uint8_t mRequestTypeData;
  struct
  {
    uint8_t Recipient : 5;
    uint8_t Type      : 2;
    uint8_t Dir       : 1;
  };
}UsbRequestType_t, * pUsbRequestType_t;

typedef union _TwoBytes_t
{
  uint16_t Word;
  struct
  {
    uint8_t Lo;
    uint8_t Hi;
  };
}TwoBytes_t, * pTwoBytes_t;

typedef union _UsbSetupPacket_t
{
  uint8_t Data[8];
  struct {
    UsbRequestType_t  mRequestType;
    uint8_t             bRequest;
    TwoBytes_t        wValue;
    TwoBytes_t        wIndex;
    TwoBytes_t        wLength;
  };
} UsbSetupPacket_t, * pUsbSetupPacket_t;
#pragma pack()

typedef union _UsbDefFeature_t
{
  uint8_t Data;
  struct
  {
    uint8_t SelfPowered         :1;
    uint8_t RemoteWakeupEnable  :1;
    uint8_t                     :6;
  };
} UsbDefFeature_t, * pUsbDefFeature_t;

typedef struct _UsbDevCtrl_t
{
  UsbDevState_t     State;
  uint32_t            Configuration;
  pUsbStandardConfigurationDescriptor_t pCnfg;
  uint32_t            Interfaces;
  uint32_t            AlternateSetting[USB_MAX_INTERFACE];
  uint32_t            DevAdd;
  UsbDefFeature_t   Feature;
#if USB_HIGH_SPEED > 0
  uint32_t HighSpeed;
#endif // USB_HIGH_SPEED > 0
} UsbDevCtrl_t, * pUsbDevCtrl_t;

#define USB_T9_ERROR_REQUEST() EpCnfg[CTRL_ENP_OUT].Status = STALLED

USB_T9_EXTERN UsbSetupPacket_t UsbEp0SetupPacket;

/*************************************************************************
 * Function Name: UsbCoreInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init USB core
 *
 *************************************************************************/
void UsbCoreInit (void);

/*************************************************************************
 * Function Name: UsbCoreReq
 * Parameters:  UsbCoreReqType_t Type
 *
 * Return: uint32_t
 *
 * Description: Return device states
 *
 *************************************************************************/
uint32_t UsbCoreReq (UsbCoreReqType_t Type);

#if USB_REMOTE_WAKEUP != 0
/*************************************************************************
 * Function Name: UsbWakeUp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Wake-up device from suspend mode
 *
 *************************************************************************/
void UsbWakeUp (void);
#endif // USB_REMOTE_WAKEUP != 0

/*************************************************************************
 * Function Name: UsbSetDevState
 * Parameters:  UsbT9DevState_t DevState
 *
 * Return: none
 *
 * Description: Set device state
 *
 *************************************************************************/
void UsbSetDevState (UsbT9DevState_t DevState);

#if USB_HIGH_SPEED > 0
/*************************************************************************
 * Function Name: UsbDevHighSpeedCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB High speed callback
 *
 *************************************************************************/
void UsbDevHighSpeedCallback (Boolean Connect);
#endif // USB_HIGH_SPEED > 0

/*************************************************************************
 * Function Name: UsbDevConnectCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB connect callback
 *
 *************************************************************************/
void UsbDevConnectCallback (Boolean Connect);

/*************************************************************************
 * Function Name: UsbDevSuspendCallback
 * Parameters:  Boolean Suspend
 *
 * Return: none
 *
 * Description: USB suspend callback
 *
 *************************************************************************/
void UsbDevSuspendCallback (Boolean Suspend);

/*************************************************************************
 * Function Name: UsbDevResetCallback
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB reset callback
 *
 *************************************************************************/
void UsbDevResetCallback (void);

#if USB_HIGH_SPEED > 0
/*************************************************************************
 * Function Name: UsbDevSpeedCallback
 * Parameters: Boolean HighSpeed
 *
 * Return: none
 *
 * Description: USB Speed change callback
 *
 *************************************************************************/
void UsbDevSpeedCallback(Boolean HighSpeed);
#endif // USB_HIGH_SPEED > 0

/*************************************************************************
 * Function Name: USB_T9_Size
 * Parameters:  uint32_t Size,uint32_t HostExpSize
 *
 * Return: uint32_t
 *
 * Description: Return lower from Host expected size and size and set
 * a flag bZeroPacketPossible when size is lower that host expected size
 *
 *************************************************************************/
uint32_t USB_T9_Size(uint32_t Size,uint32_t HostExpSize);

/*************************************************************************
 * Function Name: USB_SetupHandler
 * Parameters: void
 *
 * Return: none
 *
 * Description: Setup packet handler
 *
 *************************************************************************/
void USB_SetupHandler (void);

/*************************************************************************
 * Function Name: USB_StatusHandler
 * Parameters: USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: Address status handler
 *
 *************************************************************************/
void USB_StatusHandler (USB_Endpoint_t EP);

/*************************************************************************
 * Function Name: USB_AddStatusEndHandler
 * Parameters: USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: Setup packet status phase end handler
 *
 *************************************************************************/
static
void USB_StatusEndHandler (USB_Endpoint_t EP);

/*************************************************************************
 * Function Name: USB_AddStatusEndHandler
 * Parameters: USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: Setup packet - set address status phase end handler
 *
 *************************************************************************/
static
void USB_AddStatusEndHandler (USB_Endpoint_t EP);

/*************************************************************************
 * Function Name: UsbFindInterface
 * Parameters:  UsbStandardConfigurationDescriptor_t * pConfiguration,
 *              uint32_t Interface, uint32_t AlternateSetting
 *
 * Return: pUsbStandardInterfaceDescriptor_t
 *
 * Description: Set Interface and  Alternative setting state
 *
 *************************************************************************/
static
pUsbStandardInterfaceDescriptor_t UsbFindInterface (
                      UsbStandardConfigurationDescriptor_t * pConfiguration,
                      uint32_t Interface, uint32_t AlternateSetting);

/*************************************************************************
 * Function Name: UsbFindConfiguration
 * Parameters:  uint32_t
 *
 * Return: UsbStandardConfigurationDescriptor_t *
 *
 * Description: Return pointer to current configuration
 *
 *************************************************************************/
static
UsbStandardConfigurationDescriptor_t * UsbFindConfiguration(uint32_t Configuration);

/*************************************************************************
 * Function Name: UsbSetConfigurtonState
 * Parameters:  uint32_t
 *
 * Return: UsbCommStatus_t
 *
 * Description: Set configuration state
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbSetConfigurtonState (uint32_t Configuration);

/*************************************************************************
 * Function Name: UsbDevStatus
 * Parameters:  uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: Usb Return device status
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbDevStatus(uint16_t Index);

/*************************************************************************
 * Function Name: UsbInterfaceStatus
 * Parameters:  uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Return interface status into pData
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbInterfaceStatus(uint16_t Index);

/*************************************************************************
 * Function Name: UsbEpStatus
 * Parameters:  uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Return EP status into pData
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbEpStatus(uint16_t Index);

/*************************************************************************
 * Function Name: UsbGetStatus
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Get status implement
 *
 *************************************************************************/
inline static
void UsbGetStatus (void);

/*************************************************************************
 * Function Name: UsbClearFeature
 * Parameters:  uint16_t Feature,uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear Device feature implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbClearDevFeature(uint16_t Feature,uint16_t Index);

/*************************************************************************
 * Function Name: UsbClearInterfaceFeature
 * Parameters:  uint16_t Feature,uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear Interface feature implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbClearInterfaceFeature(uint16_t Feature,uint16_t Index);

/*************************************************************************
 * Function Name: UsbClearEpFeature
 * Parameters:  uint16_t Feature,uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear EP feature implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbClearEpFeature(uint16_t Feature,uint16_t Index);

/*************************************************************************
 * Function Name: UsbClearFeature
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Clear feature implement
 *
 *************************************************************************/
inline static
void UsbClearFeature (void);

/*************************************************************************
 * Function Name: UsbSetFeature
 * Parameters:  uint16_t Feature,uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set Device feature implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbSetDevFeature(uint16_t Feature,uint16_t Index);

/*************************************************************************
 * Function Name: UsbSetInterfaceFeature
 * Parameters:  uint16_t Feature,uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set Interface feature implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbSetInterfaceFeature(uint16_t Feature,uint16_t Index);

/*************************************************************************
 * Function Name: UsbSetEpFeature
 * Parameters:  uint16_t Feature,uint16_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set EP feature implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbSetEpFeature(uint16_t Feature,uint16_t Index);

/*************************************************************************
 * Function Name: UsbSetFeature
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set feature implement
 *
 *************************************************************************/
inline static
void UsbSetFeature (void);

/*************************************************************************
 * Function Name: UsbSetAddress
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set address request implement
 *
 *************************************************************************/
inline static
void UsbSetAddress (void);

/*************************************************************************
 * Function Name: UsbGetDescriptorDevice
 * Parameters:  uint8_t Index - must be 0
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get device's descriptor request implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbGetDescriptorDevice(uint8_t Index);

/*************************************************************************
 * Function Name: UsbGetDescriptorConfiguration
 * Parameters:  uint8_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get configuration's descriptor request implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbGetDescriptorConfiguration(uint8_t Index);

/*************************************************************************
 * Function Name: UsbGetDescriptorString
 * Parameters:  uint8_t Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get string's descriptor request implement
 *
 *************************************************************************/
inline static
UsbCommStatus_t UsbGetDescriptorString(uint8_t Index);

/*************************************************************************
 * Function Name: UsbGetDescriptor
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB get descriptor request implement
 *
 *************************************************************************/
inline static
void UsbGetDescriptor (void);

/*************************************************************************
 * Function Name: UsbSetDescriptor
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set descriptor
 *
 *************************************************************************/
inline static
void UsbSetDescriptor(void);

/*************************************************************************
 * Function Name: UsbGetConfiguration
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Get configuration implement
 *
 *************************************************************************/
inline static
void UsbGetConfiguration(void);

/*************************************************************************
 * Function Name: UsbSetConfiguration
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set configuration implement
 *
 *************************************************************************/
inline static
void UsbSetConfiguration(void);

/*************************************************************************
 * Function Name: UsbGetInterface
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement get interface request
 *
 *************************************************************************/
inline static
void UsbGetInterface (void);

/*************************************************************************
 * Function Name: UsbSetInterface
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement set interface request
 *
 *************************************************************************/
inline static
void UsbSetInterface (void);

/*************************************************************************
 * Function Name: UsbSynchFrame
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement synch frame request
 *
 *************************************************************************/
inline static
void UsbSynchFrame (void);

/*************************************************************************
 * Function Name: UsbEpInterfaceCnfg
 * Parameters: pUsbStandardInterfaceDescriptor_t pInterface
 *             Boolean Configure
 *
 * Return: none
 *
 * Description: Configure/deconfigure EPs of a certain interface
 *
 *************************************************************************/
static
void UsbEpInterfaceCnfg(pUsbStandardInterfaceDescriptor_t pInterface,
                        Boolean Configure);

/*************************************************************************
 * Function Name: USB_StatusIn
 * Parameters:  void (*pFp)(USB_Endpoint_t)
 *
 * Return: none
 *
 * Description: Status in phase
 *
 *************************************************************************/
static
void USB_StatusIn (void (*pFp)(USB_Endpoint_t));

#endif //__USB_T9_H
