#include "LPC2478.h"

void LCD_Initialize(){
  LCD_CFG = 0 << LCD_CFG_CLKDIV;
  LCD_TIMH = (134 << LCD_TIMH_HBP) | (16 << LCD_TIMH_HFP) | 
             (10 << LCD_TIMH_HSW) | ((640 / 16 - 1) << LCD_TIMH_PPL);
  LCD_TIMV = (11 << LCD_TIMV_VBP) | (32 << LCD_TIMV_VFP) |
             (2 << LCD_TIMV_VSW) | (480 << LCD_TIMV_LPP);
  LCD_POL = (0 << LCD_POL_PCD_HI) | (0 << LCD_POL_BCD) |
            ((640 / 1 - 1) << LCD_POL_CPL) | (0 << LCD_POL_IOE) |
            (0 << LCD_POL_IPC) | (1 << LCD_POL_IHS) | (1 << LCD_POL_IVS) |
            (0 << LCD_POL_ACB) | (0 << LCD_POL_CLKSEL) | 
            (2 << LCD_POL_PCD_LO);
  LCD_CTRL = (0 << LCD_CTRL_WATERMARK) | (0 << LCD_CTRL_LcdVComp) |
             (0 << LCD_CTRL_LcdPwr) | (0 << LCD_CTRL_BEPO) |
             (0 << LCD_CTRL_BEBO) | (1 << LCD_CTRL_BGR) |
             (0 << LCD_CTRL_LcdDual) | (0 << LCD_CTRL_LcdMono8) |
             (1 << LCD_CTRL_LcdTFT) | (0 << LCD_CTRL_LcdBW) |
             (4 << LCD_CTRL_LcdBpp) | (0 << LCD_CTRL_LcdEn);
  LCD_UPBASE = LCD_BUFFER0_BASE_ADDR;
  LCD_LPBASE = LCD_BUFFER0_BASE_ADDR;
  uint32_t i = 0;
  for(i = 0; i < 100000; i++);
  LCD_CTRL |= (1 << LCD_CTRL_LcdEn);
  for(i = 0; i < 100000; i++);
  LCD_CTRL |= (1 << LCD_CTRL_LcdPwr);
}

void LCD_Finalize(){
  LCD_CTRL &= ~(1 << LCD_CTRL_LcdPwr);
  LCD_CTRL &= ~(1 << LCD_CTRL_LcdEn);
}

