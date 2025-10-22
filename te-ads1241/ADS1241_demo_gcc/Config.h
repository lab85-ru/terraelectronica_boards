#ifndef CONFIG_H
#include <stdint.h>
#include <stdbool.h>
#include "LPC2478.h"

// Частота
#define FOSC          (12000000)
#define MSEL_VAL      (12 - 1)
#define NSEL_VAL      (1 - 1)
#define CCLKCFG_VAL   (4 - 1)
#define CCLK          (2 * FOSC * (MSEL_VAL + 1) / ((NSEL_VAL + 1) * (CCLKCFG_VAL + 1)))

// MAM
#define MAM_FETCH     (4)
#define MAM_MODE      (2)

// MEMMAP
#define MEMMAP_VAL    (1)

#define EXRAM_SIZE              ((16 * 1024 * 1024 * 16) / 8)

#define EXRAM_BASE_ADDR         (DYNAMIC_BANK0_BASE_ADDR)
#define EXRAM_END_ADDR          (EXRAM_BASE_ADDR + EXRAM_SIZE)

// LCD
#define _16BPP        (16)
#define _24BPP        (32)
#define COLOUR_DEPTH  _16BPP
#define COLOUR_SIZE   (COLOUR_DEPTH / 8)

#if COLOUR_SIZE == 2
#define COLOUR_TYPE   uint16_t
#elif COLOUR_SIZE == 4
#define COLOUR_TYPE   uint32_t
#endif // COLOUR_SIZE

#define LCD_WIDTH     (640)
#define LCD_HEIGHT    (480)

#define LCD_BUFFER_SIZE         (LCD_WIDTH * LCD_HEIGHT * COLOUR_SIZE)
#define LCD_BUFFER0_BASE_ADDR   (EXRAM_BASE_ADDR)
#define LCD_BUFFER0_END_ADDR    (LCD_BUFFER0_BASE_ADDR + LCD_BUFFER_SIZE)
#define LCD_BUFFER1_BASE_ADDR   (LCD_BUFFER0_END_ADDR)
#define LCD_BUFFER1_END_ADDR    (LCD_BUFFER1_BASE_ADDR + LCD_BUFFER_SIZE)

// Heap
#define HEAP_SIZE               (EXRAM_SIZE - LCD_BUFFER_SIZE * 2)
#define HEAP_BASE_ADDR          (LCD_BUFFER1_END_ADDR)
#define HEAP_END_ADDR           (EXRAM_END_ADDR)

#endif // CONFIG_H

