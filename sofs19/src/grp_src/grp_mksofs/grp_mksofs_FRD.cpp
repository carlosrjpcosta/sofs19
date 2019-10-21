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
        deblk[0].in = 0;
        strcpy(deblk[0].name,".");
        deblk[1].in = 0;
        strcpy(deblk[1].name,"..");
        for(int i=2;i<DPB;i++){
            deblk[i].in = NullReference; //0xFFFFFFFF
            strcpy(deblk[i].name, "");
        }
        
        soWriteRawBlock(currentBlock, &deblk);
    }
};

