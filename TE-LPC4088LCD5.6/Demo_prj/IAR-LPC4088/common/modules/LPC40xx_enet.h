/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : LPC40xx_enet.h
 *    Description : MAC/DMA Controller with DMA (ENET) driver include file
 *
 *    History :
 *    1. Date        : March 14, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 59014 $
 **************************************************************************/
#include <io_macros.h>

#ifndef __LPC40XX_ENET_H
#define __LPC40XX_ENET_H

#define EMAC_MAX_PACKET_SIZE (UIP_CONF_BUFFER_SIZE + 16)
#define ENET_DMA_DESC_NUMB   2
#define ENET_OK  (1)
#define ENET_NOK (0)

/* EMAC MODULE ID	*/
#define OLD_EMAC_MODULE_ID	((0x3902 << 16) | 0x2000)
#define MAC_MODULEID	      (*(volatile unsigned int *)(0x2009CFFC))

#define AUTO_NEGOTIATION_ENA 0  // Enable PHY Auto-negotiation

#define PHY_TO               666666     // ~10sec

// KS8721B1
#define PHY_BMCR                0x0000
#define PHY_BMSR                0x0001
#define PHY_PHYIDR1             0x0002
#define PHY_PHYIDR2             0x0003
#define PHY_ANAR                0x0004
#define PHY_ANLPAR              0x0005
#define PHY_ANLPARNP            0x0005
#define PHY_ANER                0x0006
#define PHY_ANNPTR              0x0007
#define PHY_LPNPA               0x0008

#define PHY_RECR                0x0015
#define PHY_INTCTRL             0x001B
#define PHY_100PHY              0x001F

/* BMCR bitmap */
#define BMCR_RESET              0x8000
#define BMCR_LOOPBACK           0x4000
#define BMCR_SPEED_100          0x2000
#define BMCR_AN                 0x1000
#define BMCR_POWERDOWN          0x0800
#define BMCR_ISOLATE            0x0400
#define BMCR_RE_AN              0x0200
#define BMCR_DUPLEX             0x0100

/* BMSR bitmap */
#define BMSR_100BE_T4           0x8000
#define BMSR_100TX_FULL         0x4000
#define BMSR_100TX_HALF         0x2000
#define BMSR_10BE_FULL          0x1000
#define BMSR_10BE_HALF          0x0800
//#define BMSR_NOPREAM            0x0040
#define BMSR_AUTO_DONE          0x0020
#define BMSR_REMOTE_FAULT       0x0010
#define BMSR_NO_AUTO            0x0008
#define BMSR_LINK_ESTABLISHED   0x0004

/* PHY_ANAR bitmap */
#define ANAR_NEXT_PAGE          0x8000
#define ANAR_REMOTE_FAULT       0x2000
#define ANAR_PAUSE              0x0C00
#define ANAR_100BE_T4           0x0200
#define ANAR_100BT_FULL         0x0100
#define ANAR_100BT              0x0080
#define ANAR_10BT_FULL          0x0040
#define ANAR_10BT               0x0020
#define ANAR_SELECTOR           0x001F

/* ANLPAR bitmap */
#define ANLPAR_NEXT_PAGE        0x8000
#define ANLPAR_ACKN             0x4000
#define ANLPAR_REMOTE_FAULT     0x2000
#define ANLPAR_PAUSE            0x0C00
#define ANLPAR_100BE_T4         0x0200
#define ANLPAR_100BT_FULL       0x0100
#define ANLPAR_100BT            0x0080
#define ANLPAR_10BT_FULL        0x0040
#define ANLPAR_10BT             0x0020
#define ANLPAR_SELECTOR         0x001F

/* PHY_100PHY setting */
#define PHYCR_MDIX_DIS          0x2000
#define PHYCR_ENR_DET           0x1000
#define PHYCR_FORCE_LINK        0x0800
#define PHYCR_POWER_SAVING      0x0400
#define PHYCR_INTR_LEVEL        0x0200
#define PHYCR_JABBER_ENA        0x0100
#define PHYCR_PAUSE_ENA         0x0080
#define PHYCR_PHY_ISO           0x0040
#define PHYCR_MODE              0x001C
#define PHYCR_SQE_TST_ENA       0x0002
#define PHYCR_SCRAM_DIS         0x0001

#define SPEED_100               1
#define SPEED_10                0
#define FULL_DUPLEX             1
#define HALF_DUPLEX             0

#define FIX_SPEED               SPEED_100
#define FIX_DUPLEX              FULL_DUPLEX

typedef union _EnetRxCR_t
{
  uint32_t Data;
  struct {
    uint32_t  DMA_XFERCOUNT :12;
    uint32_t  CONT_EN       : 1;
    uint32_t                : 1;
    uint32_t  NXT_EN        : 1;
    uint32_t  DLY_EN        : 1;
    uint32_t                : 1;
    uint32_t  ENTRY_TRIG    : 5;
    uint32_t  ADDR_WRAP     :10;
  };
} EnetRxCR_t, * pEnetRxCR_t;

typedef union _EnetRxSR_t
{
  uint32_t Data;
  struct {
    uint32_t  FrameLength     :11;
    uint32_t                  : 1;
    uint32_t  Overlength      : 1;
    uint32_t  FalseCarrier    : 1;
    uint32_t  WatchdogTO      : 1;
    uint32_t  RuntFrame       : 1;
    uint32_t  Valid           : 1;
    uint32_t  LateCollision   : 1;
    uint32_t  FrameType       : 1;
    uint32_t  MIIError        : 1;
    uint32_t  ExtraBits       : 1;
    uint32_t  CRCError        : 1;
    uint32_t  OneLevelVLAN    : 1;
    uint32_t  TwoLevelVLAN    : 1;
    uint32_t  LengthError     : 1;
    uint32_t  ControlFrame    : 1;
    uint32_t  UnsupportedFrame: 1;
    uint32_t  MulticastFrame  : 1;
    uint32_t  BroadcastFrame  : 1;
    uint32_t  FilteringFail   : 1;
    uint32_t  PacketFilter    : 1;
    uint32_t  FrameAbort      : 1;
  };
} EnetRxSR_t, * pEnetRxSR_t;

typedef union _EnetTxSR_t
{
  uint32_t Data;
  struct {
    uint32_t  FrameAborted    : 1;
    uint32_t                  : 1;
    uint32_t  NoCarrier       : 1;
    uint32_t  LossOfCarrier   : 1;
    uint32_t  ExcessiveDef    : 1;
    uint32_t  LateCollision   : 1;
    uint32_t  ExcessiveColl   : 1;
    uint32_t  UnderRun        : 1;
    uint32_t  Deferred        : 1;
    uint32_t  LateCollisionObs: 1;
    uint32_t  CollisionCount  : 4;
    uint32_t                  : 2;
    uint32_t  Valid           : 1;
    uint32_t                  : 1;
    uint32_t  ByteCounter     :13;
    uint32_t  PacketRetry     : 1;
  };
} EnetTxSR_t, * pEnetTxSR_t;


typedef union _EnetTxCtrl_t
{
  uint32_t Data;
  struct
  {
    uint32_t Size     : 11;
    uint32_t          : 15;
    uint32_t Override :  1;
    uint32_t Huge     :  1;
    uint32_t Pad      :  1;
    uint32_t CRC      :  1;
    uint32_t Last     :  1;
    uint32_t Intr     :  1;
  };
} EnetTxCtrl_t, *pEnetTxCtrl_t;

typedef struct _EnetDmaTxDesc_t
{
  uint32_t *      pBuffer;
  EnetTxCtrl_t EnetTxCtrl;
} EnetDmaTxDesc_t, * pEnetDmaTxDesc_t;

typedef union _EnetDmaTxStatus_t
{
  uint32_t Data;
  struct
  {
    uint32_t                    :21;
    uint32_t CollisionCount     : 4;
    uint32_t Defer              : 1;
    uint32_t ExcessiveDefer     : 1;
    uint32_t ExcessiveCollision : 1;
    uint32_t LateCollision      : 1;
    uint32_t Underrun           : 1;
    uint32_t NoDescriptor       : 1;
    uint32_t Error              : 1;
  };
} EnetDmaTxStatus_t, * pEnetDmaTxStatus_t;

typedef struct _EnetRxCtrl_t
{
  uint32_t Size     : 11;
  uint32_t          : 20;
  uint32_t Intr     :  1;
} EnetRxCtrl_t, *pEnetRxCtrl_t;

typedef struct _EnetDmaRxDesc_t
{
  uint32_t *      pBuffer;
  EnetRxCtrl_t EnetRxCtrl;
} EnetDmaRxDesc_t, * pEnetDmaRxDesc_t;

typedef union _EnetDmaRxStatus_t
{
  uint32_t Data[2];
  struct
  {
    uint32_t RxSize         :11;
    uint32_t                : 7;
    uint32_t ControlFrame   : 1;
    uint32_t VLAN           : 1;
    uint32_t FailFilter     : 1;
    uint32_t Multicast      : 1;
    uint32_t Broadcast      : 1;
    uint32_t CRCError       : 1;
    uint32_t SymbolError    : 1;
    uint32_t LengthError    : 1;
    uint32_t RangeError     : 1;
    uint32_t AlignmentError : 1;
    uint32_t Overrun        : 1;
    uint32_t NoDescriptor   : 1;
    uint32_t LastFlag       : 1;
    uint32_t Error          : 1;
    uint32_t SAHashCRC      : 8;
    uint32_t                : 8;
    uint32_t DAHashCRC      : 8;
    uint32_t                : 8;
  };
} EnetDmaRxStatus_t, * pEnetDmaRxStatus_t;

/*************************************************************************
 * Function Name:
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init  MAC/DMA Controller
 *
 *************************************************************************/
Boolean tapdev_init(void);

/*************************************************************************
 * Function Name: tapdev_read
 * Parameters:
 * Return:
 *
 * Description: Read data for MAC/DMA Controller
 *
 *************************************************************************/
uint32_t tapdev_read(void * pPacket);

/*************************************************************************
 * Function Name: tapdev_send
 * Parameters:
 * Return: Boolean
 *
 * Description: Send data to MAC/DMA Controller
 *
 *************************************************************************/
Boolean tapdev_send (void *pPacket, uint32_t size);

/*************************************************************************
 * Function Name: ENET_MIIWriteRegister
 * Parameters: uint8_t DevId, uint8_t RegAddr, uint32_t Value
 * Return: none
 *
 * Description: Writes a value on the PHY registers
 *
 *************************************************************************/
static void ENET_MIIWriteRegister (uint8_t DevId, uint8_t RegAddr, uint32_t Value);

/*************************************************************************
 * Function Name: ENET_MIIReadRegister
 * Parameters: uint8_t DevId, uint8_t RegAddr, uint32_t Value
 * Return: uint32_t
 *
 * Description: Read a value from the PHY registers
 *
 *************************************************************************/
static uint32_t ENET_MIIReadRegister (uint8_t DevId, uint8_t RegAddr);

#endif // __LPC40XX_ENET_H

