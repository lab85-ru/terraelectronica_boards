#ifndef __SDIO_H__
#define __SDIO_H__

#include "appdefs.h"

#define SDIO_FILE_NAME "0:image.bin"

uint32_t sdio_get_dump_size();
TestStatus sdio_get_test_result();
TestStatus sdio_test();


#endif /*__SDIO_H__*/
