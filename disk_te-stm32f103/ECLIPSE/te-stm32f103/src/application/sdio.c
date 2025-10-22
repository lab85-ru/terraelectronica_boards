#include "sdio.h"
#include "ff.h"

static  sdio_dump_size = 0 ;
TestStatus sdio_test_result = FAILED ;
//---------------------------------------------------------------------
TestStatus sdio_get_test_result()
{
  return sdio_test_result ;
}
//---------------------------------------------------------------------
uint32_t sdio_get_dump_size()
{
  return sdio_dump_size ;
}
//---------------------------------------------------------------------
TestStatus sdio_test()
{
	extern uint32_t _image_end_ ;
    extern uint32_t _image_start_;
	const void* dump_ptr = &_image_start_ ; // дамп прошивки микроконтроллера
	const uint32_t sdio_dump_size = (uint32_t)&_image_end_  - (uint32_t)&_image_start_ ; // размер проши
	uint32_t byte_readed ;
	FRESULT fresult = FR_NOT_READY ;
	FATFS  FileSystemObject ;
	FIL    FileObject ;

	// монтирование файловой системы
	fresult = f_mount( 0 /* номер устройства, 0: SDIO*/, &FileSystemObject ) ;

	if ( fresult == FR_OK )
		// создание файла в корне файловой системы
		fresult = f_open( &FileObject , SDIO_FILE_NAME ,  FA_CREATE_ALWAYS | FA_WRITE | FA_READ ) ;

	if ( fresult == FR_OK )
		// запись дампа прошивки в файл на накопителе
		fresult = f_write( &FileObject , dump_ptr , sdio_dump_size ,  (UINT*)&byte_readed ) ;

	if ( fresult == FR_OK )
		// закрытие файла
		fresult = f_close(&FileObject) ;

	f_mount( 0 , NULL ) ;

	sdio_test_result = !fresult ;

    return (TestStatus) !fresult ;
}
//---------------------------------------------------------------------

