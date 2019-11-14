#include "grp_mksofs.h"

#include "core.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <iostream>
#include <math.h>

namespace sofs19
{
    void grpComputeStructure(uint32_t ntotal, uint32_t & itotal, uint32_t & nbref)
    {
        soProbe(601, "%s(%u, %u, ...)\n", __FUNCTION__, ntotal, itotal);

        /* change the following line by your code */
        // binComputeStructure(ntotal, itotal, nbref);

        // if itotal = 0, ntotal/16 should be used as the start value for itotal
        if(itotal == 0){
            itotal = (ntotal/16);
        }
        
        // itotal is always lower than or equal to ntotal/8
        if(itotal > (ntotal/8)){
            itotal = (ntotal/16);
        }

        // itotal is always greater than or equal to IPB
        if(itotal < IPB){
           itotal = (ntotal/16);
        }

        // itotal must be rounded up to be multiple of IPB
        if(itotal % IPB == 0){
            itotal = itotal/IPB;
        }
        else{
            while(itotal % IPB != 0){
                itotal = itotal + 1;
            }
        }

        //splitting data blocks between reference data blocks and free data blocks
        // if single data block remains, it is assigned to the inode table
        
        uint32_t nib = itotal/IPB;

        if((nib + 2 + HEAD_CACHE_SIZE) >= ntotal){
            nbref = 0;
        }
        else{
            if((ntotal - nib - 2 - HEAD_CACHE_SIZE) % RPB == 1) {
                itotal = itotal + 8;
                nib = itotal/IPB;
                nbref = uint32_t(ceil((double(ntotal - nib - 2 - HEAD_CACHE_SIZE) - 1) / (RPB)));
            }
            else nbref = uint32_t(ceil(double(ntotal - nib - 2 - HEAD_CACHE_SIZE) / (RPB)));
        }
    }
};

