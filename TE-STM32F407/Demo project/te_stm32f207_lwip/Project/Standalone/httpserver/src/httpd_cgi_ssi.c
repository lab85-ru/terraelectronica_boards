/**
  ******************************************************************************
  * @file    httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   Webserver SSI and CGI handlers
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "main.h"
#include "stm32f2xx_it.h"

#include <string.h>
#include <stdlib.h>

tSSIHandler ADC_Page_SSI_Handler;
uint32_t ADC_not_configured=1;

int led_on_count = 0;
int led_off_count = 0;
CanTxMsg CAN1TxMessage,CAN2TxMessage;

/* we will use character "a" as tag for new CGI */
char const* TAGCHAR_new="a";
char const* TAGCHAR="t";

char const *TAGS_new[2];


/* CGI handler for LED control */ 
const char * TRANSMIT_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char * LED_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/leds.cgi" will start LEDS_CGI_Handler */
const tCGI TRANSMIT_CGI={"/transmit.cgi", TRANSMIT_CGI_Handler};
const tCGI LED_CGI={"/led.cgi", LED_CGI_Handler};

/* Cgi call table */
tCGI CGI_TAB[2];

u16_t Show_Counters(int iIndex, char *pcInsert, int iInsertLen) 
{
  unsigned int result_lenght = 0; 
  float temp = 0;
  unsigned int adc_value = GetADCConvertedValue();
  int key;
  char *view_memory = pcInsert; 
  
  temp= (((3.3/4096)*((float)(adc_value >> 4)))-0.76)/(0.0025)+25;
  
  result_lenght += sprintf(pcInsert, "Temp sensor : %.1f C; Input Buffer : ",temp);
  pcInsert += result_lenght;

  do {
   key = USART3_GetKey();
    if (key != (-1)) {
      *pcInsert =(char )key;
      pcInsert++;
      result_lenght++;
      }	
  } while (key!=(-1));
   
  result_lenght += sprintf(pcInsert," <");
  return result_lenght;
  }


const char * LED_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  uint32_t i = 0;
  
  if (iIndex == 1) {
    if (strcmp(pcParam[i] , "ledon")==0) {
      led_on_count++;
      STM_EVAL_LEDToggle(LED1);
    }
    if (strcmp(pcParam[i] , "ledoff")==0) {
      led_off_count++;
      GPIO_ToggleBits(GPIOC, GPIO_Pin_13);
    }
  }

  return "/test.shtml"; 
}


/**
  * @brief  CGI handler for LEDs control 
  */
const char * TRANSMIT_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  uint32_t i=0;
  uint32_t count = 0;
  
  if (strcmp(pcParam[i] , "uart2")==0) 
    while (pcValue[i][count] != 0) {
      USART2_SendChar((int )pcValue[i][count]);
      count++;
   }
  
  if (strcmp(pcParam[i] , "uart3")==0) 
    while (pcValue[i][count] != 0) {
      USART3_SendChar((int )pcValue[i][count]);
      count++;
   }
  
  
  if (strcmp(pcParam[i] , "can1")==0) {
    for (count = 0;(pcValue[i][count] != 0);count++) {
	if (count<3) CAN1TxMessage.Data[count] = pcValue[i][count];
    }
    CAN1TxMessage.StdId = 0x321;
    CAN1TxMessage.ExtId = 0x01;
    CAN1TxMessage.RTR = CAN_RTR_DATA;
    CAN1TxMessage.IDE = CAN_ID_STD;
    CAN1TxMessage.DLC = count;

    CAN_Transmit(CAN1, &CAN1TxMessage);
  }
  
  if (strcmp(pcParam[i] , "can2")==0) {
    for (count = 0;(pcValue[i][count] != 0);count++) {
	if (count<3) CAN2TxMessage.Data[count] = pcValue[i][count];
    }
    CAN2TxMessage.StdId = 0x321;
    CAN2TxMessage.ExtId = 0x01;
    CAN2TxMessage.RTR = CAN_RTR_DATA;
    CAN2TxMessage.IDE = CAN_ID_STD;
    CAN2TxMessage.DLC = count;

    CAN_Transmit(CAN2, &CAN1TxMessage);
  }
  
  /* uri to send after cgi call*/
  return "/test.shtml";
}

/**
 * Initialize SSI handlers
 */
void httpd_ssi_init(void)
{  
  /* configure SSI handlers (ADC page SSI) */
  
  TAGS_new[0] = TAGCHAR_new;
  TAGS_new[1] = TAGCHAR;
    
  http_set_ssi_handler(Show_Counters, (char const **)TAGS_new, 2);
}

/**
 * Initialize CGI handlers
 */
void httpd_cgi_init(void)
{ 
  /* configure CGI handlers (LEDs control CGI) */
  CGI_TAB[0] = TRANSMIT_CGI;
  CGI_TAB[1] = LED_CGI;
  
  http_set_cgi_handlers(CGI_TAB, 2);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
