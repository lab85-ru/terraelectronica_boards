#ifndef __NAND_H__
#define __NAND_H__

//#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"


typedef struct
{
  uint8_t Maker_ID;
  uint8_t Device_ID;
  uint8_t Third_ID;
  uint8_t Fourth_ID;
}NAND_IDTypeDef;


#define CMD_AREA                   (uint32_t)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                  (uint32_t)(1<<17)  /* A17 = ALE high */

#define DATA_AREA                  ((uint32_t)0x00000000)

/**
  * @brief  FSMC NAND memory command
  */
#define NAND_CMD_READ0						 ((uint8_t)0x00)
#define NAND_CMD_READ1						 ((uint8_t)0x30)

#define NAND_CMD_AREA_A            ((uint8_t)0x00)
#define NAND_CMD_AREA_B            ((uint8_t)0x01)
#define NAND_CMD_AREA_C            ((uint8_t)0x50)

#define NAND_CMD_WRITE0            ((uint8_t)0x80)
#define NAND_CMD_WRITE_TRUE1       ((uint8_t)0x10)

#define NAND_CMD_ERASE0            ((uint8_t)0x60)
#define NAND_CMD_ERASE1            ((uint8_t)0xD0)

#define NAND_CMD_READID            ((uint8_t)0x90)
#define NAND_CMD_STATUS            ((uint8_t)0x70)
#define NAND_CMD_LOCK_STATUS       ((uint8_t)0x7A)
#define NAND_CMD_RESET             ((uint8_t)0xFF)


#define NAND_PAGE_SIZE             ((uint16_t)0x0800) /* 2048 bytes per page w/o Spare Area */
#define NAND_BLOCK_SIZE            ((uint16_t)0x0040) /* 64x2048 bytes pages per block */
#define NAND_ZONE_SIZE             ((uint16_t)0x0800) /* 2048 Block per zone */
#define NAND_SPARE_AREA_SIZE       ((uint16_t)0x0040) /* last 16 bytes as spare area */
#define NAND_MAX_ZONE              ((uint16_t)0x0001) /* 1 zones of 2048 block */


#define COLUMN_ADDR1(ADDR) 	(uint8_t)((ADDR)& 0xFF)
#define COLUMN_ADDR2(ADDR) 	(uint8_t)(((ADDR)& 0xF00) >> 8)
#define ROW_ADDR1(ADDR)			(uint8_t)(((ADDR)& 0xFF000) >> 12)
#define ROW_ADDR2(ADDR)			(uint8_t)(((ADDR)& 0xFF00000) >> 20)
#define ROW_ADDR3(ADDR)			(uint8_t)(((ADDR)& (1 << 28)) >> 28)


void NAND_Init(void);
void NAND_ReadID(NAND_IDTypeDef* NAND_ID);
uint8_t NAND_EraseBlock(uint32_t addr);
uint8_t NAND_WritePage(uint32_t addr, uint8_t *buffer, uint32_t size);
void NAND_ReadPage(uint32_t addr, uint8_t *buffer, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
