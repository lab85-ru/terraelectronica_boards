
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "mmgr.h"

// структура данных описывающих кучу
typedef struct HEAP
{
	unsigned long Dummy;
	unsigned char HeapPool[ configTOTAL_HEAP_SIZE ]; // байтовый пул кучи
} THeap ;

static THeap Heap ;

// структура элемента связанного списка описывающая свободные блоки памяти
typedef struct BLOCK_LINK
{
	struct BLOCK_LINK* NextFreeBlock; // указатель на следующий блок нераспределенной памяти
	size_t BlockSize;  // размер блока нераспределенной памяти
} TBlockLink ;

static const unsigned short  heapSTRUCT_SIZE	= ( sizeof( TBlockLink ) + ( sizeof( TBlockLink ) % portBYTE_ALIGNMENT ) );
#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( heapSTRUCT_SIZE * 2 ) )

// два связанных элемента списка - стартовый и завершающий
// маркирующие границы кучи
static TBlockLink Start, End;

// число отказов распредеоения памяти
static size_t AllocFails = 0 ;
static size_t HeapPeak = configTOTAL_HEAP_SIZE ;

// блок кода реализующий модификацию списка свободных блоков
// список сортируется - по увеличению размера блоков
// объявлено макросом для уменьшения вложенности вызовов и снижения использования стека
#define InsertBlockIntoFreeList( BlockToInsert )   \
{					                \
  TBlockLink *Iterator;				\
  size_t BlockSize;   					\
  BlockSize = BlockToInsert->BlockSize;		\
  /* итеративный поиск блока большего размера*/         \
  for( Iterator = &Start; Iterator->NextFreeBlock->BlockSize < BlockSize; Iterator = Iterator->NextFreeBlock )	\
   {							\
     /* блок найден при индексировании аргументов цикла */\
   }							\
  /* корекция списка  */                                \
  BlockToInsert->NextFreeBlock = Iterator->NextFreeBlock; \
  Iterator->NextFreeBlock = BlockToInsert;	\
}
/*-----------------------------------------------------------*/
// инициализация стартового и завершающего элемента связанного списка
// объявлено макросом для уменьшения вложенности вызовов и снижения использования стека
//#define HeapInit()
static __attribute__ ((always_inline)) void HeapInit()
{
 // TBlockLink *FirstFreeBlock;
  /* xStart инициализация как начало списка*/
  Start.NextFreeBlock = ( void * ) Heap.HeapPool;
  Start.BlockSize = ( size_t ) 0;
  /* xEnd инициализация как конец списка. */
  End.NextFreeBlock = NULL;
  End.BlockSize = configTOTAL_HEAP_SIZE;
  /* нициализация "первого" блока содержащего весь пул */
//  FirstFreeBlock = ( void * ) Heap.HeapPool;
//  FirstFreeBlock->BlockSize = configTOTAL_HEAP_SIZE;
//  FirstFreeBlock->NextFreeBlock = &End;

  ((TBlockLink *)Heap.HeapPool)->BlockSize = configTOTAL_HEAP_SIZE;
  ((TBlockLink *)Heap.HeapPool)->NextFreeBlock = &End;
}
/*-----------------------------------------------------------*/

void *pvPortMalloc( size_t WantedSize ) __attribute__ ((alias ("HeapMalloc"))) ;
void *HeapMalloc( size_t WantedSize )
{
TBlockLink *Block, *PreviousBlock, *NewBlockLink;
static long HeapHasBeenInitialised = pdFALSE;
void *Return = NULL;
        // останов планировщика
	vTaskSuspendAll();
	{
		/* инициализация списка свободных блоков если она не выполнена */
		if( HeapHasBeenInitialised == pdFALSE )
		{
			HeapInit();
			HeapHasBeenInitialised = pdTRUE;
		}

		/* увеличение требуемого размера на выровненый размер елемента списка */
		if( WantedSize > 0 )
		{
			WantedSize += heapSTRUCT_SIZE;

			/* выравнивание границы распределяймой памяти */
			if( WantedSize & portBYTE_ALIGNMENT_MASK )
			{
                           WantedSize += ( portBYTE_ALIGNMENT - ( WantedSize & portBYTE_ALIGNMENT_MASK ) );
			}
		}

		if( ( WantedSize > 0 ) && ( WantedSize < configTOTAL_HEAP_SIZE ) )
		{
			// поиск свободного наименьшего размера, но больше чем требуемый
			PreviousBlock = &Start;
			Block = Start.NextFreeBlock;
			while( ( Block->BlockSize < WantedSize ) && ( Block->NextFreeBlock ) )
			{
				PreviousBlock = Block;
				Block = Block->NextFreeBlock;
			}

			// если блок найден, выполнение распределение памяти
			if( Block != &End )
			{
				// вычисление адреса распределяемой памяти
				Return = ( void * ) ( ( ( unsigned char * ) PreviousBlock->NextFreeBlock ) + heapSTRUCT_SIZE );

				/* This block is being returned for use so must be taken our of the
				list of free blocks. */
				PreviousBlock->NextFreeBlock = Block->NextFreeBlock;

				/* If the block is larger than required it can be split into two. */
				if( ( Block->BlockSize - WantedSize ) > heapMINIMUM_BLOCK_SIZE )
				{
					/* This block is to be split into two.  Create a new block
					following the number of bytes requested. The void cast is
					used to prevent byte alignment warnings from the compiler. */
					NewBlockLink = ( void * ) ( ( ( unsigned char * ) Block ) + WantedSize );

					/* Calculate the sizes of two blocks split from the single
					block. */
					NewBlockLink->BlockSize = Block->BlockSize - WantedSize;
					Block->BlockSize = WantedSize;

					/* Insert the new block into the list of free blocks. */
					InsertBlockIntoFreeList( ( NewBlockLink ) );
				}
			}
		}
	}
	if ( !Return ) AllocFails ++ ;
	xTaskResumeAll();
	return Return;
}
/*-----------------------------------------------------------*/

void vPortFree( void* pv ) __attribute__ (( alias ("HeapFree")));
void  HeapFree( void* pv )
{
unsigned char *puc = ( unsigned char * ) pv;
TBlockLink *Link;

	if( pv )
	{
		/* The memory being freed will have an xBlockLink structure immediately
		before it. */
		puc -= heapSTRUCT_SIZE;
		Link = ( void * ) puc;
		vTaskSuspendAll();
		{
			/* добавление блока в список свободных */
			InsertBlockIntoFreeList( ( ( TBlockLink * ) Link ) );
		}
		xTaskResumeAll();
	}
}
/*
void vPortFree(void* pv)
{
  HeapFree(pv) ;
}
*/
/*-----------------------------------------------------------*/
void HeapDefragmentation()
{
   // нереализовано
}
/*-----------------------------------------------------------*/
void HeapGetState( THeapState* state )
{
  size_t free_space = 0 ;
  vTaskSuspendAll();

  state->TotalSize = configTOTAL_HEAP_SIZE ;
  state->AllocFails = AllocFails ;

  TBlockLink* link = &Start ;
  while (1)
      {
        if ( !link->NextFreeBlock ) break ;
        free_space += link->BlockSize ;
        link = link->NextFreeBlock ;
      }
  if ( HeapPeak > free_space ) HeapPeak = free_space ;
  xTaskResumeAll();

  state->AllocPeak = HeapPeak   ;
  state->FreeSpace = free_space   ;
}

//------------------------------------------------------------
size_t HeapGetFree()
{
  size_t free_space = 0 ;
  vTaskSuspendAll();
  TBlockLink* link = &Start ;
  while (1)
    {
      if ( !link->NextFreeBlock ) break ;
      free_space += link->BlockSize ;
      link = link->NextFreeBlock ;
    }
  if ( HeapPeak > free_space ) HeapPeak = free_space ;
  xTaskResumeAll();
  return free_space ;
}

