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

        //RPB -- References per block
        if(nbref==0)
            return; //Every reference is in the head cache
        uint32_t nextReference= (nbref + 1) + HEAD_CACHE_SIZE; //Block 0 is for the root         
        uint32_t totalDataBlocks= ntotal - (itotal/IPB); //Number of Data Blocks
        for(uint32_t i=0; i<nbref; i++){ //Goes through all Reference Data Blocks (nbref)
            uint32_t buf [RPB];
            if(i==nbref-1) // Last Reference Block
                buf[0]= NullReference;
            else
                buf[0]= i+2;     
            for(uint32_t j=1; j<RPB; j++){
                if(nextReference==totalDataBlocks-1){ // No more Data Blocks
                    for(uint32_t k=j;k<RPB;k++){
                        buf[k]= NullReference;
                    }
                    break;
                }  
                else{
                    buf[j]= nextReference;
                    nextReference++;
                }            
            }
            soWriteRawBlock(i+2+(itotal/IPB),&buf); // Writes in the disk the buffer
        }
    }
};

