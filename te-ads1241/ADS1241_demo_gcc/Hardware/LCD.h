#ifndef LCD_H
#define LCD_H
#include "Config.h"

typedef struct {
  const uint8_t *data;
  uint16_t version;
  uint16_t type;
  uint16_t ascent;
  uint16_t width;
  uint16_t height;
  uint8_t first;
  uint8_t last;
  uint32_t offset;
} Font;

void LCD_Initialize();
void DrawPoint(int x, int y, COLOUR_TYPE colour);
void DrawLine(int x0, int y0, int xn, int yn, COLOUR_TYPE colour);
void RegionFill(int x0, int y0 , int width, int height, COLOUR_TYPE colour);
bool LoadFont(Font *font, const uint8_t *data);
uint16_t DrawChar(int x, int y, const Font *font, char c, COLOUR_TYPE colour);
uint16_t DrawString(int x, int y, const Font *font, const char *str, COLOUR_TYPE colour);

#endif // LCD_H

