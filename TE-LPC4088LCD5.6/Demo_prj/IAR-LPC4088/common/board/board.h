/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2012
 **
 **    $Revision: 57747 $
 **
 ***************************************************************************/
#include <intrinsics.h>

#ifndef __BOARD_H
#define __BOARD_H

#if defined(IAR_LPC_4088_SK)

/*UART DEF*/
#define UART_TX_MASK    (1UL<<2)
#define UART_TX_DIR     LPC_GPIO0->DIR
#define UART_TX_IOCON   LPC_IOCON->P0_2

#define UART_RX_MASK    (1UL<<3)
#define UART_RX_DIR     LPC_GPIO0->DIR
#define UART_RX_IOCON   LPC_IOCON->P0_3

/* USB OTG Link LED */
#define USB_OTG_LINK_LED_MASK   (1UL<<18)
#define USB_OTG_LINK_LED_FDIR   LPC_GPIO1->DIR
#define USB_OTG_LINK_LED_FSET   LPC_GPIO1->SET
#define USB_OTG_LINK_LED_FCLR   LPC_GPIO1->CLR
#define USB_OTG_LINK_LED_FIO    LPC_GPIO1->PIN
#define USB_OTG_LINK_LED_IOCON  LPC_IOCON->P1_18
/* USB Host Link LED */
#define USB_H_LINK_LED_MASK   (1UL<<13)
#define USB_H_LINK_LED_FDIR   LPC_GPIO0->DIR
#define USB_H_LINK_LED_FSET   LPC_GPIO0->SET
#define USB_H_LINK_LED_FCLR   LPC_GPIO0->CLR
#define USB_H_LINK_LED_FIO    LPC_GPIO0->PIN
#define USB_H_LINK_LED_IOCON  LPC_IOCON->P0_13
/*USB D+ D- and I2C*/
#define USB_DP_IOCON          LPC_IOCON->P0_29
#define USB_DM_IOCON          LPC_IOCON->P0_30
#define USB_SDA_IOCON         LPC_IOCON->P0_27
#define USB_SCL_IOCON         LPC_IOCON->P0_28
/*USB VBUS*/
#define USB_VBUS_MASK         (1UL<<14)
#define USB_VBUS_FDIR         LPC_GPIO0->DIR
#define USB_VBUS_FSET         LPC_GPIO0->SET
#define USB_VBUS_FCLR         LPC_GPIO0->CLR
#define USB_VBUS_FIO          LPC_GPIO0->PIN
#define USB_VBUS_IOCON        LPC_IOCON->P0_14
/*USB #PPWR2*/
#define USB_PPWR2_MASK         (1UL<<12)
#define USB_PPWR2_FDIR         LPC_GPIO0->DIR
#define USB_PPWR2_FSET         LPC_GPIO0->SET
#define USB_PPWR2_FCLR         LPC_GPIO0->CLR
#define USB_PPWR2_FIO          LPC_GPIO0->PIN
#define USB_PPWR2_IOCON        LPC_IOCON->P0_12
/*USB #OVRCR2*/
#define USB_OVRCR2_MASK         (1UL<<31)
#define USB_OVRCR2_FDIR         LPC_GPIO1->DIR
#define USB_OVRCR2_FSET         LPC_GPIO1->SET
#define USB_OVRCR2_FCLR         LPC_GPIO1->CLR
#define USB_OVRCR2_FIO          LPC_GPIO1->PIN
#define USB_OVRCR2_IOCON        LPC_IOCON->P1_31
/*USB #PWRD2*/
#define USB_PWRD2_MASK         (1UL<<30)
#define USB_PWRD2_FDIR         LPC_GPIO1->DIR
#define USB_PWRD2_FSET         LPC_GPIO1->SET
#define USB_PWRD2_FCLR         LPC_GPIO1->CLR
#define USB_PWRD2_FIO          LPC_GPIO1->PIN
#define USB_PWRD2_IOCON        LPC_IOCON->P1_30

/* MMC/SD card*/
#define SD_CMD_IOCON        LPC_IOCON->P1_3
#define SD_CLK_IOCON        LPC_IOCON->P1_2
#define SD_DAT_0_IOCON      LPC_IOCON->P1_6
#define SD_DAT_1_IOCON      LPC_IOCON->P1_7
#define SD_DAT_2_IOCON      LPC_IOCON->P1_11
#define SD_DAT_3_IOCON      LPC_IOCON->P1_12
/* Card power*/
#define SD_PWR_MASK         (1UL << 5)
#define SD_PWR_FDIR         LPC_GPIO1->DIR
#define SD_PWR_FSET         LPC_GPIO1->SET
#define SD_PWR_FCLR         LPC_GPIO1->CLR
#define SD_PWR_IOCON        LPC_IOCON->P1_5

/* Card present */
#define SD_CP_MASK          (1UL << 13)
#define SD_CP_FDIR          LPC_GPIO1->DIR
#define SD_CP_FIO           LPC_GPIO1->PIN
#define SD_CP_IOCON         LPC_IOCON->P1_13

/*CAN*/
#define CAN_RD_MASK     (1UL << 0)
#define CAN_RD_FDIR     LPC_GPIO0->DIR
#define CAN_RD_FIO      LPC_GPIO0->PIN
#define CAN_RD_IOCON    LPC_IOCON->P0_0

#define CAN_TD_MASK     (1UL << 1)
#define CAN_TD_FDIR     LPC_GPIO0->DIR
#define CAN_TD_FSET     LPC_GPIO0->SET
#define CAN_TD_FCLR     LPC_GPIO0->CLR
#define CAN_TD_FIO      LPC_GPIO0->PIN
#define CAN_TD_IOCON    LPC_IOCON->P0_1


/*LCD*/
#define  LCD0_IOCON     LPC_IOCON->P0_4
#define  LCD1_IOCON     LPC_IOCON->P0_5
#define  LCD2_IOCON     LPC_IOCON->P4_28
#define  LCD3_IOCON     LPC_IOCON->P4_29
#define  LCD4_IOCON     LPC_IOCON->P2_6
#define  LCD5_IOCON     LPC_IOCON->P2_7
#define  LCD6_IOCON     LPC_IOCON->P2_8
#define  LCD7_IOCON     LPC_IOCON->P2_9

#define  LCD8_IOCON     LPC_IOCON->P0_6
#define  LCD9_IOCON     LPC_IOCON->P0_7
#define  LCD10_IOCON    LPC_IOCON->P1_20
#define  LCD11_IOCON    LPC_IOCON->P1_21
#define  LCD12_IOCON    LPC_IOCON->P1_22
#define  LCD13_IOCON    LPC_IOCON->P1_23
#define  LCD14_IOCON    LPC_IOCON->P1_24
#define  LCD15_IOCON    LPC_IOCON->P1_25

#define  LCD16_IOCON    LPC_IOCON->P0_8
#define  LCD17_IOCON    LPC_IOCON->P0_9
#define  LCD18_IOCON    LPC_IOCON->P2_12
#define  LCD19_IOCON    LPC_IOCON->P2_13
#define  LCD20_IOCON    LPC_IOCON->P1_26
#define  LCD21_IOCON    LPC_IOCON->P1_27
#define  LCD22_IOCON    LPC_IOCON->P1_28
#define  LCD23_IOCON    LPC_IOCON->P1_29
#define  LCD_DCLK_IOCON LPC_IOCON->P2_2
#define  LCD_VS_IOCON LPC_IOCON->P2_3
#define  LCD_ENAB_IOCON LPC_IOCON->P2_4
#define  LCD_HS_IOCON LPC_IOCON->P2_5

//#define  LCD_PWR_IOCON  LPC_IOCON->P2_0
#define  LCD_BL_E_MASK  (1<<0)
#define  LCD_BL_E_FIO   LPC_GPIO2->PIN
#define  LCD_BL_E_FDIR  LPC_GPIO2->DIR
#define  LCD_BL_E_FSET  LPC_GPIO2->SET
#define  LCD_BL_E_FCLR  LPC_GPIO2->CLR
#define  LCD_BL_E_IOCON LPC_IOCON->P2_0
#define  LCD_BL_AJ_MASK  (1<<1)
#define  LCD_BL_AJ_FIO   LPC_GPIO2->PIN
#define  LCD_BL_AJ_FDIR  LPC_GPIO2->DIR
#define  LCD_BL_AJ_FSET  LPC_GPIO2->SET
#define  LCD_BL_AJ_FCLR  LPC_GPIO2->CLR
#define  LCD_BL_AJ_IOCON LPC_IOCON->P2_1

/*Touch screen*/
#define TS_X1_MASK          (1UL << 23)
#define TS_X1_FIO           LPC_GPIO0->PIN
#define TS_X1_FDIR          LPC_GPIO0->DIR
#define TS_X1_FSET          LPC_GPIO0->SET
#define TS_X1_FCLR          LPC_GPIO0->CLR
#define TS_X1_INTR_R        LPC_GPIOINT->IO0IntEnR
#define TS_X1_INTR_CLR      LPC_GPIOINT->IO0IntClr
#define TS_X1_IOCON         LPC_IOCON->P0_23

#define TS_X2_MASK          (1UL << 24)
#define TS_X2_FIO           LPC_GPIO0->PIN
#define TS_X2_FDIR          LPC_GPIO0->DIR
#define TS_X2_FSET          LPC_GPIO0->SET
#define TS_X2_FCLR          LPC_GPIO0->CLR
#define TS_X2_INTR_R        LPC_GPIOINT->IO0IntEnR
#define TS_X2_INTR_CLR      LPC_GPIOINT->IO0IntClr
#define TS_X2_IOCON         LPC_IOCON->P0_24

#define TS_Y1_MASK          (1UL << 25)
#define TS_Y1_FIO           LPC_GPIO0->PIN
#define TS_Y1_FDIR          LPC_GPIO0->DIR
#define TS_Y1_FSET          LPC_GPIO0->SET
#define TS_Y1_FCLR          LPC_GPIO0->CLR
#define TS_Y1_INTR_R        LPC_GPIOINT->IO0IntEnR
#define TS_Y1_INTR_CLR      LPC_GPIOINT->IO0IntClr
#define TS_Y1_IOCON         LPC_IOCON->P0_25

#define TS_Y2_MASK          (1UL << 26)
#define TS_Y2_FIO           LPC_GPIO0->PIN
#define TS_Y2_FDIR          LPC_GPIO0->DIR
#define TS_Y2_FSET          LPC_GPIO0->SET
#define TS_Y2_FCLR          LPC_GPIO0->CLR
#define TS_Y2_INTR_R        LPC_GPIOINT->IO0IntEnR
#define TS_Y2_INTR_CLR      LPC_GPIOINT->IO0IntClr
#define TS_Y2_IOCON         LPC_IOCON->P0_26

/*Ethernet*/
#define ETH_TX0_IOCON     LPC_IOCON->P1_0
#define ETH_TX1_IOCON     LPC_IOCON->P1_1
#define ETH_TX_EN_IOCON   LPC_IOCON->P1_4
#define ETH_CRS_IOCON     LPC_IOCON->P1_8
#define ETH_RX0_IOCON     LPC_IOCON->P1_9
#define ETH_RX1_IOCON     LPC_IOCON->P1_10
#define ETH_RX_ER_IOCON   LPC_IOCON->P1_14
#define ETH_REF_CLK_IOCON LPC_IOCON->P1_15
#define ETH_MDC_IOCON     LPC_IOCON->P1_16
#define ETH_MDIO_IOCON    LPC_IOCON->P1_17

#else  
#error define type of the board
#endif  /*endif IAR_LPC_4088_SK*/


#endif /* __BOARD_H */
