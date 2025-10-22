#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "mini_STM32F217.h"
#include "nand.h"
#include <stdio.h>

#define PSRAM_TEST_START 	0x60000000
#define TEST_BUFFER_SIZE	0x100

void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);

/* Error codes*/
#define PSRAM_TEST_FAIL				1
#define NAND_READ_ID_FAIL 		2
#define NAND_ERASE_BLOCK_FAIL	3
#define NAND_WRITE_PAGE_FAIL	4
#define	NAND_READ_PAGE_FAIL		5
#define NAND_COMPARE_FAIL			6

#endif /* __MAIN_H */
