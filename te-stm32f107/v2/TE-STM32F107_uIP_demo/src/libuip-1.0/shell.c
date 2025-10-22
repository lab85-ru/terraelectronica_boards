 /*
 * Copyright (c) 2003, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: shell.c,v 1.1 2006/06/07 09:43:54 adam Exp $
 *
 */
#include <string.h>


#include "shell.h"
#include "uip.h"

#include "appdefs.h"
#include "rtl8201cp.h"
#include "can_test.h"

struct ptentry {
  char *commandstr;
  void (* pfunc)(char *str);
};

#define SHELL_PROMPT "TE-STM32F107> "

/*---------------------------------------------------------------------------*/
static void
parse(register char *str, struct ptentry *t)
{
  struct ptentry *p;
  for(p = t; p->commandstr != NULL; ++p) {
    if(strncmp(p->commandstr, str, strlen(p->commandstr)) == 0)
    {
      str = str + strlen(p->commandstr) ;
      break;
    }
  }

  p->pfunc(str);
}
//-----------------------------------------------------------------------------
static char* itoa ( register unsigned int val)
{
  static char buf[32] ;
  static const char table[] = {'0','1','2','3','4','5','6','7','8','9'} ;
  unsigned int tmp = val ;
  memset (buf,0,32);
  int pos = 10 ;
  do
   {
    pos--;
    buf[pos] = table[ tmp % 10 ] ;
    tmp /= 10 ;
   } while (tmp) ;
  return buf + pos ;
}
//---------------------------------------------------------------------------
static char* itoah ( register unsigned int val , unsigned int radix)
{
  static char buf[32] ;
  static const char table[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'} ;
  unsigned int tmp = val ;
  memset (buf,0,32);
  int pos = 10 ;
  do
   {
    pos--;
    buf[pos] = table[ tmp % radix ] ;
    tmp /= radix ;
   } while (tmp) ;
  return buf + pos ;
}
/*---------------------------------------------------------------------------*/
static void
stats(char *str)
{
  shell_output("IP packets received \t\t", itoa(uip_stat.ip.recv));
  shell_output("IP packets sent \t\t", itoa(uip_stat.ip.sent));
  shell_output("IP packets dropped \t\t", itoa(uip_stat.ip.drop));
  shell_output("ICMP packets received \t\t", itoa(uip_stat.icmp.recv));
  shell_output("ICMP packets sent \t\t", itoa(uip_stat.icmp.sent));
  shell_output("ICMP packets dropped \t\t", itoa(uip_stat.icmp.drop));
  shell_output("TCP packets received \t\t", itoa(uip_stat.tcp.recv));
  shell_output("TCP packets sent \t\t", itoa(uip_stat.tcp.sent));
  shell_output("TCP packets dropped \t\t", itoa(uip_stat.tcp.drop));
  shell_output("TCP packets retransmitted \t", itoa(uip_stat.tcp.rexmit));
  shell_output("TCP connection attempts \t", itoa(uip_stat.tcp.synrst));
}
//---------------------------------------------------------------------------
static void
phy(char *str)
{
  if (!strlen(str))
   {
	  shell_output( "Reatek RTL8201CP PHY registr read command:" , "");
	  shell_output( "   phy0    Basic Mode Control Register (0x0)" , "");
	  shell_output( "   phy1    Basic Mode Status Register (0x1)" , "");
	  shell_output( "   phy2    PHY Identifier Register 1 (0x2)" , "");
	  shell_output( "   phy3    PHY Identifier Register 2 (0x3)" , "");
	  shell_output( "   phy4    Auto-Negotiation Advertisement Reg (0x4)" , "");
	  shell_output( "   phy5    uto-Negotiation Link Partner Ability Reg (0x5)" , "");
	  shell_output( "   phy6    Auto-Negotiation Expansion Reg (0x6)" , "");
	  shell_output( "   phy16   NWay Setup Reg (0x16)" , "");
	  shell_output( "   phy17   Loopback, Bypass, Receiver Error Mask Reg (0x17)" , "");
	  shell_output( "   phy18   RX_ER Counter Reg (0x18" , "");
	  shell_output( "   phy19   NR Display Reg (0x19)" , "");
	  shell_output( "   phy25   Test Reg (0x25)" , "");
	  return ;
   }

  rtl8201cp_read_regs( PHY_ADDRESS ) ;

  switch ( atoi(str) )
    {
      case 0 :
              shell_output( "Basic Mode Control Register (0x0):" , "");
              shell_output( itoa(phy_bmcr.bits.DuplexMode) , "\tDuplexMode");
              shell_output( itoa(phy_bmcr.bits.Loopback) , "\tLoopback");
              shell_output( itoa(phy_bmcr.bits.PowerDown), "\tPowerDown");
              shell_output( itoa(phy_bmcr.bits.Reserved0), "\tReserved0\t\t");
              shell_output( itoa(phy_bmcr.bits.Reserved1), "\tReserved1\t\t");
              shell_output( itoa(phy_bmcr.bits.Reset), "\tReset\t\t");
              shell_output( itoa(phy_bmcr.bits.RestartAutoEnable), "\tRestartAutoEnable\t\t");
              shell_output( itoa(phy_bmcr.bits.RestartAutoNegotiation), "\tRestartAutoNegotiation\t\t");
              shell_output( itoa(phy_bmcr.bits.SpdSet), "\tSpdSet\t\t");
              break ;
      case 1:
              shell_output( "Basic Mode Status Register (0x1):" , "");
              shell_output( itoa(phy_bmsr.bits.AutoNegotiation) , "\tAutoNegotiation");
              shell_output( itoa(phy_bmsr.bits.AutoNegotiationComplite) , "\tAutoNegotiationComplite");
              shell_output( itoa(phy_bmsr.bits.ExtendedCapability), "\tExtendedCapability");
              shell_output( itoa(phy_bmsr.bits.JabberDetect), "\tJabberDetect");
              shell_output( itoa(phy_bmsr.bits.LinkStatus), "\tLinkStatus");
              shell_output( itoa(phy_bmsr.bits.MF_PreambuleSuppression), "\tMF_PreambuleSuppression");
              shell_output( itoa(phy_bmsr.bits.RemoteFault), "\tRemoteFault");
              shell_output( itoa(phy_bmsr.bits.Reserved), "\tReserved");
              shell_output( itoa(phy_bmsr.bits._100Base_TX_FD), "\t100Base_TX_FD");
              shell_output( itoa(phy_bmsr.bits._100Base_TX_HD), "\t100Base_TX_HD");
              shell_output( itoa(phy_bmsr.bits._10Base_T4), "\t10Base_T4\t\t");
              shell_output( itoa(phy_bmsr.bits._10Base_T_FD), "\t10Base_T_FD");
              shell_output( itoa(phy_bmsr.bits._10Base_T_HD), "\t10Base_T_HD");
              break ;
      case 2:
              shell_output( "PHY Identifier Register 1 (0x2):" , "");
              shell_output( itoah(phy_id1r , 16 ), "h\tphy_id1r" );
      case 3:
              shell_output( "PHY Identifier Register 2 (0x3):" , "");
              shell_output( itoah(phy_id2r , 16 ), "h\tphy_id2r"  );
              break ;
      case 4:
              shell_output( "Auto-Negotiation Advertisement Reg (0x4):" , "");
              shell_output( itoa(phy_anar.bits.ACK), "\tACK");
              shell_output( itoa(phy_anar.bits.NP), "\tNP");
              shell_output( itoa(phy_anar.bits.RF), "\tRF");
              shell_output( itoa(phy_anar.bits.RXFC), "\tRXFC");
              shell_output( itoa(phy_anar.bits.Reserved), "\tReserved");
              shell_output( itoa(phy_anar.bits.Selector), "\tSelector");
              shell_output( itoa(phy_anar.bits.T4), "\tT4");
              shell_output( itoa(phy_anar.bits.TX), "\tTX");
              shell_output( itoa(phy_anar.bits.TXFC), "\tTXFC");
              shell_output( itoa(phy_anar.bits.TXFD), "\tTXFD");
              shell_output( itoa(phy_anar.bits._10), "\t10");
              shell_output( itoa(phy_anar.bits._10FD), "\t10FD");
              break ;
      case 5:
              shell_output( "Auto-Negotiation Link Partner Ability Reg (0x5):" , "");
              shell_output( itoa(phy_anlpar.bits.ACK), "\tACK");
              shell_output( itoa(phy_anlpar.bits.NP), "\tNP");
              shell_output( itoa(phy_anlpar.bits.RF), "\tRF");
              shell_output( itoa(phy_anlpar.bits.RXFC), "\tRXFC");
              shell_output( itoa(phy_anlpar.bits.Reserved), "\tReserved");
              shell_output( itoa(phy_anlpar.bits.Selector), "\tSelector");
              shell_output( itoa(phy_anlpar.bits.T4), "\tT4");
       		  shell_output( itoa(phy_anlpar.bits.TXFC), "\tTXFC");
              shell_output( itoa(phy_anlpar.bits.TXFD), "\tTXFD");
              shell_output( itoa(phy_anlpar.bits._100BASE_TX), "\t100BASE_TX");
              shell_output( itoa(phy_anlpar.bits._10Base_T), "\t10Base_T");
       		  shell_output( itoa(phy_anlpar.bits._10FD), "\t10FD");
              break ;
      case 6:
              shell_output( "Auto-Negotiation Expansion Reg (0x6):" , "");
              shell_output( itoa(phy_aner.bits.LP_NP_ABLE), "\tLP_NP_ABLE");
              shell_output( itoa(phy_aner.bits.LP_NW_ABLE), "\tLP_NW_ABLE");
              shell_output( itoa(phy_aner.bits.NP_ABLE), "\tNP_ABLE");
              shell_output( itoa(phy_aner.bits.PAGE_RX), "\tPAGE_RX");
              shell_output( itoa(phy_aner.bits.MLF), "\tMLF");
              shell_output( itoa(phy_aner.bits.Reserved), "\tReserved");
              break ;
      case 16:
    	      shell_output( "NWay Setup Reg (0x16):" , "");
              shell_output( itoa(phy_nwsr.bits.ENNWLE), "\tENNWLE");
              shell_output( itoa(phy_nwsr.bits.FLAGABD), "\tFLAGABD");
              shell_output( itoa(phy_nwsr.bits.FLAGLSC), "\tFLAGLSC");
              shell_output( itoa(phy_nwsr.bits.FLAGPDF), "\tFLAGPDF");
              shell_output( itoa(phy_nwsr.bits.NWLPBK), "\tNWLPBK");
              shell_output( itoa(phy_nwsr.bits.Reserved_0), "\tReserved_0");
              shell_output( itoa(phy_nwsr.bits.Reserved_1), "\tReserved_1");
              shell_output( itoa(phy_nwsr.bits.Testfun), "\tTestfun");
              break ;
      case 17:
          	  shell_output( "Loopback, Bypass, Receiver Error Mask Reg (0x17):" , "");
              shell_output( itoa(phy_lpremr.bits.AnalogOFF), "\tAnalogOFF");
          	  shell_output( itoa(phy_lpremr.bits.BP_4B5B), "\tBP_4B5Bt");
          	  shell_output( itoa(phy_lpremr.bits.BP_SCR), "\tBP_SCR");
          	  shell_output( itoa(phy_lpremr.bits.CODE_err), "\tCODE_errt");
              shell_output( itoa(phy_lpremr.bits.FXMODE), "\tFXMODE");
          	  shell_output( itoa(phy_lpremr.bits.F_Link_10), "\tF_Link_10");
          	  shell_output( itoa(phy_lpremr.bits.F_Link_100), "\tF_Link_100");
          	  shell_output( itoa(phy_lpremr.bits.JBEN), "\tJBEN");
          	  shell_output( itoa(phy_lpremr.bits.LB), "\tLB");
          	  shell_output( itoa(phy_lpremr.bits.LDPS), "\tLDPS");
          	  shell_output( itoa(phy_lpremr.bits.LINK_err), "\tLINK_err");
          	  shell_output( itoa(phy_lpremr.bits.PKT_err), "\tPKT_err");
          	  shell_output( itoa(phy_lpremr.bits.PME_err), "\tPME_err");
          	  shell_output( itoa(phy_lpremr.bits.RMIIMODE), "\tRMIIMODE");
          	  shell_output( itoa(phy_lpremr.bits.RPTR), "\tRPTR");
          	  shell_output( itoa(phy_lpremr.bits.Reserve), "\tReserve");
              break ;
      case 18:
    	      shell_output( "RX_ER Counter Reg (0x18):" , "");
    	      shell_output( itoa(phy_rxercr), "");
              break ;
      case 19:
    	      shell_output( "SNR Display Reg (0x19):" , "");
    	      shell_output( itoa(phy_snrdr & 0x8), "\tSNR");
    	      shell_output( itoa(phy_snrdr>>4), "\tReserved");
    	      break ;
      case 25:
    	      shell_output( "Test Reg (0x25):" , "");
              shell_output( itoa(phy_tr.bits.LINK10), "\tLINK10");
              shell_output( itoa(phy_tr.bits.LINK100), "\tLINK100");
              shell_output( itoa(phy_tr.bits.PHYAD), "\tPHYAD");
              shell_output( itoa(phy_tr.bits.Test_1), "\tTest_1");
              shell_output( itoa(phy_tr.bits.Test_2), "\tTest_2");
              break;
      default:
    	      shell_output( "Realtek RLT8201CP PHY not present register " , str );
    }
}
//--------------------------------------------------------------------------
static void
help(char *str)
{
  shell_output("Available commands:", "");
  shell_output("stats   - show network statistics", "");
  shell_output("conn    - show TCP connections", "");
  shell_output("help, ? - show help", "");
  shell_output("exit    - exit shell", "");
}
/*---------------------------------------------------------------------------*/
static void
unknown(char *str)
{
  if(strlen(str) > 0) {
    shell_output("Unknown command: ", str);
  }
}
//----------------------------------------------------------------------------
static void
ifconfig(char *str)
{
 /*   uip_sethostaddr (s->ipaddr);
    uip_setnetmask (s->netmask);
    uip_setdraddr (s->default_router);
    uip_setsntpaddr (s->sntpaddr);*/

  uint8_t ipaddr[4] ;
  char addr[64] ;
  uip_gethostaddr (ipaddr) ;
  sprintf ( addr , "%i.%i.%i.%i" , ipaddr[0], ipaddr[1] , ipaddr[2] , ipaddr[3] ) ;
  shell_output(addr, "");
  uip_getnetmask (ipaddr) ;
  sprintf ( addr , "%i.%i.%i.%i" , ipaddr[0], ipaddr[1] , ipaddr[2] , ipaddr[3] ) ;
  shell_output(addr, "");
  uip_getdraddr (ipaddr) ;
  sprintf ( addr , "%i.%i.%i.%i" , ipaddr[0], ipaddr[1] , ipaddr[2] , ipaddr[3] ) ;
  shell_output(addr, "");
  uip_getsntpaddr (ipaddr) ;
  sprintf ( addr , "%i.%i.%i.%i" , ipaddr[0], ipaddr[1] , ipaddr[2] , ipaddr[3] ) ;
  shell_output(addr, "");
}
//---------------------------------------------------------------------------
static void can(char *str)
{
  if ( can_test() )
	  shell_output("   test packet transfer CAN1->CAN2 and CAN2->CAN1 is OK!", "");
  else
	  shell_output("   CAN test is fails", "");
}
//---------------------------------------------------------------------------
static const struct ptentry parsetab[] =
  {
   {"phy", phy },
   {"stats", stats},
   //{"conn", help},
   {"help", help},
   {"exit", shell_quit},
   {"ifconfig", ifconfig },
   {"?", help},
   {"can", can},

   /* Default action */
   {NULL, unknown}};
/*---------------------------------------------------------------------------*/
void
shell_init(void)
{
}
/*---------------------------------------------------------------------------*/
void
shell_start(void)
{
  shell_output("uIP command shell", "");
  shell_output("Type '?' and return for help", "");
  shell_prompt(SHELL_PROMPT);
}
/*---------------------------------------------------------------------------*/
void
shell_input(char *cmd)
{
  parse(cmd, parsetab);
  shell_prompt(SHELL_PROMPT);
}
/*---------------------------------------------------------------------------*/
