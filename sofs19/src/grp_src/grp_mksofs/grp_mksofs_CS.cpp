#include "grp_mksofs.h"

#include "core.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <iostream>

namespace sofs19
{
    void grpComputeStructure(uint32_t ntotal, uint32_t & itotal, uint32_t & nbref)
    {
        soProbe(601, "%s(%u, %u, ...)\n", __FUNCTION__, ntotal, itotal);

        /* change the following line by your code */
        binComputeStructure(ntotal, itotal, nbref);
    }
};

