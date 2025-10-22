#include "sdcard.h"
#include "ff.h"
#include "diskio.h"
#include "platform_config.h"


#include "stm32f10x.h"

//---------------------------------------------------------------------------------------------
static uint32_t disc_status[_VOLUMES] ;
SD_CardInfo disc_sd_card_info ;

void __attribute__((constructor)) disk_sd_init_status()
{
  for ( uint8_t drive = 0 ; drive < _VOLUMES ; drive++ )
	  disc_status[drive] = STA_NODISK ;
}
//---------------------------------------------------------------------------------------------
void disk_sd_init_power_detect_pins()
{
  static bool is_initialized = FALSE ;
  if (is_initialized == TRUE ) return ;

  RCC_APB2PeriphClockCmd(SDIO_CARD_POWER_DETECT_APB2Periph_GPIO , ENABLE);
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = SDIO_CARD_DETECT_PIN ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
  GPIO_Init(SDIO_CARD_POWER_DETECT_PORT, &GPIO_InitStructure);

  SDIO_CARD_POWER_DETECT_PORT->BSRR = SDIO_CARD_POWER_PIN ;

  GPIO_InitStructure.GPIO_Pin = SDIO_CARD_POWER_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SDIO_CARD_POWER_DETECT_PORT, &GPIO_InitStructure);

  is_initialized = TRUE ;
}
//---------------------------------------------------------------------------------------------
bool disk_sd_present()
{
	disk_sd_init_power_detect_pins();
	return !(SDIO_CARD_POWER_DETECT_PORT->IDR & SDIO_CARD_DETECT_PIN) ;
}
//---------------------------------------------------------------------------------------------
void disk_sd_set_power ( FunctionalState NewState )
{
	disk_sd_init_power_detect_pins();
	if ( NewState == ENABLE )
		SDIO_CARD_POWER_DETECT_PORT->BRR = SDIO_CARD_POWER_PIN ;
	else
		SDIO_CARD_POWER_DETECT_PORT->BSRR = SDIO_CARD_POWER_PIN ;
}
//---------------------------------------------------------------------------------------------
SD_CardInfo* disk_sd_get_card_info ( )
{
	return &disc_sd_card_info ;
}
//---------------------------------------------------------------------------------------------
DRESULT disk_read(uint8_t drive, uint8_t *buffer, uint32_t sector, uint8_t count)
{
    switch ( drive )
    {
      case 0: // SDIO  интерфейс
        {
    	  SD_Error Status = SD_OK;
    	  u32 *buffer32;
    	  buffer32 = (u32*)buffer;
    	  switch (count)
    	  {
    	    case 0 : return RES_PARERR ;
    	    case 1 :
        	  Status = SD_ReadBlock( sector << 9, buffer32 ,  _MAX_SS);
        	  break ;
    	    default:
        	  Status = SD_ReadMultiBlocks( sector << 9 , buffer32,  _MAX_SS , count);
    	  }
         if (Status == SD_OK)
             return RES_OK ;
         else
             return RES_ERROR;
        }
      default:   // другие интерфейсы накопителей не реализованы
    	  return RES_NOTRDY ;
    }
}
//----------------------------------------------------------------------------------------------
DRESULT disk_write(uint8_t drive, const uint8_t *buffer, uint32_t sector, uint8_t count)
{
    switch ( drive )
    {
      case 0: // SDIO  интерфейс
       {
    	  SD_Error Status = SD_OK;
    	  u32 *buffer32;
    	  buffer32 = (u32*)buffer;
    	  switch (count)
    	  {
    	    case 0 : return RES_PARERR ;
    	    case 1 :
        	  Status = SD_WriteBlock(  sector << 9 , buffer32,  _MAX_SS);
        	  break ;
    	    default:
        	  Status = SD_WriteMultiBlocks( sector << 9 , buffer32, _MAX_SS , count);
    	  }
         if (Status == SD_OK)
             return RES_OK ;
         else
             return RES_ERROR;
        }
      default:   // другие интерфейсы накопителей не реализованы
    	  return RES_NOTRDY ;
    }
}
//----------------------------------------------------------------------------------------------
uint32_t get_fattime (void)
{
   return 0 ;
}
//----------------------------------------------------------------------------------------------

DSTATUS disk_initialize ( uint8_t drive)
{
	switch ( drive )
	{
	  case 0: // SDIO  интерфейс
	   {
	    NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

	    // проверяем наличие накопителя в сокете
		if ( !disk_sd_present() )
		{
			disc_status[drive] = STA_NODISK ;
		    return disc_status[drive] ;
		}
		disc_status[0] = STA_NOINIT ;

		SD_Error Status = SD_OK;
		// подключение питания к сокету
		disk_sd_set_power(ENABLE);
		Status = SD_Init();
		if (Status == SD_OK)// Read CSD/CID MSD registers
   		   Status = SD_GetCardInfo(&disc_sd_card_info);

        if (Status == SD_OK)// Select Card
		   Status = SD_SelectDeselect((u32) (disc_sd_card_info.RCA << 16));

        if (Status == SD_OK)
		   Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);

		if (Status == SD_OK) // Set Device Transfer Mode to DMA
		   Status = SD_SetDeviceMode(SD_DMA_MODE);

        if ( Status == SD_OK )
		   disc_status[0] &= ~STA_NOINIT ;
        break ;
	    }
	  default: // другие интерфейсы накопителей не реализованы
		break ;
	 }
  return disc_status[drive] ;
}
//----------------------------------------------------------------------------------------------
DSTATUS disk_deinitialize (uint8_t drive)
{
	switch ( drive )
	{
	  case 0: // SDIO  интерфейс
		  disc_status[drive] = STA_NOINIT ;
	      SDIO_DeInit();
	      // отключение питания от сокета
	      disk_sd_set_power(DISABLE);
	      NVIC_InitTypeDef NVIC_InitStructure;
	      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	      NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	      NVIC_Init(&NVIC_InitStructure);
	      break ;
	  default: // другие интерфейсы накопителей не реализованы
		  break ;
	}
	return disc_status[drive] ;
}
//----------------------------------------------------------------------------------------------
DSTATUS disk_status (uint8_t drive)
{
   return disc_status[drive] ;
}
//----------------------------------------------------------------------------------------------
DRESULT disk_ioctl ( uint8_t drive,   uint8_t ctrl, void* buff)
{
	 switch(drive)
	   {
	      case 0 : // SDIO  интерфейс
	    	  switch(ctrl)
	    		       {
	    		        case CTRL_SYNC:
	    		        	return RES_OK;
	    		        case GET_SECTOR_SIZE:
	    		          {
	    		        	*(uint32_t*)buff = 0 ;
	    		        	return RES_OK;
	    		          }
	    		        case GET_SECTOR_COUNT:
	    		          {
	    		        	*(uint32_t*)buff = 0 ;
	    		        	return RES_OK;
	    		          }
	    		        case GET_BLOCK_SIZE:
	    		            *(uint32_t*)buff = 1 ;
	    		            return RES_OK;
	    		        default:
	    		            return RES_PARERR;
	    		        }
	    	  break ;
	      default :  // другие интерфейсы накопителей не реализованы
	    	  return RES_NOTRDY ;
	   }
}
//----------------------------------------------------------------------------------------------



