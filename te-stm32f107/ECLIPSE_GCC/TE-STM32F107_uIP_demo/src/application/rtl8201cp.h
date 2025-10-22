#ifndef __RTL8201CP_H__
#define __RTL8201CP_H__

#include <stdint.h>

//----------------------------------------------------------------
typedef struct
{
   union
    {
	 uint16_t reg_val  ;
	 struct
	 {
	   	uint16_t Reserved0 : 8 ;
	   	uint16_t DuplexMode : 1 ;
	   	uint16_t RestartAutoNegotiation : 1 ;
	   	uint16_t Reserved1 : 1 ;
	   	uint16_t PowerDown : 1 ;
	   	uint16_t RestartAutoEnable : 1 ;
	   	uint16_t SpdSet : 1 ;
	   	uint16_t Loopback : 1 ;
	   	uint16_t Reset : 1 ;
	 } bits ;
    };
} BasicModeControlReg ;
//---------------------------------------------------------------
typedef struct
{
   union
    {
	 uint16_t reg_val  ;
	 struct
	 {
	   	uint16_t ExtendedCapability : 1 ;
	   	uint16_t JabberDetect : 1 ;
	   	uint16_t LinkStatus : 1 ;
	   	uint16_t AutoNegotiation : 1 ;
	   	uint16_t RemoteFault : 1 ;
	   	uint16_t AutoNegotiationComplite : 1 ;
	   	uint16_t MF_PreambuleSuppression  : 1 ;
	   	uint16_t Reserved : 4 ;
	   	uint16_t _10Base_T_HD : 1 ;
	   	uint16_t _10Base_T_FD : 1 ;
	   	uint16_t _100Base_TX_HD : 1 ;
	   	uint16_t _100Base_TX_FD : 1 ;
	   	uint16_t _10Base_T4: 1 ;
	 } bits ;
    };
} BasicModeStatusReg  ;
//---------------------------------------------------------------
typedef struct
{
   union
    {
	 uint16_t reg_val  ;
	 struct
	 {
	   	uint16_t Selector : 5 ;
	   	uint16_t _10 : 1 ;
	   	uint16_t _10FD : 1 ;
	   	uint16_t TX : 1 ;
	   	uint16_t TXFD : 1 ;
	   	uint16_t T4 : 1 ;
	   	uint16_t RXFC : 1 ;
	   	uint16_t TXFC : 1 ;
	   	uint16_t Reserved : 1 ;
	   	uint16_t RF : 1 ;
	   	uint16_t ACK : 1 ;
	   	uint16_t NP : 1 ;
	 } bits ;
    };
} AutoNegotiationAdvertisementReg  ;
//-------------------------------------------------------------
typedef struct
{
   union
    {
	 uint16_t reg_val  ;
	 struct
	 {
	   	uint16_t  Selector : 5 ;
	   	uint16_t _10Base_T : 1 ;
	   	uint16_t _10FD : 1 ;
	   	uint16_t _100BASE_TX : 1 ;
	   	uint16_t TXFD : 1 ;
	   	uint16_t T4 : 1 ;
	   	uint16_t RXFC : 1 ;
	   	uint16_t TXFC : 1 ;
	   	uint16_t Reserved : 1 ;
	   	uint16_t RF : 1 ;
	   	uint16_t ACK : 1 ;
	   	uint16_t NP : 1 ;
	 } bits ;
    };
} AutoNegotiationLinkPartnerAbilityReg  ;
//-------------------------------------------------------------
typedef struct
{
   union
    {
	 uint16_t reg_val  ;
	 struct
	 {
	   	uint16_t LP_NW_ABLE : 1 ;
	   	uint16_t PAGE_RX : 1 ;
	   	uint16_t NP_ABLE : 1 ;
	   	uint16_t LP_NP_ABLE : 1 ;
	   	uint16_t MLF : 1 ;
	   	uint16_t Reserved : 11  ;

	 } bits ;
    };
} AutoNegotiationExpansionReg  ;
//-------------------------------------------------------------
typedef struct
{
   union
    {
	 uint16_t reg_val  ;
	 struct
	 {
	   	uint16_t FLAGLSC : 1 ;
	   	uint16_t FLAGPDF : 1 ;
	   	uint16_t FLAGABD : 1 ;
	   	uint16_t Reserved_0 : 6 ;
	   	uint16_t NWLPBK  : 1 ;
	   	uint16_t Testfun : 1 ;
	   	uint16_t ENNWLE : 1 ;
	   	uint16_t Reserved_1 : 4 ;
	 } bits ;
    };
} NWaySetupReg  ;
//-----------------------------------------------------------
typedef struct
{
   union
    {
	 uint16_t reg_val  ;
	 struct
	 {
	   	uint16_t RMIIMODE : 1 ;
	   	uint16_t FXMODE : 1 ;
	   	uint16_t PKT_err : 1 ;
	   	uint16_t LINK_err : 1 ;
	   	uint16_t PME_err  : 1 ;
	   	uint16_t CODE_err : 1 ;
	   	uint16_t JBEN : 1 ;
	   	uint16_t F_Link_100 : 1 ;
	   	uint16_t F_Link_10 : 1 ;
		uint16_t LB : 1 ;
		uint16_t Reserve : 1 ;
		uint16_t AnalogOFF : 1 ;
		uint16_t LDPS : 1 ;
		uint16_t BP_SCR : 1 ;
		uint16_t BP_4B5B : 1 ;
		uint16_t RPTR : 1 ;
	 } bits ;
    };
} LoopbackBypassReceiverErrorMaskReg  ;
//-----------------------------------------------------------
typedef struct
{
   union
    {
	 uint16_t reg_val  ;
	 struct
	 {
	   	uint16_t LINK100 : 1 ;
	   	uint16_t LINK10 : 1 ;
	   	uint16_t Test_1 : 5 ;
	   	uint16_t PHYAD : 5 ;
	   	uint16_t Test_2  : 4 ;
	 } bits ;
    };
} TestReg ;
//-----------------------------------------------------------

extern BasicModeControlReg phy_bmcr ;
extern BasicModeStatusReg  phy_bmsr ;
extern uint16_t phy_id1r ;
extern uint16_t phy_id2r ;
extern AutoNegotiationAdvertisementReg phy_anar ;
extern AutoNegotiationLinkPartnerAbilityReg phy_anlpar ;
extern AutoNegotiationExpansionReg phy_aner ;
extern NWaySetupReg phy_nwsr ;
extern LoopbackBypassReceiverErrorMaskReg phy_lpremr ;
extern uint16_t phy_rxercr ;
extern uint16_t phy_snrdr ;
extern TestReg phy_tr ;

void rtl8201cp_read_regs( uint16_t phy_addres );

#endif // __RTL8201CP_H__
