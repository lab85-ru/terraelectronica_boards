#include "appdefs.h"
#include "rtl8201cp.h"
#include "stm32_eth.h"

//------------------------------------------------------
BasicModeControlReg phy_bmcr ;
BasicModeStatusReg  phy_bmsr ;
uint16_t phy_id1r ;
uint16_t phy_id2r ;
AutoNegotiationAdvertisementReg phy_anar ;
AutoNegotiationLinkPartnerAbilityReg phy_anlpar ;
AutoNegotiationExpansionReg phy_aner ;
NWaySetupReg phy_nwsr ;
LoopbackBypassReceiverErrorMaskReg phy_lpremr ;
uint16_t phy_rxercr ;
uint16_t phy_snrdr ;
TestReg phy_tr ;
//------------------------------------------------------
// чтение регистров PHY RTL8201CP
void rtl8201cp_read_regs( uint16_t phy_addres )
{
	phy_bmcr.reg_val   = 0xffff ;
	phy_bmsr.reg_val   = 0xffff ;
	phy_id1r           = 0xffff ;
	phy_id2r           = 0xffff ;
	phy_anar.reg_val   = 0xffff ;
	phy_anlpar.reg_val = 0xffff ;
	phy_aner.reg_val   = 0xffff ;
	phy_nwsr.reg_val   = 0xffff ;
	phy_lpremr.reg_val = 0xffff ;
	phy_rxercr         = 0xffff ;
	phy_snrdr          = 0xffff ;
	phy_tr.reg_val     = 0xffff ;


	phy_bmcr.reg_val   = ETH_ReadPHYRegister( phy_addres, 0  );
	phy_bmsr.reg_val   = ETH_ReadPHYRegister( phy_addres, 1  );
	phy_id1r           = ETH_ReadPHYRegister( phy_addres, 2  );
	phy_id2r           = ETH_ReadPHYRegister( phy_addres, 3  );
	phy_anar.reg_val   = ETH_ReadPHYRegister( phy_addres, 4  );
    phy_anlpar.reg_val = ETH_ReadPHYRegister( phy_addres, 5  );
	phy_aner.reg_val   = ETH_ReadPHYRegister( phy_addres, 6  );
	phy_nwsr.reg_val   = ETH_ReadPHYRegister( phy_addres, 16 );
	phy_lpremr.reg_val = ETH_ReadPHYRegister( phy_addres, 17 );
	phy_rxercr         = ETH_ReadPHYRegister( phy_addres, 18 );
	phy_snrdr          = ETH_ReadPHYRegister( phy_addres, 19 );
	phy_tr.reg_val     = ETH_ReadPHYRegister( phy_addres, 25 );
}
