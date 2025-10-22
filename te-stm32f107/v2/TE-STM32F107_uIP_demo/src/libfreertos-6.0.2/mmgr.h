#ifndef __MMGR_H__
#define __MMGR_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct
   {
     uint32_t TotalSize  ;
     uint32_t AllocFails ;
     uint32_t AllocPeak  ;
     uint32_t FreeSpace  ;
   } THeapState ;

  void *HeapMalloc( size_t WantedSize ) ;
  void HeapFree( void * );
  void HeapGetState( THeapState* state );



#ifdef __cplusplus
  }
#endif

#endif /*__MMGR_H__*/
