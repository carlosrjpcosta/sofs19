/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2009-2019
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
    uint32_t grpAllocDataBlock()
    {
        soProbe(441, "%s()\n", __FUNCTION__);

        SOSuperBlock *sb = soSBGetPointer();

        if(sb -> brcache.idx == BLOCK_REFERENCE_CACHE_SIZE){
            soReplenishBRCache();
        }

        if(sb -> brcache.idx == NullReference){
            throw SOException(ENOSPC, __FUNCTION__);
        }

        uint32_t index = sb -> brcache.idx;
        uint32_t ref = sb -> brcache.ref[index];
        sb -> brcache.ref[index] = NullReference;

        sb -> brcache.idx++;
        sb -> dz_free--;

        soSBSave();

        return ref;

        /* change the following line by your code */
        // return binAllocDataBlock();
    }
};

