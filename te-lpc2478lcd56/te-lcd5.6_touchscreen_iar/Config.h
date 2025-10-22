#ifndef CONFIG_H
#define CONFIG_H
#include "LPC2478.h"

// Рабочая частота
#define FOSC                    (12000000)
#define MSEL_VAL                (12 - 1)
#define NSEL_VAL                (1 - 1)
#define CCLKCFG_VAL             (4 - 1)
#define CCLK                    (2 * FOSC * (MSEL_VAL + 1) / \
                                ((NSEL_VAL + 1) * (CCLKCFG_VAL + 1)))

// MAM
#define MAM_FETCH               (4)
#define MAM_MODE                (2)

// MEMMAP
#define MEMMAP_VAL              (1)

//LCD
#define LCD_BUFFER0_BASE_ADDR   (DYNAMIC_BANK0_BASE_ADDR)

// Touchscreen
#define XADCDR                  AD0DR3
#define XADCNUM                 (3)
#define YADCDR                  AD0DR1
#define YADCNUM                 (1)

// X1
#define X1BIT                   (24)
#define X1DIR                   FIO0DIR
#define X1SET                   FIO0SET
#define X1CLR                   FIO0CLR
#define X1PIN                   FIO0PIN
#define X1SELBIT                (16)
#define X1SEL                   PINSEL1
#define X1MODE                  PINMODE1
#define X1ADCSEL                (1)

// X2
#define X2BIT                   (29)
#define X2DIR                   FIO3DIR
#define X2SET                   FIO3SET
#define X2CLR                   FIO3CLR
#define X2PIN                   FIO3PIN
#define X2SELBIT                (26)
#define X2SEL                   PINSEL7
#define X2MODE                  PINMODE7

// Y1
#define Y1BIT                   (26)
#define Y1DIR                   FIO0DIR
#define Y1SET                   FIO0SET
#define Y1CLR                   FIO0CLR
#define Y1PIN                   FIO0PIN
#define Y1SELBIT                (20)
#define Y1SEL                   PINSEL1
#define Y1MODE                  PINMODE1
#define Y1ADCSEL                (1)

// Y2
#define Y2BIT                   (28)
#define Y2DIR                   FIO3DIR
#define Y2SET                   FIO3SET
#define Y2CLR                   FIO3CLR
#define Y2PIN                   FIO3PIN
#define Y2SELBIT                (24)
#define Y2SEL                   PINSEL7
#define Y2MODE                  PINMODE7

#endif // CONFIG_H

