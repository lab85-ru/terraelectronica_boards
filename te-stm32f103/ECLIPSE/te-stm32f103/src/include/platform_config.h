// файл конфигурации платформы
// описывает особенности платы
// схему сопряжения выводов МК и компоненов платы и тд.
// например описывается распиновка портов

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

   #define USB_DISCONNECT_PORT                      GPIOB
   #define USB_DISCONNECT_PIN                       GPIO_Pin_5
   #define USB_DISCONNECT_RCC_APB2Periph_GPIO       RCC_APB2Periph_GPIOB

   #define DEBUG_LED_PORT                           GPIOA
   #define DEBUG_LED_PIN_D10                        GPIO_Pin_4
   #define DEBUG_LED_PIN_D11                        GPIO_Pin_5
   #define DEBUG_LED_PIN_D12                        GPIO_Pin_6
   #define DEBUG_LED_GPIO_MODE                      GPIO_Mode_Out_PP
   #define DEBUG_LED_RCC_APB2Periph_GPIO            RCC_APB2Periph_GPIOA

   #define SDIO_CARD_POWER_PIN                      GPIO_Pin_0
   #define SDIO_CARD_DETECT_PIN                     GPIO_Pin_1
   #define SDIO_CARD_POWER_DETECT_PORT              GPIOB
   #define SDIO_CARD_POWER_DETECT_APB2Periph_GPIO   RCC_APB2Periph_GPIOB

  #define CAN_RCC_APB2Periph_GPIO                   RCC_APB2Periph_GPIOB
  #define CAN_GPIO_Remap                            GPIO_Remap1_CAN1
  #define CAN_GPIO_PORT                             GPIOB
  #define CAN_GPIO_RX_PIN                           GPIO_Pin_8
  #define CAN_GPIO_TX_PIN                           GPIO_Pin_9



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
