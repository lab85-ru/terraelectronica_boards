#include "ADS1241.h"
#include "LPC2478.h"
#include "Config.h"

#define Tosc      100

static inline void cs_enable(){
  FIO2SET = _BV(19);
}

static inline void cs_disable(){
  FIO2CLR = _BV(19);
}

static inline void spi_init(){
  SSP0CR0 = (7 << SSP0CR0_DSS) | (0 << SSP0CR0_CPOL) | (1 << SSP0CR0_CPHA);
  SSP0CR1 = 0;
  SSP0CPSR = 30;
  SSP0CR1 |= (1 << SSP0CR1_SSE);
}

static inline void spi_write(uint8_t data){
  data = ~data;
  while(!(SSP0SR & _BV(SSP0SR_TNF)));
  SSP0DR = data;
  while(!(SSP0SR & _BV(SSP0SR_TFE)));
}

static inline uint8_t spi_read(){
  register uint8_t data;
  while(SSP0SR & _BV(SSP0SR_RNE)){
    data = SSP0DR;
  }
  SSP0DR = ~(0x00);
  while(!(SSP0SR & _BV(SSP0SR_RNE)));
  data = SSP0DR;
  data = ~data;
  return data;
}

bool ADS1241_Initialize(){
  spi_init();
  FIO2DIR |= _BV(19); //CS
  FIO0DIR |= _BV(28); // POL
  FIO0CLR = _BV(28);
  FIO2DIR |= _BV(31); // Reset
  FIO2CLR = _BV(31);
  FIO2DIR |= _BV(25); // BufEn
  //FIO2SET = _BV(25);
  FIO2CLR = _BV(25);
  FIO2DIR |= _BV(15); // DSYNC
  FIO2CLR = _BV(15);
  FIO0DIR |= _BV(1); // PWDN
  FIO0CLR = _BV(1);
  cs_enable();
  return true;
}

bool DRDY(){
  static bool status = false;
  if(!(FIO0PIN & _BV(22))){
    status = true;
  }
  else if(status){
    status = false;
    return true;
  }
  return false;
}

int32_t RDATA(){
  uint32_t data = 0;
  spi_write(0x01);
  T1TC = 0; while(T1TC < 50000);
  data |= spi_read() << 16;
  T1TC = 0; while(T1TC < 50000);
  data |= spi_read() << 8;
  T1TC = 0; while(T1TC < 50000);
  data |= spi_read();
  T1TC = 0; while(T1TC < 50000);
  if(data & 0x800000){
    data = 0xFFFFFF - data;
  }
  return data;
}

uint8_t RREG(uint8_t addr){
  spi_write(0x10 | (addr & 0x0F));
  T1TC = 0; while(T1TC < 50000);
  spi_write(0);
  T1TC = 0; while(T1TC < 50000);
  uint8_t data = spi_read();
  T1TC = 0; while(T1TC < 50000);
  return data;
}

void WREG(uint8_t addr, uint8_t data){
  spi_write(0x50 | (addr & 0x0F));
  T1TC = 0; while(T1TC < 50000);
  spi_write(0);
  T1TC = 0; while(T1TC < 50000);
  spi_write(data);
  T1TC = 0; while(T1TC < 50000);
}

