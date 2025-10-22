/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : uip_webserver.c
 *    Description : IAR-LPC4088-SK uIP Web Server Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-4088-SK board. This demo shows a web server
 * application  running on the top of the uIP 1.0 TCP-IP stack.
 *
 * The default IP address is (defined in uipopt.h):
 *   192.168.0.100 
 * The physical MAC address is (defined in uipopt.h):
 *   00-ff-ff-ff-ff-fftion sensor is used).
 * 
 *
 *    $Revision: 59014 $
 **************************************************************************/
#include <yfuns.h>
#include <stdint.h>

#include "LPC407x_8x.h"
#include "board.h"
#include "sdram_64M_32bit_drv.h"
#include "drv_glcd.h"

#include "LPC40xx_enet.h"
#include "uip_arp.h"
#include "timer.h"

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

volatile uint32_t Ticks;

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
/**/
/** external data **/
#pragma section=".intvec"
/** public functions **/
/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: void
 * Return: 
 *
 * Description: Low level system init (clock, flash accelerator, 
 *              SDRAM and vector table address)
 *              
 *
 *************************************************************************/
int __low_level_init (void)
{
  /*Here the clock is already set by
    SystemInit function*/
  
  /*SDRAM init*/
  SDRAM_Init();

  return  1;
}

/*************************************************************************
 * Function Name: TMR0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void TIMER0_IRQHandler (void)
{
  ++Ticks;
  /*clear match channel 0 interrupt */
  LPC_TIM0->IR = (1<<0);
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: TMR0_Init
 * Parameters: uint32_t tps - Ticks per second
 *
 * Return: none
 *
 * Description: Timer 0 start
 *
 *************************************************************************/
void TMR0_Init(uint32_t tps)
{
  Ticks = 0;
  /* Enable TIM0 clock*/
  LPC_SC->PCONP |= (1<<1);

  /*Init Timer 0 */
  LPC_TIM0->TCR = (1<<1);       /* counting  disable, set reset */
  LPC_TIM0->TCR = 0;            /* release reset */
  LPC_TIM0->CTCR = 0;           /* Timer Mode: every rising PCLK edge */
  LPC_TIM0->MCR = (1<<0) |      /* Enable Interrupt on MR0*/
                  (1<<1) |      /* Enable reset on MR0 */
                  (0<<2) ;      /* Disable stop on MR0 */
  /*set timer 0 period*/
  LPC_TIM0->PR = 0;
  LPC_TIM0->MR0 = PeripheralClock/(tps);
  /* init timer 0 interrupt */
  LPC_TIM0->IR = (1<<0);        /* clear pending interrupt */
  LPC_TIM0->TCR = (1<<0);       /* counting Enable */
  /*Enable NVIC TMR0 Interrupt*/
  NVIC_EnableIRQ(TIMER0_IRQn);
}

/*************************************************************************
 * Function Name: TMR0_Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 stop
 *
 *************************************************************************/
void TMR0_Stop(void)
{
  /* counting  disable */
  LPC_TIM0->TCR &= ~(1<<0);
  /* Enable NVIC TMR0 Interrupt */
  NVIC_DisableIRQ(TIMER0_IRQn);
  /* Timer 0 Clock disable */
  LPC_SC->PCONP &= ~(1<<1);
}

#define BMP_IMG_ADDR  0x28000000
/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
//pPic_t pPic; 
unsigned int i;
uip_ipaddr_t ipaddr;
struct timer periodic_timer, arp_timer;
  /*Disable interrupts*/
  __disable_interrupt();
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Init LCD Controller*/
  GLCD_Init (NULL, NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /* Sys timer init 1/100 sec tick */
  clock_init(2);

  timer_set(&periodic_timer, CLOCK_SECOND / 2);
  timer_set(&arp_timer, CLOCK_SECOND * 10);
  
  /*Enable interrupts*/
  __enable_interrupt();

  
  GLCD_SetFont(&Terminal_18_24_12,0x00FF00FF,0x0);
  GLCD_SetWindow(0,0,639,479);
  GLCD_TextSetPos(0,0);
  GLCD_print("                 uIP WEB SERVER DEMO\r\n");
  GLCD_SetFont(&Terminal_18_24_12,0x00FFFF00,0x0);

  //* Initialize the ethernet device driver*/
  GLCD_print("Initialize the ethernet device driver ...\r\n");
   while(!tapdev_init());

  GLCD_print("PHY ok.\r\n");

 
  /* uIP web server
     Initialize the uIP TCP/IP stack.*/
  GLCD_print("uIP TCP/IP stack initialize ...\r\n");
  uip_init();
  GLCD_print("uIP TCP/IP stack ok.\r\n");

  
  uip_ipaddr(ipaddr, UIP_IPADDR0,UIP_IPADDR1,UIP_IPADDR2,UIP_IPADDR3);
  uip_sethostaddr(ipaddr);
  GLCD_print("    IP addres - %d.%d.%d.%d\r\n",UIP_IPADDR0,UIP_IPADDR1,UIP_IPADDR2,UIP_IPADDR3);
  uip_ipaddr(ipaddr, UIP_DRIPADDR0,UIP_DRIPADDR1,UIP_DRIPADDR2,UIP_DRIPADDR3);
  uip_setdraddr(ipaddr);
  uip_ipaddr(ipaddr, UIP_NETMASK0,UIP_NETMASK1,UIP_NETMASK2,UIP_NETMASK3);
  uip_setnetmask(ipaddr);
  GLCD_print("    IP netmask - %d.%d.%d.%d\r\n",UIP_NETMASK0,UIP_NETMASK1,UIP_NETMASK2,UIP_NETMASK3);
  GLCD_print("    IP router - %d.%d.%d.%d\r\n",UIP_DRIPADDR0,UIP_DRIPADDR1,UIP_DRIPADDR2,UIP_DRIPADDR3);
  
  /* Initialize the HTTP server. */
  GLCD_print("HTTP Server init...\r\n");
  httpd_init();
  GLCD_print("HTTP Server run.\r\n");

  while(1)
  {
    uip_len = tapdev_read(uip_buf);
    if(uip_len > 0)
    {
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
	        tapdev_send(uip_buf,uip_len);
	      }
      }
      else if(BUF->type == htons(UIP_ETHTYPE_ARP))
      {
        uip_arp_arpin();
	      /* If the above function invocation resulted in data that
	         should be sent out on the network, the global variable
	         uip_len is set to a value > 0. */
	      if(uip_len > 0)
        {
	        tapdev_send(uip_buf,uip_len);
	      }
      }
    }
    else if(timer_expired(&periodic_timer))
    {
      timer_reset(&periodic_timer);
      for(i = 0; i < UIP_CONNS; i++)
      {
      	uip_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0)
        {
          uip_arp_out();
          tapdev_send(uip_buf,uip_len);
        }
      }
#if UIP_UDP
      for(i = 0; i < UIP_UDP_CONNS; i++) {
        uip_udp_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0) {
          uip_arp_out();
          tapdev_send();
        }
      }
#endif /* UIP_UDP */
      /* Call the ARP timer function every 10 seconds. */
      if(timer_expired(&arp_timer))
      {
        timer_reset(&arp_timer);
        uip_arp_timer();
      }
    }
  }
}
/** private functions **/
