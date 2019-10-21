#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    /*
       filling in the contents of the root directory:
       the first 2 entries are filled in with "." and ".." references
       the other entries are empty.
       If rdsize is 2, a second block exists and should be filled as well.
       */
    void grpFillRootDir(uint32_t itotal)
    {
        soProbe(606, "%s(%u)\n", __FUNCTION__, itotal);
        
        uint32_t currentBlock = (itotal/IPB)+1;
        SODirEntry deblk[DPB];
        //assert (sizeof(deblk) == BlockSize);
        deblk[0].in = currentBlock;
        strcpy(deblk[0].name,".\n");
        deblk[0].in = currentBlock;
        strcpy(deblk[1].name,"..\n");
        deblk[1].in = currentBlock;
        for(int i=2;i<sizeof(deblk);i++){
            deblk[i].in = NullReference; //0xFFFFFFFF
            strcpy(deblk[i].name, "0\n");
        }

        soWriteRawBlock(currentBlock, &deblk);
        /* change the following line by your code */
        //return binFillRootDir(itotal);
    }
};

