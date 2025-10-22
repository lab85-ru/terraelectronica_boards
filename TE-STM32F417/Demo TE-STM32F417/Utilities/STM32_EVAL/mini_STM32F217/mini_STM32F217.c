#include "mini_STM32F217.h"

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK};

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  * @retval None
  */
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);


  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  * @retval None
  */
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  * @retval None
  */
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  * @retval None
  */
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}



void INIT_FSMC_pins(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
/*-- GPIOs Configuration -----------------------------------------------------*/
/*
+-------------------+--------------------+------------------+------------------+
+                             xxRAM pins assignment                            +
+-------------------+--------------------+------------------+------------------+
| PB7  <-> FSMC_NL  | PE0  <-> FSMC_NBL0 | PF0  <-> FSMC_A0 | PG0 <-> FSMC_A10 |
| PD0  <-> FSMC_D2  | PE1  <-> FSMC_NBL1 | PF1  <-> FSMC_A1 | PG1 <-> FSMC_A11 |
| PD1  <-> FSMC_D3  | PE3  <-> FSMC_A19  | PF2  <-> FSMC_A2 | PG2 <-> FSMC_A12 |
| PD3  <-> FSMC_CLK | PE4  <-> FSMC_A20  | PF3  <-> FSMC_A3 | PG3 <-> FSMC_A13 |
| PD4  <-> FSMC_NOE | PE5  <-> FSMC_A21  | PF4  <-> FSMC_A4 | PG4 <-> FSMC_A14 |
| PD5  <-> FSMC_NWE | PE6  <-> FSMC_A22  | PF5  <-> FSMC_A5 | PG5 <-> FSMC_A15 |
| PD6  <-> FSMC_NWA | PE7  <-> FSMC_D4   | PF12 <-> FSMC_A6 | PG6 <-> FSMC_INT2|
| PD7  <-> FSMC_NE1 | PE8  <-> FSMC_D5   | PF13 <-> FSMC_A7 | PG9 <-> FSMC_NCE3|
| PD8  <-> FSMC_D13 | PE9  <-> FSMC_D6   | PF14 <-> FSMC_A8 |                  |
| PD9  <-> FSMC_D14 | PE10 <-> FSMC_D7   | PF15 <-> FSMC_A9 |                  |
| PD10 <-> FSMC_D15 | PE11 <-> FSMC_D8   |                  |                  |
| PD11 <-> FSMC_A16 | PE12 <-> FSMC_D9   |                  |                  |
| PD12 <-> FSMC_A17 | PE13 <-> FSMC_D10  |                  |                  |
| PD13 <-> FSMC_A18 | PE14 <-> FSMC_D11  |                  |                  |
| PD14 <-> FSMC_D0  | PE15 <-> FSMC_D12  |                  |                  |
| PD15 <-> FSMC_D1  |                    |                  |                  |
+-------------------+--------------------+------------------+------------------+
*/

	/* Enable GPIOs clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD |
                               RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF |
		                         RCC_AHB1Periph_GPIOG, ENABLE);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	/* GPIOB configuration */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* GPIOD configuration */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 |
              GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8 |
              GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
              GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	
        GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* GPIOE configuration */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_3 |
                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6  | GPIO_Pin_7  |
                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* GPIOF configuration */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15 , GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2
                | GPIO_Pin_3 | GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_12 |
                  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* GPIOG configuration */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource2 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource3 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource6 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOG, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9 , GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2 |
              GPIO_Pin_3 | GPIO_Pin_4  | GPIO_Pin_5  |
              GPIO_Pin_9 ;

	GPIO_Init(GPIOG, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

        /* Enable FSMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
}


void INIT_FSMC_PSRAM(void )
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  PSRAMTiming;

  /*-- FSMC Configuration ------------------------------------------------------*/
  PSRAMTiming.FSMC_AddressSetupTime = 6;
  PSRAMTiming.FSMC_AddressHoldTime = 4;
  PSRAMTiming.FSMC_DataSetupTime = 6;
  PSRAMTiming.FSMC_BusTurnAroundDuration = 2;
  PSRAMTiming.FSMC_CLKDivision = 0;
  PSRAMTiming.FSMC_DataLatency = 0;
  PSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_PSRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &PSRAMTiming;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &PSRAMTiming;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void INIT_FSMC_NAND_bank3(void )
{
	FSMC_NANDInitTypeDef FSMC_NANDInitStructure;
	FSMC_NAND_PCCARDTimingInitTypeDef  NANDTiming;
	
	NANDTiming.FSMC_SetupTime = 0x4;
	NANDTiming.FSMC_WaitSetupTime = 0x12;
	NANDTiming.FSMC_HoldSetupTime = 0x8;
	NANDTiming.FSMC_HiZSetupTime = 0x4;

	FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank3_NAND;
	/* FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Enable; */
	FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Disable;
	FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes;
	FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x3;
	FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x3;
	FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &NANDTiming;
	FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &NANDTiming;

	FSMC_NANDInit(&FSMC_NANDInitStructure);
	FSMC_NANDCmd(FSMC_Bank3_NAND, ENABLE);
}
