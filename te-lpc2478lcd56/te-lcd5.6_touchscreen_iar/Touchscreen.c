#include "Touchscreen.h"
#include "Config.h"
#include "LCD.h"

#define AV (225)
static double A, B, C, D, E, F;

static inline void x1vcc(){
  X1SEL &= ~(3 << X1SELBIT); // GPIO
  X1DIR |= _BV(X1BIT); // OUT
  X1SET = _BV(X1BIT); // VCC
}

static inline void x1adc(){
  X1SEL |= (X1ADCSEL << X1SELBIT); // ADC
}

static inline void x1input(){
  X1SEL &= ~(3 << X1SELBIT); // GPIO
  X1MODE = (X1MODE & ~(3 << X1SELBIT)) | (0 << X1SELBIT); // PULL-UP
  X1DIR &= ~_BV(X1BIT); // INPUT
}

static inline void x2gnd(){
  X2SEL &= ~(3 << X2SELBIT); // GPIO
  X2DIR |= _BV(X2BIT); // OUT
  X2CLR = _BV(X2BIT); // GND
}

static inline void x2open(){
  X2SEL &= ~(3 << X2SELBIT); // GPIO
  X2DIR &= ~_BV(X2BIT); // INPUT
}

static inline void y1vcc(){
  Y1SEL &= ~(3 << Y1SELBIT); // GPIO
  Y1DIR |= _BV(Y1BIT); // OUT
  Y1SET = _BV(Y1BIT); // VCC
}

static inline void y1adc(){
  Y1SEL |= (Y1ADCSEL << Y1SELBIT); // ADC
}

static inline void y1open(){
  Y1SEL &= ~(3 << Y1SELBIT); // GPIO
  Y1DIR &= ~_BV(Y1BIT); // INPUT
}

static inline void y2gnd(){
  Y2SEL &= ~(3 << Y2SELBIT); // GPIO
  Y2DIR |= _BV(Y2BIT); // OUT
  Y2CLR = _BV(Y2BIT); // GND
}

static inline void y2open(){
  Y2SEL &= ~(3 << Y2SELBIT); // GPIO
  Y2DIR &= ~_BV(Y2BIT); // INPUT
}

void TouchscreenInitialize(){
  AD0CR = (1 << 21) | (1 << 16) | (4 << 8) | (1 << XADCNUM) | (1 << YADCNUM);
  // Отключаем подтягивающие резисторы
  X1MODE = (X1MODE & ~(3 << X1SELBIT)) | (2 << X1SELBIT);
  X2MODE = (X2MODE & ~(3 << X2SELBIT)) | (2 << X2SELBIT);
  Y1MODE = (Y1MODE & ~(3 << Y1SELBIT)) | (2 << Y1SELBIT);
  Y2MODE = (Y2MODE & ~(3 << Y2SELBIT)) | (2 << Y2SELBIT);
}

void TouchscreenFinalize(){
}

bool TouchscreenPressed(){
  x1input();
  x2open();
  y1open();
  y2gnd();
  asm("nop");
  asm("nop");
  asm("nop");
  if(bit_is_clear(X1PIN, X1BIT)){
    return true;
  }
  else {
    return false;
  }
}

uint32_t TouchscreenRawX(){
  x1vcc();
  x2gnd();
  y1adc();
  y2open();
  int32_t result = 0, i;
  for(i = 0; i < AV; i++){
    while(bit_is_clear(XADCDR, 31));
    result += ((XADCDR >> 6) & 0x3FF);
  }
  result /= AV;
  return result;
}

uint32_t TouchscreenRawY(){
  y1vcc();
  y2gnd();
  x1adc();
  x2open();
  int32_t result = 0, i;
  for(i = 0; i < AV; i++){
    while(bit_is_clear(YADCDR, 31));
    result += ((YADCDR >> 6) & 0x3FF);
  }
  result /= AV;
  return result;
}

int32_t TouchscreenX(){
  double x = TouchscreenRawX();
  double y = TouchscreenRawY();
  return (int32_t)(A * x + B * y + C);
}

int32_t TouchscreenY(){
  double x = TouchscreenRawX();
  double y = TouchscreenRawY();
  return (int32_t)(D * x + E * y + F);
}

static int64_t mac(int32_t *v1, int32_t *v2, int32_t n);

void TouchscreenCalibration(){
  int32_t raw_x[5], raw_y[5];
  int32_t unit[5] = {1, 1, 1, 1, 1};
  int32_t xd[5] = {0.5 * 640, 0.2 * 640, 0.2 * 640, 0.8 * 640, 0.8 * 640};
  int32_t yd[5] = {0.5 * 480, 0.8 * 480, 0.2 * 480, 0.2 * 480, 0.8 * 480};
  LCD_Set(WHITE);
  while(TouchscreenPressed());
  int i;
  for(i = 0; i < 5; i++){
    if(i > 0){
      LCD_FillRegion(xd[i - 1] - 1, yd[i - 1] - 1, 3, 3, WHITE);
    }
    LCD_FillRegion(xd[i] - 1, yd[i] - 1, 3, 3, BLACK);
    while(!TouchscreenPressed());
    raw_x[i] = TouchscreenRawX();
    raw_y[i] = TouchscreenRawY();
    while(TouchscreenPressed());
    // Задержка для предотвращения повторного нажатия
    volatile int j;
    for(j = 0; j < 2000000; j++);
  }
  // Расчет коэффициентов
  int64_t a = mac(raw_x, raw_x, 5);
  int64_t b = mac(raw_y, raw_y, 5);
  int64_t c = mac(raw_x, raw_y, 5);
  int64_t d = mac(raw_x, unit, 5);
  int64_t e = mac(raw_y, unit, 5);
  int64_t X1 = mac(raw_x, xd, 5);
  int64_t X2 = mac(raw_y, xd, 5);
  int64_t X3 = mac(xd, unit, 5);
  int64_t Y1 = mac(raw_x, yd, 5);
  int64_t Y2 = mac(raw_y, yd, 5);
  int64_t Y3 = mac(yd, unit, 5);
  int64_t dt = 5 * (a * b - c * c) + 2 * c * d * e - a * e * e - b * d * d;
  int64_t dx1 = 5 * (X1 * b - X2 * c) + e * (X2 * d - X1 * e) + X3 * (c * e - 
    b * d);
  int64_t dx2 = 5 * (X2 * a - X1 * c) + d * (X1 * e - X2 * d) + X3 * (c * d - 
    a * e);
  int64_t dx3 = X3 * (a * b - c * c) + X1 * (c * e - b * d) + X2 * (c * d - 
    a * e);
  int64_t dy1 = 5 * (Y1 * b - Y2 * c) + e * (Y2 * d - Y1 * e) + Y3 * (c * e - 
    b * d);
  int64_t dy2 = 5 * (Y2 * a - Y1 * c) + d * (Y1 * e - Y2 * d) + Y3 * (c * d - 
    a * e);
  int64_t dy3 = Y3 * (a * b - c * c) + Y1 * (c * e - b * d) + Y2 * (c * d - 
    a * e);
  A = (double)dx1 / (double)dt;
  B = (double)dx2 / (double)dt;
  C = (double)dx3 / (double)dt;
  D = (double)dy1 / (double)dt;
  E = (double)dy2 / (double)dt;
  F = (double)dy3 / (double)dt;
}

static int64_t mac(int32_t *v1, int32_t *v2, int32_t n){
  int64_t result = 0;
  int32_t i;
  for(i = 0; i < n; i++){
    result += v1[i] * v2[i];
  }
  return result;
}

