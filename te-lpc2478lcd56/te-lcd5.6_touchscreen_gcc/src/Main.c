#include "Config.h"
#include "SDRAM.h"
#include "LCD.h"
#include "Touchscreen.h"

static void init();

int main(){
  init();
  TouchscreenCalibration();
  LCD_Set(WHITE);
  int32_t x = -1, y = -1;
  while(true){
    if(TouchscreenPressed()){
      int32_t new_x = TouchscreenX();
      int32_t new_y = TouchscreenY();
      if(TouchscreenPressed()){
        LCD_FillRegion(x - 1, y - 1, 3, 3, WHITE); // Стираем старую точку
        x = new_x;
        y = new_y;
        LCD_FillRegion(x - 1, y - 1, 3, 3, BLACK); // Рисуем новую точку
      }
    }
  }
}

static void ClockInitialize();
static void PinsInitialize();

static void init(){
  PCONP = _BV(PCONP_PCTIM0) | _BV(PCONP_PCEMC) | _BV(PCONP_PCAD) | 
  _BV(PCONP_PCLCD);
  MAMTIM = MAM_FETCH;
  MAMCR = MAM_MODE;
  MEMMAP = MEMMAP_VAL;
  SCS |= _BV(SCS_GPIOM);
  ClockInitialize();
  PinsInitialize();
  EMCControl = 0x00000001;
  SDRAM_Initialize();
  LCD_Initialize();
  TouchscreenInitialize();
}

static inline void PLLFeed(){
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
}

static void ClockInitialize(){
  // Отключаем PLL
  if(bit_is_set(PLLSTAT, PLLSTAT_PLLC)){
    PLLCON = _BV(PLLCON_PLLE);
    PLLFeed();
  }
  PLLCON = 0;
  PLLFeed();
  // Выбираем в качестве источника Main Osc
  SCS |= _BV(SCS_OSCEN);
  while(!(SCS & _BV(SCS_OSCSTAT)));
  CLKSRCSEL = 0x01;
  // Устанавливаем значения M и N
  PLLCFG = (MSEL_VAL << PLLCFG_MSEL) | (NSEL_VAL << PLLCFG_NSEL);
  PLLFeed();
  PLLCON = _BV(PLLCON_PLLE);
  PLLFeed();
  // Устанавливаем значения делителя частоты
  CCLKCFG = CCLKCFG_VAL;
  // Ждем пока значения будут применены
  while(bit_is_set(PLLSTAT, PLLSTAT_PLOCK));
  // Подключаем PLL
  PLLCON = _BV(PLLCON_PLLE) | _BV(PLLCON_PLLC);
  PLLFeed();
}

static void PinsInitialize(){
  PINSEL0 = 0x03055550;
  PINSEL1 = 0x00000000;
  PINSEL2 = 0x0280A8A0;
  PINSEL3 = 0xC5555500;
  PINSEL4 = 0x050FFFFF;
  PINSEL5 = 0x05F13115;
  PINSEL6 = 0x55555555;
  PINSEL8 = 0x55555555;
  PINSEL9 = 0x1A055555;
  PINSEL11 = 0x0000000B; // TFT 16-bit (5:6:5)
}

