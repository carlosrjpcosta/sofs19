#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpResetFreeDataBlocks(uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(607, "%s(%u, %u, %u)\n", __FUNCTION__, ntotal, itotal, nbref);

        /* change the following line by your code */
        binResetFreeDataBlocks(ntotal, itotal, nbref);
        uint8_t array [BlockSize];
        memset(array, 0, BlockSize);
        uint32_t freeblocks = (itotal/IPB) + nbref + 2;
        for(uint32_t i = freeblocks; i < ntotal; i++){
            soWriteRawBlock(i, &array);
        }
    }
};

