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
        uint32_t BRB = RPB * RPB;
        uint32_t doubleIndirectStart = N_INDIRECT * RPB + N_DIRECT;
        uint32_t doubleIndirectEnd = BRB * N_DOUBLE_INDIRECT + doubleIndirectStart;
        uint32_t res;

        if(fbn < 0 || fbn>= doubleIndirectEnd){
            throw SOException(EINVAL, __FUNCTION__);
        }

        SOInode* ind = soGetInodePointer(ih);
        if(fbn < N_DIRECT){
            res = ind -> d[fbn];
        }else if(fbn < (N_INDIRECT * RPB + N_DIRECT)){
            res = grpGetIndirectFileBlock(ind, fbn - N_DIRECT);
        }else{
            res = grpGetDoubleIndirectFileBlock(ind, fbn - doubleIndirectStart);
        }

        return res;
    }

    /* ********************************************************* */
#if true
    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* change the following two lines by your code */
        //throw SOException(ENOSYS, __FUNCTION__);

        uint32_t blk[RPB];

        if(ip -> i1[afbn/RPB] == NullReference){
            return NullReference;
        }
        else{
            soReadDataBlock((ip -> i1[afbn/RPB]), blk);
            return blk[afbn%RPB];
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

        uint32_t temp;
        uint32_t blk[RPB];

        if(ip -> i2[afbn/(RPB*RPB)] == NullReference){
            return NullReference;
        }
        else{
            soReadDataBlock((ip -> i2[afbn/(RPB*RPB)]), blk);
            temp = blk[afbn/(RPB - ((afbn / (RPB * RPB)) * RPB))];
            soReadDataBlock(temp, blk);
            return blk[afbn%RPB];
        }
    }

#endif
};

