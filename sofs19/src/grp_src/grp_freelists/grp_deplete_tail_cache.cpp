/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;

namespace sofs19
{
    /* only fill the current block to its end */
    void grpDepleteTailCache(void)
    {
        soProbe(444, "%s()\n", __FUNCTION__);
        /* change the following line by your code */
        //binDepleteTailCache();
        SOSuperBlock* sb= soGetSuperBlockPointer();
        
        //If cache is not full
        if (sb -> tail_cache.idx != TAIL_CACHE_SIZE){
            return;
        }

        //If there is no tail reference data block
        if (sb -> tail_blk == NullReference){
            uint32_t dbn = soAllocDataBlock();
            uint32_t buf[RPB];
            soReadDataBlock(dbn, buf);
            memset(&buf, NullReference, sizeof(buf));
            soWriteDataBlock(dbn, buf);
            sb -> head_blk = dbn;
            sb -> tail_blk = dbn; // Head and tail reference data block number is the same
            sb -> head_idx = 1;
            sb -> tail_idx = 1; 
        // Tail reference data block is full
        } else if (sb -> tail_idx == RPB){
                uint32_t dbn = soAllocDataBlock();
                uint32_t buf[RPB];
                soReadDataBlock(sb -> tail_blk, buf);
                buf[0]= dbn;
                soWriteDataBlock(sb -> tail_blk, buf);
                sb -> tail_blk = dbn; // New tail reference data block number
                memset(&buf, NullReference, sizeof(buf));
                soWriteDataBlock(dbn,buf);
                sb -> tail_idx = 1;
        }

        uint32_t avaliableSlots= RPB - sb -> tail_idx;
        // Tail cache will not be empty
        if (avaliableSlots < TAIL_CACHE_SIZE){
            uint32_t buf[RPB];
            uint32_t leftoverRefs= TAIL_CACHE_SIZE - avaliableSlots; 
            soReadDataBlock(sb->tail_blk,buf);
            memcpy (&(buf[sb->tail_idx]), &(sb->tail_cache.ref[0]), avaliableSlots * sizeof(uint32_t));
            memcpy (&(sb->tail_cache.ref[0]) , &(sb->tail_cache.ref[avaliableSlots]), leftoverRefs * sizeof(uint32_t));
            memset(&(sb->tail_cache.ref[leftoverRefs]), NullReference, (TAIL_CACHE_SIZE-leftoverRefs) * sizeof(uint32_t));
            soWriteDataBlock(sb->tail_blk,buf);
            sb->tail_idx= RPB;
            sb->tail_cache.idx= leftoverRefs;
        } else {
            uint32_t buf[RPB];
            soReadDataBlock(sb->tail_blk,buf);
            memcpy (&(buf[sb->tail_idx]), &(sb->tail_cache.ref[0]), TAIL_CACHE_SIZE * sizeof(uint32_t));
            memset(&(sb->tail_cache.ref[0]), NullReference, TAIL_CACHE_SIZE * sizeof(uint32_t));
            soWriteDataBlock(sb->tail_blk,buf);
            sb->tail_idx+= TAIL_CACHE_SIZE; 
            sb->tail_cache.idx= 0;
        }

        soSaveSuperBlock();
    }
};
