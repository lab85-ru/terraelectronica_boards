#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#undef HTONS

#include "stm32_eth.h"

#include "uip.h"
#include "uip_arp.h"
#include "httpd.h"
#include "telnetd.h"
#include "dhcpc.h"
#include "sntp.h"
#include "uip_handler.h"


//---------------------------------------------------------------------------------
void TransmitPacket(void)
{
  int i;
      u8 data[UIP_CONF_BUFFER_SIZE];


      // Copy the header portion part
      for(i=0; i < (UIP_LLH_LEN + 40); ++i)
         data[i] =  uip_buf[i];

      // Copy the data portion part
      for(; i < uip_len; ++i)
         data[i] =  ((u8_t*)uip_appdata)[i - UIP_LLH_LEN - 40 ];

      ETH_HandleTxPkt(data,uip_len);
}
//--------------------------------------------------------------------------------
void uip_handler_recv()
{
	static struct uip_eth_hdr* BUF = (struct uip_eth_hdr*)uip_buf; ;

	while ( (uip_len = ETH_HandleRxPkt(uip_buf)) )
	{
	if (uip_len == 0 ) return ;
	if(BUF->type == htons(UIP_ETHTYPE_IP))
	  {
		uip_arp_ipin();
		uip_input();
		/* If the above function invocation resulted in data that
		should be sent out on the network, the global variable
		uip_len is set to a value > 0. */
		if(uip_len > 0)
		  {
		    uip_arp_out();
		    TransmitPacket();
		  }
	  }
	else
	  if(BUF->type == htons(UIP_ETHTYPE_ARP))
	   {
		 uip_arp_arpin();
		 /* If the above function invocation resulted in data that
		 should be sent out on the network, the global variable
		 uip_len is set to a value > 0. */
		 if(uip_len > 0)
		  {
		    TransmitPacket();
		  }
	   }
	}
}
//----------------------------------------------------------------------------------
void uip_handler_periodic(void)
{
	static struct timer arp_timer={0,100};

	uint32_t i ;
    for(i = 0; i < UIP_CONNS; i++)
	   {
		uip_periodic(i);
		/* If the above function invocation resulted in data that
		should be sent out on the network, the global variable
		uip_len is set to a value > 0. */
		if(uip_len > 0)
	     {
	      uip_arp_out();
		  TransmitPacket();
	     }
	   }

    for(i = 0; i < UIP_UDP_CONNS; i++)
	  {
	    uip_udp_periodic(i);
	    /* If the above function invocation resulted in data that
	    should be sent out on the network, the global variable
	    uip_len is set to a value > 0. */
	    if(uip_len > 0)
	     {
	      uip_arp_out();
	      TransmitPacket();
	     }
	  }

    /* Call the ARP timer function every 10 seconds. */
	if(timer_expired(&arp_timer))
	  {
	    timer_reset(&arp_timer);
	    uip_arp_timer();
	  }
}

//----------------------------------------------------------------------------------------------------
void dispatch_tcp_appcall (void)
{
  switch ( HTONS(uip_conn->lport))
   {
     case HTTP_SERVER_PORT : httpd_appcall ();   break ;
     case TELNET_SERVER_PORT: telnetd_appcall (); break ;
     default: {}
   }
}
//----------------------------------------------------------------------------------------------------
void dispatch_udp_appcall (void)
{
  switch ( HTONS(uip_udp_conn->rport))
   {
     case DHCPC_SERVER_PORT :  dhcpc_appcall();  break ;
     case SNTP_SERVER_PORT  :  sntp_appcall ();  break ;
     default: {  }
   }
}
//----------------------------------------------------------------------------------------------------
static void uipAutoSNTPTimeSynced (time_t *epochSeconds)
{
 if (*epochSeconds)
    {
      n32_t to;
      uip_gettimeoffset (&to);

      // здесь необходимо разместить код установка часов реального времени

      /*
      *epochSeconds += to;
      rtcSetEpochSeconds (*epochSeconds);
      */
  }
}
//----------------------------------------------------------------------------------------------------
static void uipAutoSNTP (void)
{
  uip_ipaddr_t addr;

  uip_getsntpaddr (&addr);

  if (!uip_iszeroaddr (&addr))
    sntpSync (&addr, uipAutoSNTPTimeSynced);
}
//----------------------------------------------------------------------------------------------------
void dhcpc_configured (const dhcpcState_t *s)
{
	if (!s->ipaddr [0] && !s->ipaddr [1])
  {
    //printf ("Can't get address via DHCP and no static address configured, stopping uIP task\n");
		  // установка адреса платы, шлюза и маски
	uint16_t ipaddr[2];
	uip_ipaddr(ipaddr, 192,168,0,8);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192,168,0,1);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);
  }
  else 
  {
    //printf ("\nIP address via DHCP is %d.%d.%d.%d\n" PROMPT, uip_ipaddr1 (s->ipaddr), uip_ipaddr2 (s->ipaddr), uip_ipaddr3 (s->ipaddr), uip_ipaddr4 (s->ipaddr));
    //fflush (stdout);
    uip_sethostaddr (s->ipaddr);
    uip_setnetmask (s->netmask);
    uip_setdraddr (s->default_router);
    uip_setsntpaddr (s->sntpaddr);
    uip_settimeoffset (&s->timeoffset);
  }

    // инициализация HTTP сервера.
    httpd_init();
    // инициализация telnet сервера
    telnetd_init();
    uipAutoSNTP ();

}
//----------------------------------------------------------------------
void uip_startup()
{
  clock_init();
  // инициализация uIP TCP/IP стека.
  uip_init();
  uip_arp_init ();
  #if (USE_DHCP==1)
     uip_ethaddr.addr[0]=0x12 ;
     uip_ethaddr.addr[1]=0x07 ;
     uip_ethaddr.addr[2]=0xAA ;
     uip_ethaddr.addr[3]=0x99 ;
     uip_ethaddr.addr[4]=0x12 ;
     uip_ethaddr.addr[5]=0x55 ;
     dhcpc_init (uip_ethaddr.addr, sizeof (uip_ethaddr.addr));
     dhcpc_request ();
  #else
	 // установка адреса платы, шлюза и маски

	 uint16_t ipaddr[2];
	 uip_ipaddr(ipaddr, 192,168,0,197);
	 uip_sethostaddr(ipaddr);
	 uip_ipaddr(ipaddr, 192,168,0,1);
	 uip_setdraddr(ipaddr);
	 uip_ipaddr(ipaddr, 255,255,255,0);
	 uip_setnetmask(ipaddr);

	 // инициализация HTTP сервера.
	 httpd_init();
     // инициализация telnet сервера
     telnetd_init();
     uipAutoSNTP ();
  #endif
}
