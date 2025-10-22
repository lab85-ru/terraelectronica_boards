#include "LCD.h"
#include "LPC2478.h"
#include "Config.h"

static uint32_t _lcd_buffer = LCD_BUFFER0_BASE_ADDR;

static inline void _draw_point(int32_t x, int32_t y, uint16_t colour){
  if((x < 0) || (x >= 640) || (y < 0) || (y >= 480)){
    return;
  }
  x = 640 - x;
  *(uint16_t *)(_lcd_buffer + (x + y * 640) * 2) = colour;
}

void LCD_Initialize(){
  LCD_CFG = 0 << LCD_CFG_CLKDIV;
  LCD_TIMH = (134 << LCD_TIMH_HBP) | (16 << LCD_TIMH_HFP) | 
    (10 << LCD_TIMH_HSW) | ((640 / 16 - 1) << LCD_TIMH_PPL);
  LCD_TIMV = (11 << LCD_TIMV_VBP) | (32 << LCD_TIMV_VFP) |
    (2 << LCD_TIMV_VSW) | (480 << LCD_TIMV_LPP);
  LCD_POL = (0 << LCD_POL_PCD_HI) | (0 << LCD_POL_BCD) |
    ((640 / 1 - 1) << LCD_POL_CPL) | (0 << LCD_POL_IOE) | (0 << LCD_POL_IPC) | 
    (1 << LCD_POL_IHS) | (1 << LCD_POL_IVS) | (0 << LCD_POL_ACB) | 
    (0 << LCD_POL_CLKSEL) | (2 << LCD_POL_PCD_LO);
  LCD_CTRL = (0 << LCD_CTRL_WATERMARK) | (0 << LCD_CTRL_LcdVComp) |
    (0 << LCD_CTRL_LcdPwr) | (0 << LCD_CTRL_BEPO) | (0 << LCD_CTRL_BEBO) | 
    (1 << LCD_CTRL_BGR) | (0 << LCD_CTRL_LcdDual) | (0 << LCD_CTRL_LcdMono8) |
    (1 << LCD_CTRL_LcdTFT) | (0 << LCD_CTRL_LcdBW) | (6 << LCD_CTRL_LcdBpp) | 
    (0 << LCD_CTRL_LcdEn);
  LCD_UPBASE = _lcd_buffer;
  LCD_LPBASE = _lcd_buffer;
  volatile uint32_t i = 0;
  for(i = 0; i < 100000; i++);
  LCD_CTRL |= (1 << LCD_CTRL_LcdEn);
  for(i = 0; i < 100000; i++);
  LCD_CTRL |= (1 << LCD_CTRL_LcdPwr);
}

void LCD_Finalize(){
}

void LCD_Set(uint16_t colour){
  register uint16_t *ptr = (uint16_t *)_lcd_buffer;
  uint16_t *end = (uint16_t *)(_lcd_buffer + (640 * 480) * 2);
  while(ptr < end){
    *ptr++ = colour;
  }
}

void LCD_DrawPoint(int32_t x, int32_t y, uint16_t colour){
  _draw_point(x, y, colour);
}

void LCD_FillRegion(int32_t x, int32_t y, uint32_t width, uint32_t height, 
uint16_t colour){
  int32_t xi, yi;
  for(yi = y; yi < y + height; yi++){
    for(xi = x; xi < x + width; xi++){
      _draw_point(xi, yi, colour);
    }
  }
}

