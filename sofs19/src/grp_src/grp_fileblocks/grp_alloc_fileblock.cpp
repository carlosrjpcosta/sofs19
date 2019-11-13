#include "grp_fileblocks.h"

#include "freelists.h"
#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <errno.h>

#include <iostream>

namespace sofs19
{

#if true
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn);
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn);
#endif

    /* ********************************************************* */

    uint32_t grpAllocFileBlock(int ih, uint32_t fbn)
    {
        soProbe(302, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);
        SOInode* ip = soGetInodePointer(ih);
        uint32_t BRB = RPB * RPB;
        uint32_t doubleIndirectStart = N_INDIRECT * RPB + N_DIRECT;
        uint32_t doubleIndirectEnd = BRB * N_DOUBLE_INDIRECT + doubleIndirectStart;
        uint32_t blk;

        if (fbn < 0 || fbn >= doubleIndirectEnd) {
            throw SOException(EINVAL, __FUNCTION__);
        }

        if (fbn < N_DIRECT) {
            blk = soAllocDataBlock();
            ip->d[fbn] = blk ;
            ip->blkcnt++;
        }

        if (fbn >= N_DIRECT && fbn < doubleIndirectStart) {
            blk = grpAllocIndirectFileBlock(ip, fbn - N_DIRECT);
        }

        if (fbn >= doubleIndirectStart) {
            blk = grpAllocDoubleIndirectFileBlock(ip, fbn - doubleIndirectStart);
        }

        soSaveInode(ih);
        return blk;
        //return binAllocFileBlock(ih, fbn);
    }

    /* ********************************************************* */

#if true
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);
        uint32_t iindex = (afbn/RPB) % RPB;
        uint32_t ref = afbn % RPB;
        uint32_t db[RPB];
        uint32_t blk;

        if (ip->i1[iindex] == NullReference) {
            blk = soAllocDataBlock();
            for (uint32_t i = 0; i < RPB; i++) {
                db[i] = NullReference;
            }
            soWriteDataBlock(blk,db);
            ip->i1[iindex] = blk;
            soReadDataBlock(ip->i1[iindex], &db);
            blk = soAllocDataBlock();
            db[ref] = blk;
            ip->blkcnt += 2;
        }
        else {
            soReadDataBlock(ip->i1[iindex], &db);
            blk = soAllocDataBlock();
            ip->i1[iindex] = blk;
            ip->blkcnt++;
        }
        return blk;
        /* change the following two lines by your code */
    }
#endif

    /* ********************************************************* */

#if true
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);
        uint32_t diindex = afbn/RPB/RPB;
        uint32_t db[RPB];
        uint32_t db2[RPB];
        uint32_t blk;
        if (ip->i2[diindex] == NullReference) {
            blk = soAllocDataBlock();
            for (uint32_t i = 0; i < RPB; i++) {
                db[i] = NullReference;
                db2[i] = NullReference;
            }
            soWriteDataBlock(blk,db);
            ip->i2[diindex] = blk;
            
            
            //create indirect
            uint32_t ref = (afbn/RPB)%RPB;
            soWriteDataBlock(blk,db2);
            blk = soAllocDataBlock();
            db2[ref] = blk;
            ip->blkcnt += 2;


            soReadDataBlock(ip->i2[diindex], &db);
            ip->blkcnt++;
        }
        else {
            soReadDataBlock(ip->i2[diindex], &db);
            //create indirect
            uint32_t ref = (afbn/RPB)%RPB;
            soWriteDataBlock(blk,db2);
            blk = soAllocDataBlock();
            db2[ref] = blk;
            ip->blkcnt += 2;
        }
        return blk;
        /* change the following two lines by your code */
    }
#endif
};

