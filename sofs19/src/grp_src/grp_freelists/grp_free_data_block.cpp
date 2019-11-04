/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    void grpFreeDataBlock(uint32_t bn)
    {
        soProbe(442, "%s(%u)\n", __FUNCTION__, bn);
        SOSuperBlock* sb = soGetSuperBlockPointer();
        if (sb -> tail_cache.idx == TAIL_CACHE_SIZE){
            soDepleteTailCache();
        }
        sb -> tail_cache.ref[sb -> tail_cache.idx] = bn;
        sb -> tail_cache.idx++;
        sb -> dz_free++;
        soSaveSuperBlock();



        /* change the following line by your code */
        //binFreeDataBlock(bn);
    }
};

