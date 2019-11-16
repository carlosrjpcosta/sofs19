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

        else if (fbn >= N_DIRECT && fbn < doubleIndirectStart) {
            blk = grpAllocIndirectFileBlock(ip, fbn - N_DIRECT);
        }

        else {
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
        uint32_t i1idx = afbn/RPB;
        uint32_t ref = afbn % RPB;
        uint32_t buf[RPB];
        uint32_t blk;

        for (uint32_t i = 0; i < RPB; i++) {
            buf[i] = NullReference;
        }
        
        if (ip->i1[i1idx] == NullReference) {
        
            ip->i1[i1idx] = soAllocDataBlock();

            blk = soAllocDataBlock();
            buf[ref] = blk;
            soWriteDataBlock(ip->i1[i1idx], buf);
            ip->blkcnt += 2;
        }
        else {
            blk = soAllocDataBlock();
            soReadDataBlock(ip->i1[i1idx], buf);
            buf[ref]= blk;
            soWriteDataBlock(ip->i1[i1idx], buf);
            ip->blkcnt++;
        }
        return blk;
    }
#endif

    /* ********************************************************* */

#if true
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);
        uint32_t i2idx = afbn/(RPB*RPB);
        uint32_t ref1 = afbn/(RPB - (afbn / RPB));
        uint32_t ref2 = afbn%RPB;
        uint32_t buf1[RPB];
        uint32_t buf2[RPB];
        uint32_t blk;
        
        for (uint32_t i = 0; i < RPB; i++) {
            buf1[i] = NullReference;
            buf2[i] = NullReference;
        }

        if (ip->i2[i2idx] == NullReference) {

            ip->i2[i2idx] = soAllocDataBlock();

            uint32_t tmp = soAllocDataBlock();
            buf1[ref1]= tmp; 
            soWriteDataBlock(ip->i2[i2idx],buf1);
            
            blk = soAllocDataBlock();
            buf2[ref2] = blk;
            soWriteDataBlock(tmp,buf2);
            ip->blkcnt += 3;
        }
        else {
            soReadDataBlock(ip->i2[i2idx], buf1);
            if (buf1[ref1]==NullReference){
                uint32_t tmp = soAllocDataBlock();
                buf1[ref1]= tmp; 
                soWriteDataBlock(ip->i2[i2idx],buf1);
            
                blk = soAllocDataBlock();
                buf2[ref2] = blk;
                soWriteDataBlock(tmp,buf2);
                ip->blkcnt += 2;
            } else{
                blk = soAllocDataBlock();
                soReadDataBlock(buf1[ref1], buf2);
                buf2[ref2]= blk;
                soWriteDataBlock(buf1[ref1], buf2);
                ip->blkcnt++;
            }
        }
        return blk;
    }
#endif
};

