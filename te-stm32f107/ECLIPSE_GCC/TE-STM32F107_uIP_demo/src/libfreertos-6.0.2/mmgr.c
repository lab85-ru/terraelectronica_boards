
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "mmgr.h"

// ��������� ������ ����������� ����
typedef struct HEAP
{
	unsigned long Dummy;
	unsigned char HeapPool[ configTOTAL_HEAP_SIZE ]; // �������� ��� ����
} THeap ;

static THeap Heap ;

// ��������� �������� ���������� ������ ����������� ��������� ����� ������
typedef struct BLOCK_LINK
{
	struct BLOCK_LINK* NextFreeBlock; // ��������� �� ��������� ���� ���������������� ������
	size_t BlockSize;  // ������ ����� ���������������� ������
} TBlockLink ;

static const unsigned short  heapSTRUCT_SIZE	= ( sizeof( TBlockLink ) + ( sizeof( TBlockLink ) % portBYTE_ALIGNMENT ) );
#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( heapSTRUCT_SIZE * 2 ) )

// ��� ��������� �������� ������ - ��������� � �����������
// ����������� ������� ����
static TBlockLink Start, End;

// ����� ������� ������������� ������
static size_t AllocFails = 0 ;
static size_t HeapPeak = configTOTAL_HEAP_SIZE ;

// ���� ���� ����������� ����������� ������ ��������� ������
// ������ ����������� - �� ���������� ������� ������
// ��������� �������� ��� ���������� ����������� ������� � �������� ������������� �����
#define InsertBlockIntoFreeList( BlockToInsert )   \
{					                \
  TBlockLink *Iterator;				\
  size_t BlockSize;   					\
  BlockSize = BlockToInsert->BlockSize;		\
  /* ����������� ����� ����� �������� �������*/         \
  for( Iterator = &Start; Iterator->NextFreeBlock->BlockSize < BlockSize; Iterator = Iterator->NextFreeBlock )	\
   {							\
     /* ���� ������ ��� �������������� ���������� ����� */\
   }							\
  /* �������� ������  */                                \
  BlockToInsert->NextFreeBlock = Iterator->NextFreeBlock; \
  Iterator->NextFreeBlock = BlockToInsert;	\
}
/*-----------------------------------------------------------*/
// ������������� ���������� � ������������ �������� ���������� ������
// ��������� �������� ��� ���������� ����������� ������� � �������� ������������� �����
//#define HeapInit()
static __attribute__ ((always_inline)) void HeapInit()
{
 // TBlockLink *FirstFreeBlock;
  /* xStart ������������� ��� ������ ������*/
  Start.NextFreeBlock = ( void * ) Heap.HeapPool;
  Start.BlockSize = ( size_t ) 0;
  /* xEnd ������������� ��� ����� ������. */
  End.NextFreeBlock = NULL;
  End.BlockSize = configTOTAL_HEAP_SIZE;
  /* ������������ "�������" ����� ����������� ���� ��� */
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
        // ������� ������������
	vTaskSuspendAll();
	{
		/* ������������� ������ ��������� ������ ���� ��� �� ��������� */
		if( HeapHasBeenInitialised == pdFALSE )
		{
			HeapInit();
			HeapHasBeenInitialised = pdTRUE;
		}

		/* ���������� ���������� ������� �� ���������� ������ �������� ������ */
		if( WantedSize > 0 )
		{
			WantedSize += heapSTRUCT_SIZE;

			/* ������������ ������� �������������� ������ */
			if( WantedSize & portBYTE_ALIGNMENT_MASK )
			{
                           WantedSize += ( portBYTE_ALIGNMENT - ( WantedSize & portBYTE_ALIGNMENT_MASK ) );
			}
		}

		if( ( WantedSize > 0 ) && ( WantedSize < configTOTAL_HEAP_SIZE ) )
		{
			// ����� ���������� ����������� �������, �� ������ ��� ���������
			PreviousBlock = &Start;
			Block = Start.NextFreeBlock;
			while( ( Block->BlockSize < WantedSize ) && ( Block->NextFreeBlock ) )
			{
				PreviousBlock = Block;
				Block = Block->NextFreeBlock;
			}

			// ���� ���� ������, ���������� ������������� ������
			if( Block != &End )
			{
				// ���������� ������ �������������� ������
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
			/* ���������� ����� � ������ ��������� */
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
   // �������������
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

