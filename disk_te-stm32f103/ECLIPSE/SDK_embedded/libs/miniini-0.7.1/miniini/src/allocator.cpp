// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "allocator.h"
#include <cstring>
#include <cassert>

namespace miniini_private
{

Allocator::Allocator(const ui size, const ui blocks)
    :NumBlocks(blocks)
    ,CurrentBlock(0)
    ,BlockMinSize(size / NumBlocks + 1)
    ,Blocks(new Block * [NumBlocks])
{
    //Allocate enough blocks to have size of space
    for(ui block = 0; block < NumBlocks; ++block)
    {
        Blocks[block] = new Block(BlockMinSize);
    }
}

void Allocator::Trim()
{
    //Since current block can only be incremented,
    //if the current block is not the last block, there are
    //some blocks left since the construction that we can delete.
    if(CurrentBlock < (NumBlocks - 1))
    {
        for(ui block = CurrentBlock + 1; block < NumBlocks; ++block)
        {
            delete Blocks[block];
        }
        //Reallocate Blocks to only hold pointers to used blocks.
        NumBlocks = CurrentBlock + 1;
        Block * * tempblocks = new Block * [NumBlocks];
        memcpy(tempblocks, Blocks, NumBlocks * sizeof(Block *));
        delete [] Blocks;
        Blocks = tempblocks;
    }
}

void Allocator::DeleteSpace(c * const ptr)
{
    assert(ptr);
    Block * block;
    //Find which block does ptr point to
    for(ui blockidx = 0; blockidx < NumBlocks; ++blockidx)
    {
        block = Blocks[blockidx];
        //Ptr is inside this block
        if(ptr >= block->Data && ptr < (block->Data + block->Allocated))
        {
            ++block->PtrsDeleted;
            //If as many pointers as given were deleted, we can delete this block
            const bool readytodelete = block->PtrsDeleted == block->PtrsGiven ? 
                                       true : false;
            if(readytodelete)
            {
                //This is the current block: no need to delete it as it's not yet full
                if(blockidx == CurrentBlock)
                {
                    return;
                }
                NumBlocks -= 1;
                //This is the only block left, so delete it and replace by an empty block.
                if(!NumBlocks)
                {
                    delete block;
                    Blocks[0] = new Block(BlockMinSize);
                    NumBlocks = 1;
                    return;
                }
                //Reallocate Blocks to smaller size.
                Block * * tempblocks = new Block * [NumBlocks];
                memcpy(tempblocks, Blocks, blockidx * sizeof(Block *));
                memcpy(tempblocks + blockidx, Blocks + blockidx + 1, 
                       (NumBlocks - blockidx) * sizeof(Block *));
                delete block;
                delete [] Blocks;
                Blocks = tempblocks;
            }
            return;
        }
    }
    //If we get here, ptr does not point to any block which is an error
    assert(false);
}

c * Allocator::NewSpace(const ui size)
{
    Block * block = Blocks[CurrentBlock];
    //Not enough space in this block. Add new block and allocate from there
    if((block->Allocated - block->Used) < size)
    {
        NewBlock(size);
        return NewSpace(size);
    }
    //Pointer to allocated space
    c * out = block->Data + block->Used;
    //Update block data
    block->Used += size;
    ++block->PtrsGiven;
    return out;
}

Allocator::~Allocator()
{
    //Delete all blocks
    for(ui block = 0; block < NumBlocks; ++block)
    {
        delete Blocks[block];
    }
    delete [] Blocks;
}

//PROBABLY ERROR HERE
void Allocator::NewBlock(const ui minsize)
{
    ++CurrentBlock;
    //Size of the new block is max(minsize, BlockMinSize)
    if(minsize > BlockMinSize)
    {
        BlockMinSize = minsize;
    }
    //Not the last block, we just need to move to the next one
    if(CurrentBlock < NumBlocks)
    {
        //If the block is not large enough, enlarge it
        if(Blocks[CurrentBlock]->GetFreeSpace() < minsize)
        {
            Blocks[CurrentBlock]->Reallocate(BlockMinSize);
        }
    }
    //We need to allocate a new block (and reallocate the Blocks ptr array)
    else
    {
        Block * * tempblocks = new Block * [NumBlocks + 1];
        memcpy(tempblocks, Blocks, NumBlocks * sizeof(Block *));
        tempblocks[NumBlocks] = new Block(BlockMinSize);
        delete [] Blocks;
        Blocks = tempblocks;
        ++NumBlocks;
    }
}

}
