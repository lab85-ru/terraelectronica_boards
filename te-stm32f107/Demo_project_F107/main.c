#include <stdint.h>
#include <stdbool.h>
#include "Config.h"
#include "stm32f107.h"

#if __SYSTICK
volatile uint32_t counter;
#define DELAY_MS(ms)  do {counter = 0; while(counter < (ms));} while(false)
#if __DAC
volatile uint32_t dactrg;
#endif // __DAC
#endif // __SYSTICK

#if __USART
void USART2_PutByte(uint8_t byte);
uint8_t USART2_GetByte();
#endif // __USART

int main(){
  #if __PLL_CONFIG
  RCC_CR |= 1 << RCC_CR_HSEON;
  while(!(RCC_CR & (1 << RCC_CR_HSERDY)));
  // PREDIV2 = 5; PLL2MUL = 16; PREDIV1 = 10; PLLMUL = 9; ABP1 = AHB / 2;
  RCC_CFGR = (0x1 << RCC_CFGR_PLLSRC) | (0x0 << RCC_CFGR_PLLXTPRE) |
             (0x7 << RCC_CFGR_PLLMUL) | (0x4 << RCC_CFGR_PPRE1) |
             (0x3 << RCC_CFGR_ADCPRE);
  RCC_CFGR2 = (0x9 << RCC_CFGR2_PREDIV1) | (0x4 << RCC_CFGR2_PREDIV2) |
              (0xE << RCC_CFGR2_PLL2MUL) | (0x1 << RCC_CFGR2_PREDIV1SRC);
  RCC_CR |= 1 << RCC_CR_PLL2ON;
  while(!(RCC_CR & (1 << RCC_CR_PLL2RDY)));
  RCC_CR |= 1 << RCC_CR_PLLON;
  while(!(RCC_CR & (1 << RCC_CR_PLLRDY)));
  FLASH_ACR |= 0x2;
  RCC_CFGR |= 2 << RCC_CFGR_SW;
  // RCC_CR &= ~(1 << RCC_CR_HSION);
  #endif // __PLL_CONFIG
  #if __SYSTICK
  // SysTickReload = CALC_TIMRELOAD(1000, HSI_OSC);
  SysTickReload = CALC_TIMRELOAD(1000, 72000000);
  SysTickCurrent = 0;
  SysTickControl = 7;
  #endif // __SYSTICK
  #if __PORT_OUT
  bool ledon_flag = false;
  RCC_APB2ENR |= 1 << RCC_APB2ENR_IOPCEN;
  GPIOC_CRH = (GPIOC_CRH & ~0x000000F0) | 0x00000010;
  GPIOC_BRR = 0x00000200;
  #endif // __PORT_OUT
  #if __USART
  RCC_APB1ENR |= 1 << RCC_APB1ENR_USART2EN;
  RCC_APB2ENR |= (1 << RCC_APB2ENR_IOPDEN) | (1 << RCC_APB2ENR_AFIOEN);
  AFIO_MAPR |= 1 << AFIO_MAPR_USART2_REMAP;
  GPIOD_CRL = (GPIOD_CRL & 0xF0000FFF) | 0x08BB8000;
  GPIOD_BSRR = (1 << 3) | (1 << 6);
  // 19200
  USART2_BRR = (117 << USART2_BRR_DIV_Mantissa) |
               (3 << USART2_BRR_DIV_Fraction);
  USART2_CR1 = (1 << USART2_CR1_UE) | (1 << USART2_CR1_TE) |
               (1 << USART2_CR1_RE);
  #endif // __USART
  #if __ADC
  RCC_APB2ENR |= 1 << RCC_APB2ENR_ADC1EN;
  ADC1_SQR3 = 16;
  ADC1_SMPR2 = 7;
  ADC1_CR2 = (1 << ADC1_CR2_ADON) | (1 << ADC1_CR2_CONT) |
             (0x7 << ADC1_CR2_EXTSEL) | (1 << ADC1_CR2_EXTTRIG) |
             (1 << ADC1_CR2_SWSTART) | (1 << ADC1_CR2_TSVREFE);
  ADC1_CR1 = 1 << ADC1_CR1_SCAN;
  #endif // __ADC
  #if __DAC
  RCC_APB2ENR |= (1 << RCC_APB2ENR_IOPAEN) | (1 << RCC_APB2ENR_IOPCEN);
  RCC_APB1ENR |= 1 << RCC_APB1ENR_DACEN;
  GPIOA_CRL &= 0xFFF0FFFF;
  GPIOC_CRH = (GPIOC_CRH & 0xFF0FFFFF) | 0x00300000;
  GPIOC_BRR = 1 << 13;
  DAC_CR = (1 << DAC_CR_EN1) | (1 << DAC_CR_TEN1) | (7 << DAC_CR_TSEL1) |
           (__GENERATION << DAC_CR_WAVE1) | (0xF << DAC_CR_MAMP1);
  dactrg = 0;
  #endif // __DAC
  #if __PORT_OUT && __SYSTICK
  counter = 0;
  #endif // __PORT_OUT && __SYSTICK
  while(true){
    #if __PORT_OUT && __SYSTICK
    if(counter > 1000){
      if(ledon_flag){
        GPIOC_BSRR = 0x00000200;
      }
      else{
        GPIOC_BRR = 0x00000200;
      }
      ledon_flag = !ledon_flag;
      counter = 0;
      #if __ADC && __USART
      if(ADC1_SR & _BV(ADC1_SR_EOC)){
        uint16_t data = ADC1_DR;
        USART2_PutByte(*((uint8_t *)&data + 1));
        USART2_PutByte(*(uint8_t *)&data);
      }
      #endif // __ADC && __USART
    }
    #endif // __PORT_OUT && __SYSTICK
    #if __DAC
    if(dactrg > 1){
      DAC_SWTRIGR |= 1 << DAC_SWTRIGR_SWTRIG1;
      dactrg = 0;
    }
    #endif // __DAC
  }
}

#if __SYSTICK
void SysTick_Handler(){
  counter++;
  dactrg++;
}
#endif // __SYSTICK

#if __USART
void USART2_PutByte(uint8_t byte){
  while(!(USART2_SR & _BV(USART2_SR_TXE)));
  USART2_DR = byte;
}

uint8_t USART2_GetByte(){
  while(!(USART2_SR & _BV(USART2_SR_RXNE)));
  return USART2_DR;
}
#endif // __USART

