#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillSuperBlock(const char *name, uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(602, "%s(%s, %u, %u, %u)\n", __FUNCTION__, name, ntotal, itotal, nbref);

        /*change the following line by your code
        binFillSuperBlock(name, ntotal, itotal, nbref); */

    
        SOSuperBlock sb;

        sb.magic = MAGIC_NUMBER;
        sb.version = VERSION_NUMBER;
        sb.mntstat = 0;                         // mount status (1: properly unmounted; 0: otherwise)
        sb.mntcnt = 0;                          // number of mounts since last file system check
        sb.ntotal = ntotal;                     // total number of blocks in the device

        sb.it_size = itotal/IPB;                // number of blocks that inode table comprises
        sb.itotal = itotal;                     // total number of inodes
        sb.ifree = itotal - 1;                  // number of free inodes
        sb.ihead = 1;                           // number of first free inode
        sb.itail = itotal - 1;                  // number of last free inode

        sb.dz_start = sb.it_size + 1;           // physical number of the block where the data zone starts
        sb.dz_total = ntotal - 1 - sb.it_size;  // total number of data blocks
        sb.dz_free = sb.dz_total - nbref - 1;   // number of free blocks in data zone
        
        // if se o dz_free for < 64 é tudo NULL
        if(sb.dz_free < HEAD_CACHE_SIZE){
            sb.head_blk = NullReference; // number of head reference data block
            sb.head_idx = NullReference; // first occupied position in head reference data block
            sb.tail_blk = NullReference; // number of tail reference data block
            sb.tail_idx = NullReference; // first empty position in tail reference data block
        }
        else{
            sb.head_blk = 1;
            sb.head_idx = 1;
            sb.tail_blk = nbref;
            uint32_t occupied = sb.dz_free - (HEAD_CACHE_SIZE + (nbref - 1)*RPB) + (nbref-1);
            sb.tail_idx = occupied + 1;
        }

        // cache

        // head cache

        if(sb.dz_free >= HEAD_CACHE_SIZE){
            sb.head_cache.idx = 0;
        }else{
            sb.head_cache.idx = HEAD_CACHE_SIZE - sb.dz_free;
        }        
        
        uint32_t block = nbref + 1;
        for(unsigned int i = 0; i < HEAD_CACHE_SIZE; i++){
            if(i < sb.head_cache.idx){
                sb.head_cache.ref[i] = NullReference;
            }else{
                sb.head_cache.ref[i] = block;
                block++;
            }
            
        }

        // tail cache
        sb.tail_cache.idx = 0;
        for(unsigned int i = 0; i < TAIL_CACHE_SIZE; i++){
            sb.tail_cache.ref[i] = NullReference;
        }
    }
};

