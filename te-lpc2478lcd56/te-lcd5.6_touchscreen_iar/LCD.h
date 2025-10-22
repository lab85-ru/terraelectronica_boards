#ifndef LCD_H
#define LCD_H
#include <stdbool.h>
#include <stdint.h>

#define WHITE (0xFFFF)
#define BLACK (0x0000)

void LCD_Initialize();
void LCD_Finalize();
void LCD_Set(uint16_t colour);
void LCD_DrawPoint(int32_t x, int32_t y, uint16_t colour);
void LCD_FillRegion(int32_t x, int32_t y, uint32_t width, uint32_t height, 
  uint16_t colour);

#endif // LCD_H

