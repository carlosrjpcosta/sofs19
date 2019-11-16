/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include <string.h>
#include <errno.h>
#include <iostream>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    void grpReplenishHeadCache(void)
    {
        soProbe(443, "%s()\n", __FUNCTION__);

        /* change the following line by your code */
        //binReplenishHeadCache();

        SOSuperBlock* sbp = soGetSuperBlockPointer();


        if(sbp->head_cache.idx != HEAD_CACHE_SIZE)
            return;

        if(sbp->head_blk == NullReference) {
            
            // All references from tail_cache are transfered to head_cache
            if(HEAD_CACHE_SIZE >= sbp->tail_cache.idx) { 
                uint32_t avaliableRefs = sbp->tail_cache.idx;
                uint32_t hcidx = HEAD_CACHE_SIZE - avaliableRefs;
                uint32_t n_bytes = avaliableRefs * sizeof (uint32_t);
                memcpy(&sbp->head_cache.ref[hcidx], &sbp->tail_cache.ref[0], n_bytes);
                memset(&sbp->tail_cache.ref[0], NullReference, n_bytes);
                sbp->head_cache.idx = hcidx;
                sbp->tail_cache.idx = 0;
            } 
            // There are more refs in tail_cache that in the head_cache filling head_cache completely
            else { 
                uint32_t letfoverRefs= sbp->tail_cache.idx - HEAD_CACHE_SIZE;
                uint32_t n_bytes1 = HEAD_CACHE_SIZE * sizeof (uint32_t);
                uint32_t n_bytes2 = letfoverRefs * sizeof (uint32_t);
                memcpy(&sbp->head_cache.ref[0], &sbp->tail_cache.ref[0], n_bytes1);
                memcpy(&sbp->tail_cache.ref[0], &sbp->tail_cache.ref[HEAD_CACHE_SIZE], n_bytes2);
                memset(&sbp->tail_cache.ref[letfoverRefs], NullReference, n_bytes1);
                sbp->head_cache.idx = 0;
                sbp->tail_cache.idx = letfoverRefs; 
            }
        }
        else {
            
            uint32_t bhead = sbp->head_blk;
            uint32_t ihead = sbp->head_idx;
            
            uint32_t avaliableRefs; 
            
            if (bhead == sbp->tail_blk){ // If the head reference DB is at the same time tail reference DB
                avaliableRefs= sbp->tail_idx - ihead;
            } 
            else {
                avaliableRefs= RPB - ihead; // Number of avaliable references in Head Reference DB
            }
            
            uint32_t ref[RPB];
    
            soReadDataBlock(bhead, ref);
            
            if (avaliableRefs > HEAD_CACHE_SIZE){
                avaliableRefs= HEAD_CACHE_SIZE;
            }

            uint32_t cidx = HEAD_CACHE_SIZE - avaliableRefs;

            memcpy (&(sbp->head_cache.ref[cidx]), &(ref[ihead]), avaliableRefs * sizeof(uint32_t));
            memset (&(ref[ihead]), NullReference, avaliableRefs * sizeof(uint32_t));

            sbp->head_cache.idx = cidx;
            sbp->head_idx+= avaliableRefs;

            
            // Head Reference DB becomes empty
            if(bhead == sbp->tail_blk && sbp->head_idx == sbp->tail_idx){
                sbp->head_blk= NullReference;
                sbp->head_idx= NullReference;
                sbp->tail_blk= NullReference;
                sbp->tail_idx= NullReference;
                memset(&(ref[0]), NullReference, RPB * sizeof(uint32_t));
                soFreeDataBlock(bhead);
            }        
            else if(sbp->head_idx == RPB){
               
                sbp->head_blk= ref[0];
                sbp->head_idx= 1;
                memset(&(ref[0]), NullReference, RPB * sizeof(uint32_t));
                soFreeDataBlock(bhead);
            }
            soWriteDataBlock(bhead,ref);  
        }
        soSaveSuperBlock();
    }
};

