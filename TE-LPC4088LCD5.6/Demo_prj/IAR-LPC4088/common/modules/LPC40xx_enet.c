/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : LPC40xx_enet.c
 *    Description : MAC/DMA Controller with DMA (ENET) driver
 *
 *    History :
 *    1. Date        : March 14, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 59014 $
 **************************************************************************/
#include <string.h>
#include "LPC407x_8x.h"
#include "arm_comm.h"
#include "uip-conf.h"
#include "LPC40xx_enet.h"
#include "board.h"
#include "drv_glcd.h"

#pragma segment="EMAC_DMA_RAM"

#pragma location="EMAC_DMA_RAM"
#pragma data_alignment=4
__no_init EnetDmaRxDesc_t EnetDmaRx[ENET_DMA_DESC_NUMB];
#pragma location="EMAC_DMA_RAM"
#pragma data_alignment=8
__no_init EnetDmaRxStatus_t EnetDmaRxSta[ENET_DMA_DESC_NUMB];
#pragma location="EMAC_DMA_RAM"
#pragma data_alignment=4
__no_init uint8_t RxBuff0[EMAC_MAX_PACKET_SIZE];
#pragma location="EMAC_DMA_RAM"
#pragma data_alignment=4
__no_init uint8_t RxBuff1[EMAC_MAX_PACKET_SIZE];


#pragma location="EMAC_DMA_RAM"
#pragma data_alignment=4
__no_init EnetDmaTxDesc_t EnetDmaTx[ENET_DMA_DESC_NUMB];
#pragma location="EMAC_DMA_RAM"
#pragma data_alignment=4
__no_init EnetDmaTxStatus_t EnetDmaTxSta[ENET_DMA_DESC_NUMB];
#pragma location="EMAC_DMA_RAM"
#pragma data_alignment=4
__no_init uint8_t TxBuff0[EMAC_MAX_PACKET_SIZE];
#pragma location="EMAC_DMA_RAM"
#pragma data_alignment=4
__no_init uint8_t TxBuff1[EMAC_MAX_PACKET_SIZE];

static uint8_t PhyAddr;

#define MIND_BUSY        (1u<<0)
#define MIND_NOT_VALID   (1u<<2)
/*************************************************************************
 * Function Name:
 * Parameters: None
 *
 * Return: Boolean
 *
 * Description: Init  MAC/DMA Controller
 *
 *************************************************************************/
Boolean tapdev_init(void)
{
uint32_t Reg,to;

  // clk enable
  LPC_SC->PCONP |= (1<<30);
  LPC_EMAC->PowerDown = 0;

  // Pins assignment
  Reg = MAC_MODULEID;

  ETH_TX0_IOCON = 0x21;
  ETH_TX1_IOCON = 0x21;
  ETH_TX_EN_IOCON = 0x21;
  ETH_CRS_IOCON = 0x21;
  ETH_RX0_IOCON = 0x21;
  ETH_RX1_IOCON = 0x21;
  ETH_RX_ER_IOCON = 0xA1;
  ETH_REF_CLK_IOCON = 0x21;
  ETH_MDC_IOCON = 0xA1;
  ETH_MDIO_IOCON = 0xA1;

  // Reset entire MAC
  LPC_EMAC->MAC1 = 0x0000CF00;
  LPC_EMAC->Command = 0x0038; // reset all control registers
  LPC_EMAC->MAC1 = 0;
  LPC_EMAC->Command |= (1<<9);
  LPC_EMAC->SUPP = 0;
  LPC_EMAC->TEST = 0;

  // write the station address registers
  LPC_EMAC->SA0 = (UIP_ETHADDR1<<8) | UIP_ETHADDR0;
  LPC_EMAC->SA1 = (UIP_ETHADDR3<<8) | UIP_ETHADDR2;
  LPC_EMAC->SA2 = (UIP_ETHADDR5<<8) | UIP_ETHADDR4;

  LPC_EMAC->MAXF = 0x600;
  LPC_EMAC->MCFG = 0x8028;  // clk/44
  LPC_EMAC->MCMD = 0;
  // MIIM init
  LPC_EMAC->MCFG &= ~(1<<15);

  // descriptors init
  LPC_EMAC->MAC1 |= (1<<9);
  LPC_EMAC->Command &= ~(1<<1);
  LPC_EMAC->MAC1 |= (1<<8);
  LPC_EMAC->Command |= (1<<4);
  LPC_EMAC->MAC1 &= ~((1<<8) | (1<<9));

  LPC_EMAC->MAC1 &= ~(1<<0);
  LPC_EMAC->MAC1 |= (1<<11);
  LPC_EMAC->Command &= ~(1<<0);
  LPC_EMAC->MAC1 |= (1<<10);
  LPC_EMAC->Command |= (1<<5);
  LPC_EMAC->MAC1 &= ~((1<<10) | (1<<11));

  LPC_EMAC->RxDescriptorNumber = ENET_DMA_DESC_NUMB-1;
  LPC_EMAC->TxDescriptorNumber = ENET_DMA_DESC_NUMB-1;
  LPC_EMAC->Command |= (1<<6);
  LPC_EMAC->Command |= (1<<7);
  LPC_EMAC->MAC1 |= (1<<1);
  LPC_EMAC->MAC2 = 0x30;


  LPC_EMAC->CLRT = 0x370F;
  LPC_EMAC->IPGR = 0x0C12;
  LPC_EMAC->IPGT = 0x12;
  LPC_EMAC->Command &= ~(1<<10);
  LPC_EMAC->MAC2 &= ~(1<<0);



  /* find PHY address */

  for(PhyAddr = 1; PhyAddr < 32; ++PhyAddr)
  {
    if((ENET_MIIReadRegister(PhyAddr, PHY_PHYIDR1) & 0xFFFF) != 0x0007)
    {
      continue;
    }

    if ((ENET_MIIReadRegister(PhyAddr, PHY_PHYIDR2) & 0xFFFF) == 0xC0F1)
    {
      break;
    }
  }

  if(PhyAddr == 32)
  {
    return(FALSE);
  }


#ifdef TRACE_PHY
  printf("PHY Address - %d\r\n",PhyAddr);
#endif
  ENET_MIIWriteRegister(PhyAddr,PHY_BMCR,BMCR_RESET);

  to = PHY_TO;

  while(to)
  {
    Reg = ENET_MIIReadRegister(PhyAddr,PHY_BMCR);

    if(!(Reg & BMCR_RESET))
    {
      break;
    }
  }

  if(!to)
  {
    return(FALSE);
  }

#ifdef TRACE_PHY
  Reg = ENET_MIIReadRegister(PhyAddr,0);
  printf("PHY_BMCR(0) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,1);
  printf("PHY_BMSR(1) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,0x4);
  printf("PHY_ANAR(4) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,0x5);
  printf("PHY_ANLPAR(5) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,0x6);
  printf("PHY_ANER(6) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,0x1f);
  printf("PHY_100PHY(1F) - 0x%04X\n\r",Reg);
#endif // TRACE_PHY

//  if(ENET_MIIReadRegister(PhyAddr,PHY_BMSR ) & BMSR_NOPREAM)
//  {
    // PHY support preamble suppression
//    LPC_EMAC->MCFG |= 1 << 1;
//  }
/*
#if AUTO_NEGOTIATION_ENA != 0
  // PHY interrupt init, clear and enable
  // Set Auto-Negotiation Advertisement
  ENET_MIIWriteRegister(PhyAddr,PHY_ANAR, 1 | ANAR_10BT | ANAR_10BT_FULL | ANAR_100BT | ANAR_100BT_FULL);
  // Enable Auto-Negotiation
  Reg = BMCR_AN | BMCR_RE_AN;
#else

  Reg = 0;

  if(FIX_DUPLEX == FULL_DUPLEX)
  {
    Reg |= BMCR_DUPLEX;
  }

  if(FIX_SPEED == SPEED_100)
  {
    Reg |= BMCR_SPEED_100;
  }

#endif
*/
  ENET_MIIWriteRegister(PhyAddr,PHY_BMCR,Reg);

  to = PHY_TO;

  while(to)
  {
    --to;
    if(ENET_MIIReadRegister(PhyAddr,PHY_BMSR) & BMSR_LINK_ESTABLISHED)
    {
      GLCD_print("Cable connected OK.\r\n");
      break;
    }
    else
    {
    GLCD_print("Connect cable...\r");
    } 
  }

  if(!to)
  {
    return(FALSE);
  }

#ifdef TRACE_PHY
  Reg = ENET_MIIReadRegister(PhyAddr,0);
  printf("PHY_BMCR(0) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,1);
  printf("PHY_BMSR(1) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,0x4);
  printf("PHY_ANAR(4) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,0x5);
  printf("PHY_ANLPAR(5) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,0x6);
  printf("PHY_ANER(6) - 0x%04X\n\r",Reg);
  Reg = ENET_MIIReadRegister(PhyAddr,0x1f);
  printf("PHY_100PHY(1F) - 0x%04X\n\r",Reg);
#endif // TRACE_PHY

  Reg = ENET_MIIReadRegister(PhyAddr,PHY_100PHY) & PHYCR_MODE;

  switch(Reg)
  {
  case 0x04:  // 10 BASE T Half-duplex
#ifdef TRACE_PHY
    printf("10 BASE T Half-duplex\r\n");
#endif
    Reg = 0;
    break;
  case 0x08:  // 100 BASE TX Half-duplex
#ifdef TRACE_PHY
    printf("100 BASE TX Half-duplex\r\n");
#endif
    Reg = BMCR_SPEED_100;
    LPC_EMAC->SUPP |= (1<<8);
    break;
  case 0x14: // 10 BASE T Full-duplex
#ifdef TRACE_PHY
    printf("10 BASE T Full-duplex\r\n");
#endif
    Reg = BMCR_DUPLEX;
    LPC_EMAC->IPGT = 0x15;
    LPC_EMAC->Command |= (1<<10);
    LPC_EMAC->MAC2 |= (1<<0);
    break;
  case 0x18: // 100 BASE TX Full-duplex
#ifdef TRACE_PHY
    printf("100 BASE TX Full-duplex\r\n");
#endif
    Reg = BMCR_SPEED_100 | BMCR_DUPLEX;
    LPC_EMAC->IPGT = 0x15;
    LPC_EMAC->Command |= (1<<10);
    LPC_EMAC->MAC2 |= (1<<0);
    LPC_EMAC->SUPP |= (1<<8);
    break;
  default:
    return(FALSE);
  }

  // Disable Auto-Negotiation and update speed and duplex settings
  ENET_MIIWriteRegister(PhyAddr,PHY_BMCR,Reg);

  to = PHY_TO;

  while(to)
  {
    --to;
    if(ENET_MIIReadRegister(PhyAddr,PHY_BMSR) & BMSR_LINK_ESTABLISHED)
    {
      break;
    }
  }

  if(!to)
  {
    return(FALSE);
  }

  for(uint32_t i = 0; i < ENET_DMA_DESC_NUMB; ++i)
  {
    EnetDmaRx[i].EnetRxCtrl.Size = EMAC_MAX_PACKET_SIZE-1;
    EnetDmaRx[i].EnetRxCtrl.Intr = 1;
    EnetDmaRxSta[i].Data[0] = EnetDmaRxSta[i].Data[1] = 0;

    EnetDmaTx[i].EnetTxCtrl.Data = (1<<31) | (1<<30) | (1<<29) | (1<<28) | (1<<26) | (EMAC_MAX_PACKET_SIZE-1);
    EnetDmaTxSta[i].Data = 0;

  }

  EnetDmaRx[0].pBuffer = (uint32_t *)RxBuff0;
  EnetDmaRx[1].pBuffer = (uint32_t *)RxBuff1;
  EnetDmaTx[0].pBuffer = (uint32_t *)TxBuff0;
  EnetDmaTx[1].pBuffer = (uint32_t *)TxBuff1;
  LPC_EMAC->RxConsumeIndex = LPC_EMAC->TxProduceIndex = 0;
  LPC_EMAC->TxDescriptor = (uint32_t)EnetDmaTx;
  LPC_EMAC->TxStatus = (uint32_t)EnetDmaTxSta;
  LPC_EMAC->RxDescriptor = (uint32_t)EnetDmaRx;
  LPC_EMAC->RxStatus = (uint32_t)EnetDmaRxSta;

  LPC_EMAC->Command |= (1<<0);
  LPC_EMAC->MAC1 |= (1<<0);
  LPC_EMAC->Command |= (1<<1);

  return(TRUE);
}

/*************************************************************************
 * Function Name: tapdev_read
 * Parameters:
 * Return:
 *
 * Description: Read data for MAC/DMA Controller
 *
 *************************************************************************/
uint32_t tapdev_read(void * pPacket)
{
uint32_t Indx = LPC_EMAC->RxConsumeIndex;
uint32_t Size = EMAC_MAX_PACKET_SIZE;
  if(Indx == LPC_EMAC->RxProduceIndex)
  {
    return(0);
  }
  Size = MIN(Size,(EnetDmaRxSta[Indx].RxSize+1));
  memcpy(pPacket,EnetDmaRx[Indx].pBuffer,Size);
  if(++Indx > LPC_EMAC->RxDescriptorNumber)
  {
    Indx = 0;
  }
  LPC_EMAC->RxConsumeIndex = Indx;
  return(Size);
}

/*************************************************************************
 * Function Name: tapdev_send
 * Parameters:
 * Return: Boolean
 *
 * Description: Send data to MAC/DMA Controller
 *
 *************************************************************************/
Boolean tapdev_send(void *pPacket, uint32_t size)
{
uint32_t Indx, IndxHold = LPC_EMAC->TxProduceIndex + 1;
  if(size == 0)
  {
    return(TRUE);
  }
  if(IndxHold > LPC_EMAC->TxDescriptorNumber)
  {
    IndxHold = 0;
  }
  if(IndxHold == LPC_EMAC->TxConsumeIndex)
  {
    return(FALSE);
  }
  Indx = LPC_EMAC->TxProduceIndex;
  size = MIN(size,EMAC_MAX_PACKET_SIZE);
  memcpy(EnetDmaTx[Indx].pBuffer,pPacket,size);
  EnetDmaTx[Indx].EnetTxCtrl.Size = size - 1;
  LPC_EMAC->TxProduceIndex = IndxHold;

  return(TRUE);
}

/*************************************************************************
 * Function Name: ENET_MIIWriteRegister
 * Parameters: uint8_t DevId, uint8_t RegAddr, uint32_t Value
 * Return: none
 *
 * Description: Writes a value on the PHY registers
 *
 *************************************************************************/
static void ENET_MIIWriteRegister (uint8_t DevId, uint8_t RegAddr, uint32_t Value)
{
  LPC_EMAC->MCMD = 0;                       
  LPC_EMAC->MADR = (DevId<<8) | (RegAddr);
  LPC_EMAC-> MWTD = Value;
  while(LPC_EMAC->MIND & MIND_BUSY);
}

/*************************************************************************
 * Function Name: ENET_MIIReadRegister
 * Parameters: uint8_t DevId, uint8_t RegAddr, uint32_t Value
 * Return: uint32_t
 *
 * Description: Read a value from the PHY registers
 *
 *************************************************************************/
static uint32_t ENET_MIIReadRegister (uint8_t DevId, uint8_t RegAddr)
{
  LPC_EMAC->MCMD = 0;
  LPC_EMAC->MADR = (DevId<<8) | (RegAddr);
  LPC_EMAC->MCMD = 1;             // set read operation
  while(LPC_EMAC->MIND &(MIND_BUSY | MIND_NOT_VALID));
  return(LPC_EMAC->MRDD);
}


