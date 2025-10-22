#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H
#include <stdint.h>
#include <stdbool.h>

void TouchscreenInitialize();
void TouchscreenFinalize();
bool TouchscreenPressed();
uint32_t TouchscreenRawX();
uint32_t TouchscreenRawY();
int32_t TouchscreenX();
int32_t TouchscreenY();
void TouchscreenCalibration();

#endif // TOUCHSCREEN_H

