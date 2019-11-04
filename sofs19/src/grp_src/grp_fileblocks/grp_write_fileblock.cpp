#include "grp_fileblocks.h"

#include "dal.h"
#include "core.h"
#include "fileblocks.h"
#include "bin_fileblocks.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpWriteFileBlock(int ih, uint32_t fbn, void *buf)
    {
        soProbe(332, "%s(%d, %u, %p)\n", __FUNCTION__, ih, fbn, buf);

        //soCheckInodeHandler(ih, __FUNCTION__);
        //No need to check Inode Handler (ih)
        //It is assumed that ih is a valid handler of a inode
        uint32_t dbn= soGetFileBlock(ih,fbn); //Gets the Data Block Number

        if (dbn==NullReference){ 
            dbn = soAllocFileBlock(ih,fbn);
        }

        soWriteDataBlock(dbn,buf);
    }
};

