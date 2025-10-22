#include "Config.h"
#include "LCD.h"
#include "ADS1241.h"
#include "Messages.h"
#include <stdio.h>

static bool LoadFonts();
static void DrawFrame();
static void DrawValues();
static void EraseBackground();

extern const uint8_t font16[], font20[], font30[], font60[];
Font fnt16, fnt20, fnt30, fnt60;
float current = 0.0, max = 0.0, min = 0.0, average = 0.0;
char buffer[32];

int main(){
  bool dofirst = true;
  if(!LoadFonts()) while(true);
  DrawFrame();
  DrawValues();
  WREG(0x00, 0x00); // Ku = 1
  WREG(0x01, (0 << 4) | (1 << 0)); // 1 канал
  while(true){
    if(DRDY()){
      current = ((float)RDATA() * 5000000.0 / (1.0 * 8388608.0));
      if(dofirst){
        max = current;
        min = current;
        dofirst = false;
      }
      else {
        if(current > max){
          max = current;
        }
        if(current < min){
          min = current;
        }
      }
      average = max - min;
      EraseBackground();
      DrawValues();
    }
  }
}

static bool LoadFonts(){
  if(!LoadFont(&fnt16, font16)) return false;
  if(!LoadFont(&fnt20, font20)) return false;
  if(!LoadFont(&fnt30, font30)) return false;
  if(!LoadFont(&fnt60, font60)) return false;
  return true;
}

static void DrawFrame(){
  RegionFill(0, 0, LCD_WIDTH, LCD_HEIGHT, 0xFFFF);
  DrawString(124, 400, &fnt60, MSG_ADS1241, 0x0000);
  DrawString(57, 370, &fnt16, MSG_DESCRIPTION, 0x0000);
  DrawLine(40, 355, LCD_WIDTH - 40, 355, 0x0000);
  DrawString(242, 300, &fnt30, MSG_CAPTION, 0x0000); // 155
  DrawString(170, 220, &fnt20, MSG_CURRENT ":", 0x0000); // 94
  DrawString(170, 190, &fnt20, MSG_MAX ":", 0x0000); // 159
  DrawString(170, 160, &fnt20, MSG_MIN ":", 0x0000); // 152
  DrawString(170, 130, &fnt20, MSG_AVERAGE ":", 0x0000); // 131
  DrawLine(40, 40, LCD_WIDTH - 40, 40, 0x0000);
}

static void DrawValues(){
  snprintf(buffer, sizeof(buffer), "%.2f " MSG_UV, current);
  DrawString(350, 220, &fnt20, buffer, 0x0000); //
  snprintf(buffer, sizeof(buffer), "%.2f " MSG_UV, max);
  DrawString(350, 190, &fnt20, buffer, 0x0000); //
  snprintf(buffer, sizeof(buffer), "%.2f " MSG_UV, min);
  DrawString(350, 160, &fnt20, buffer, 0x0000); //
  snprintf(buffer, sizeof(buffer), "%.2f " MSG_UV, average);
  DrawString(350, 130, &fnt20, buffer, 0x0000); //
}

static void EraseBackground(){
  RegionFill(350, 130, 290, 120, 0xFFFF);
}

