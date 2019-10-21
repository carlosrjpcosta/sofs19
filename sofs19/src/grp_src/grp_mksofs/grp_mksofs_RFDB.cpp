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
        //binResetFreeDataBlocks(ntotal, itotal, nbref);
        uint32_t array [BlockSize];
        memset(array, 0, BlockSize);
        uint32_t freeblocks = ntotal - IPB - nbref - 2; //O número de blocos livres são o número total de blocos menos os blocos ocupados (o superblock, os da tabela de inodes, os de referência (se existirem) e a raiz)
        for(uint32_t i = freeblocks; i < ntotal; i++){
            soWriteRawBlock(i, &array);
        }
    }
};

