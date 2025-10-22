#include "nand.h"
#include "stm32f4xx_gpio.h"

#define FSMC_Bank_NAND     FSMC_Bank3_NAND
#define Bank3_NAND_ADDR    ((uint32_t)0x80000000)
#define Bank_NAND_ADDR     Bank3_NAND_ADDR

#define ROW_ADDRESS (Address.Page + (Address.Block + (Address.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)

void NAND_ReadID(NAND_IDTypeDef* NAND_ID)
{
  uint32_t data = 0;

  /*!< Send Command to the command area */
  *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = 0x90;
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;

   /*!< Sequence to read ID from NAND flash */
  data = *(__IO uint32_t *)(Bank_NAND_ADDR | DATA_AREA);
	
	NAND_ID->Maker_ID   = (uint8_t)((data)& 0xFF);
	NAND_ID->Device_ID  = (uint8_t)(((data)& 0xFF00) >> 8);
	NAND_ID->Third_ID   = (uint8_t)(((data)& 0xFF0000) >> 16);
	NAND_ID->Fourth_ID  = (uint8_t)(((data)& 0xFF000000) >> 24);
}

static uint8_t NAND_ReadStatus(void )
{
  *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_STATUS;
	return (*(__IO uint8_t *)(Bank_NAND_ADDR));	
}

/**
  * @brief  Wait for NAND memory become ready
  * @param  None
  * @retval The status of the NAND memory. This parameter can be:
  *              - 0: all ok
  *              - not 0 - timeout occur
  */
static uint32_t NAND_WaitForReady(void )
{
	volatile int32_t timeout = 255;
	volatile int32_t Rb = 0;
	
	while (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
  while (!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));

	return 0;
}

/**
* @brief  Prepare for write : erase block
	* @param  Addr : address in NAND memory
  * @retval The status of the NAND memory. This parameter can be:
  *              - 0: Successful Erase
	*              - 1: Error in Erase
  */
uint8_t NAND_EraseBlock(uint32_t addr)
{
	uint8_t status;
	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE0;

	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR1(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR2(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR3(addr);

	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE1;
	
	NAND_WaitForReady();
	status = NAND_ReadStatus();
	
	return (status & 0x1);
}

	/**
	* @brief  Write page to NAND memory
	* @param  addr : address in NAND memory
	* @param  buffer : out buffer
	* @param	size : buffer size
	* @retval The status of the NAND memory. This parameter can be:
  *              - 0: Successful Program
	*              - 1: Error in Program
  */
uint8_t NAND_WritePage(uint32_t addr, uint8_t *buffer, uint32_t size)
{
	uint32_t index = 0;

	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE0;

	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = COLUMN_ADDR1(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = COLUMN_ADDR2(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR1(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR2(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR3(addr);

	for(index = 0; index < size; index++)
		*(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA) = buffer[index];
    *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE_TRUE1;	

	NAND_WaitForReady();
	return (NAND_ReadStatus() & 0x1);
}

	/**
	* @brief  Read page from NAND memory
	* @param  addr : address in NAND memory
	* @param  buffer : in buffer
	* @param	size : buffer size
	* @retval The status of the NAND memory. This parameter can be:
  *              - 0: Successful Erase
	*              - 1: Error in Erase
  */
void NAND_ReadPage(uint32_t addr, uint8_t *buffer, uint32_t size)
{
	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ0;

	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = COLUMN_ADDR1(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = COLUMN_ADDR2(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR1(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR2(addr);
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ROW_ADDR3(addr);

	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ1;

	NAND_WaitForReady();
	for (;size > 0; size--)
		*buffer++ = *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA);
}
