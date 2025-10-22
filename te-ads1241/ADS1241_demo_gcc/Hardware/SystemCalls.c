#include <stdlib.h>
#include <reent.h>
#include <sys/stat.h>
#include <errno.h>
#include "Config.h"


void * _sbrk_r(struct _reent *reent, ptrdiff_t nbytes){
	static uint8_t * heap_end;
	uint8_t *prev_heap_end;
	if(heap_end == 0){
		heap_end = (uint8_t *)HEAP_BASE_ADDR;
	}
	prev_heap_end = heap_end;
  if((nbytes % 4) > 0){
    nbytes += 4 - (nbytes % 4);
  }
	if(heap_end + nbytes >= (uint8_t *)HEAP_END_ADDR){
		while(true);
	}
	heap_end += nbytes;
	return prev_heap_end;
}

