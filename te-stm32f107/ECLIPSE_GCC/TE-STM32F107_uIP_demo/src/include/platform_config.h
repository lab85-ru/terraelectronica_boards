// файл конфигурации платформы
// описывает особенности платы
// схему сопряжения выводов МК и компоненов платы и тд.
// например описывается распиновка портов

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

   // адрес микросхемы PHY
   #define PHY_ADDRESS       0x01F

// ------------------------------------------------------------------*/

  // Модуль CAN1
  #define CAN1_RCC_APBPeriph_GPIO                   RCC_APB2Periph_GPIOD
  #define CAN1_RCC_APBPeriph_AFIO                   RCC_APB2Periph_AFIO
  #define CAN1_RCC_APBPeriph_CAN                    RCC_APB1Periph_CAN1
  #define CAN1_GPIO_Remap                           GPIO_Remap2_CAN1
  #define CAN1_GPIO_PORT                            GPIOD
  #define CAN1_RX_GPIO_PIN                          GPIO_Pin_0
  #define CAN1_TX_GPIO_PIN                          GPIO_Pin_1

  // Модуль CAN2
  #define CAN2_RCC_APBPeriph_GPIO                   RCC_APB2Periph_GPIOB
  #define CAN2_RCC_APBPeriph_AFIO                   RCC_APB2Periph_AFIO
  #define CAN2_RCC_APBPeriph_CAN                    RCC_APB1Periph_CAN2
  #define CAN2_GPIO_Remap                           GPIO_Remap_CAN2
  #define CAN2_GPIO_PORT                            GPIOB
  #define CAN2_RX_GPIO_PIN                          GPIO_Pin_5
  #define CAN2_TX_GPIO_PIN                          GPIO_Pin_6



// ------------------------------------------------------------*/
// ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

