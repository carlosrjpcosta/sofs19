#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <string.h>

namespace sofs19
{
    void grpFillReferenceDataBlocks(uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(605, "%s(%u, %u, %u)\n", __FUNCTION__, ntotal, itotal, nbref);

        /* change the following line by your code */
        return binFillReferenceDataBlocks(ntotal, itotal, nbref);
    }
};

