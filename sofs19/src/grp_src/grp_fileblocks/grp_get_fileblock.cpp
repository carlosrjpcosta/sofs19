#include "grp_fileblocks.h"

#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <errno.h>

namespace sofs19
{
    /* ********************************************************* */
#if true
    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t fbn);
    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t fbn);
#endif
    /* ********************************************************* */

    uint32_t grpGetFileBlock(int ih, uint32_t fbn)
    {
        soProbe(301, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /* change the following line by your code */
        //return binGetFileBlock(ih, fbn);

        uint32_t res;

        if(fbn < 0){
            throw SOException(EINVAL, __FUNCTION__);
        }

        SOInode* ind = soGetInodePointer(ih);
        if(fbn < N_DIRECT){
            res = ind -> d[fbn];
        }else if(fbn < (N_INDIRECT * RPB + N_DIRECT)){
            res = grpGetIndirectFileBlock(ind, fbn);
        }else{
            res = grpGetDoubleIndirectFileBlock(ind, fbn);
        }

        soSaveInode(ih);
        return res;
    }

    /* ********************************************************* */
#if true
    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* change the following two lines by your code */
        //throw SOException(ENOSYS, __FUNCTION__);

        uint32_t res;
        uint32_t *ref;
        uint32_t blk[RPB];
        afbn = afbn - N_DIRECT;

        if(ip -> i1[afbn] == (uint32_t)NullReference){
            res = NullReference;
            return res;
        }
        else{
            soReadDataBlock((ip -> i1[afbn/RPB]), blk);
            ref = (uint32_t *) blk;
            res = ref[afbn%RPB];
            return res;
        }
    }

#endif
    /* ********************************************************* */
#if true
    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* change the following two lines by your code */
        //throw SOException(ENOSYS, __FUNCTION__);

        uint32_t res;
        uint32_t temp;
        uint32_t *ref;
        uint32_t blk[RPB];
        afbn = afbn - N_DIRECT - (N_INDIRECT * RPB);

        if(ip -> i2[afbn] == (uint32_t)NullReference){
            res = NullReference;
            return res;
        }
        else{
            soReadDataBlock((ip -> i2[afbn/RPB]), blk);
            temp = blk[afbn];
            soReadDataBlock(temp, blk);
            ref = (uint32_t *) blk;
            res = ref[afbn%RPB];
            return res;
        }
    }

#endif
};

