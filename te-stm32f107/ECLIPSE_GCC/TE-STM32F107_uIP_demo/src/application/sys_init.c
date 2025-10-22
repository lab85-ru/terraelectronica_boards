#include "appdefs.h"
#include "stm32_eth.h"
#include "stm32f10x_rcc.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DP83848_PHY        /* Ethernet pins mapped on STM3210C-EVAL Board */

#define MII_MODE

static void RCC_Configuration(void)
{
 RCC_ClocksTypeDef RCC_ClockFreq;

   /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  ErrorStatus HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus != ERROR)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

 /****************************************************************/
 /*      HSE=25MHz, HCLK=72MHz, PCLK2=72MHz, PCLK1=36MHz         */
 /****************************************************************/
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /*  ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* Configure PLLs *********************************************************/
    /* PPL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}

    /* PPL1 configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }

  RCC_GetClocksFreq(&RCC_ClockFreq);

  /* Enable ETHERNET clock  */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
                        RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);

  /* Enable GPIOs clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO, ENABLE);

}

//----------------------------------------------------------------------------------------
#define ETH_RXBUFNB        8
#define ETH_TXBUFNB        4
ETH_InitTypeDef ETH_InitStructure;
/* Ethernet Rx & Tx DMA Descriptors */
ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];
u8 Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];

static  void Ethernet_Configuration(void)
{
	  ETH_InitTypeDef ETH_InitStructure;

	  GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);

	  /* Reset ETHERNET on AHB Bus */
	  ETH_DeInit();

	  /* Software reset */
	  ETH_SoftwareReset();

	  /* Wait for software reset */
	  while(ETH_GetSoftwareResetStatus()==SET);

	  /* ETHERNET Configuration ------------------------------------------------------*/
	  /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
	  ETH_StructInit(&ETH_InitStructure);

	  /* Fill ETH_InitStructure parametrs */
	  /*------------------------   MAC   -----------------------------------*/
	  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable  ;
	  //ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
	  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	  //ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
	  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
	  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
	  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
	  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Disable;
	  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
	  /* Configure ETHERNET */
	  ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

	  /* Initialize Tx Descriptors list: Chain Mode */
	  ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	  /* Initialize Rx Descriptors list: Chain Mode  */
	  ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

	  /* Enable the Ethernet Rx Interrupt */
	  ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R , ENABLE);

	  /* Enable MAC and DMA transmission and reception */
	  ETH_Start();
}

//------------------------------------------------------------------------------------
static void GPIO_Configuration(void)
{

   //ETH_MII_CRS      PA0    Floating input (reset state)
   //ETH_MII_COL      PA3    Floating input (reset state)
   //ETH_MDIO MDIO    PA2    AF output push-pull highspeed(50 MHz)
   //ETH_MDC          PC1    AF output push-pull highspeed(50 MHz)

   //ETH_MII_TX_EN    PB11   AF output push-pull highspeed(50 MHz)
   //ETH_MII_TXD0     PB12   AF output push-pull highspeed(50 MHz)
   //ETH_MII_TXD1     PB13   AF output push-pull highspeed(50 MHz)
   //ETH_MII_TXD2     PC2    AF output push-pull highspeed(50 MHz)
   //ETH_MII_TXD3     PB8    AF output push-pull highspeed(50 MHz)
   //ETH_MII_TX_CLK   PC3    Floating input (reset state)

   //ETH_MII_RX_DV    PA7    Floating input (reset state)
   //ETH_MII_RX_ER    PB10   Floating input (reset state)
   //ETH_MII_RXD0     PC4    Floating input (reset state)
   //ETH_MII_RXD1     PC5    Floating input (reset state)
   //ETH_MII_RXD2     PB0    Floating input (reset state)
   //ETH_MII_RXD3     PB1    Floating input (reset state)
   //ETH_MII_RX_CLK   PA1    Floating input (reset state)

  GPIO_InitTypeDef GPIO_InitStructure;

  //GPIOA,AF,ppout,50
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //GPIOB,AF,ppout,50
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //GPIOC,AF,ppout,50
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //GPIOA,float input,50
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //GPIOB,float input,50
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //GPIOC,float input,50
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
//------------------------------------------------------------------------------------
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  
  /* Enable the Ethernet global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);    
  
}
//------------------------------------------------------------------------------------
void sys_init(void)
{
  RCC_ClocksTypeDef RCC_Clocks;

  // конфигурация clock, PLL, Flash
  SystemInit();

  RCC_Configuration();

  // конфигурация NVIC
  NVIC_Configuration();

  // конфигурация GPIO ports */
  GPIO_Configuration();

  /* конфигурация Ethernet  MAC */
  Ethernet_Configuration();

  /* SystTick configuration: an interrupt every 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);

  #if (USE_FREERTOS==0)
     SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  #endif

  /* Update the SysTick IRQ priority should be higher than the Ethernet IRQ */
  /* The Localtime should be updated during the Ethernet packets processing */
  NVIC_SetPriority (SysTick_IRQn, 1);

}
