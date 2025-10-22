#ifndef ADS1241_H
#define ADS1241_H
#include <stdint.h>
#include <stdbool.h>

bool ADS1241_Initialize();
uint8_t RREG(uint8_t addr);
void WREG(uint8_t addr, uint8_t data);
int32_t RDATA();
bool DRDY();

#endif // ADS1241_H

