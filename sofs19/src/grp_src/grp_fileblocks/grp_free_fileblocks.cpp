#include "grp_fileblocks.h"

#include "freelists.h"
#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <inttypes.h>
#include <errno.h>
#include <assert.h>
#include <iostream>

using namespace std;

namespace sofs19
{
#if true
    /* free all blocks between positions ffbn and RPB - 1
     * existing in the block of references given by i1.
     * Return true if, after the operation, all references become NullReference.
     * It assumes i1 is valid.
     */
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn);

    /* free all blocks between positions ffbn and RPB**2 - 1
     * existing in the block of indirect references given by i2.
     * Return true if, after the operation, all references become NullReference.
     * It assumes i2 is valid.
     */
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn);
#endif

    /* ********************************************************* */

    void grpFreeFileBlocks(int ih, uint32_t ffbn)
    {
        soProbe(303, "%s(%d, %u)\n", __FUNCTION__, ih, ffbn);

        // /* change the following line by your code */
        SOInode* inode = soGetInodePointer(ih);
        uint32_t BRB = RPB * RPB;
        uint32_t doubleIndirectStart = N_INDIRECT * RPB + N_DIRECT;
        uint32_t doubleIndirectEnd = BRB * N_DOUBLE_INDIRECT + doubleIndirectStart;

        if (ffbn < N_DIRECT)
        {
            for(; ffbn < N_DIRECT; ffbn++)
            {
                if(inode->d[ffbn] != NullReference){
                    soFreeDataBlock(inode->d[ffbn]);
                    inode->d[ffbn] = NullReference;
                    inode->blkcnt --;
                }
            }
        }

        if (ffbn >= N_DIRECT && ffbn < doubleIndirectStart) {
            while (ffbn < doubleIndirectStart){
                uint32_t i1= (ffbn-N_DIRECT) / RPB;
                if(inode->i1[i1] != NullReference && grpFreeIndirectFileBlocks(inode,inode->i1[i1],(ffbn-N_DIRECT)%RPB)){
                    soFreeDataBlock(inode->i1[i1]);
                    inode->i1[i1] = NullReference;
                    inode->blkcnt --;
                }
                if((ffbn-N_INDIRECT)%RPB != 0){
                    ffbn= N_DIRECT + i1 * RPB;
                }
                ffbn+= RPB;
            }
        }

        if (ffbn >= doubleIndirectStart && ffbn < doubleIndirectEnd){
            while (ffbn < doubleIndirectEnd){
                uint32_t i2= (ffbn-doubleIndirectStart) / BRB;
                if(inode->i2[i2] != NullReference && grpFreeDoubleIndirectFileBlocks(inode,inode->i2[i2],(ffbn-doubleIndirectStart)%BRB)){
                    soFreeDataBlock(inode->i2[i2]);
                    inode->i2[i2] = NullReference;
                    inode->blkcnt --;
                }
                if((ffbn-doubleIndirectStart)%BRB != 0){
                    ffbn= doubleIndirectStart + i2 * BRB;
                }
                ffbn+= BRB;
            }
        }
        
        soSaveInode(ih);

        //binFreeFileBlocks(ih, ffbn);
    }

    /* ********************************************************* */

#if true
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        // /* change the following line by your code */
        bool allNullRefs= true;
        uint32_t buf[RPB];

        soReadDataBlock(i1,buf);
        for (uint32_t ref=0; ref<RPB; ref++){
            if(allNullRefs && ref < ffbn && buf[ref] != NullReference){
                allNullRefs = false;
            } else if (ref>= ffbn) {
                if(buf[ref]!= NullReference){
                    soFreeDataBlock(buf[ref]);
                    buf[ref] = NullReference;
                    ip->blkcnt --;
                }
            }
        }
        soWriteDataBlock(i1,buf);
        return allNullRefs; 
    }
#endif

    /* ********************************************************* */

#if true
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i2, ffbn);

        /* change the following line by your code */
        // throw SOException(ENOSYS, __FUNCTION__);

        bool allNullRefs= true;
        uint32_t buf[RPB];
        uint32_t i1idx= ffbn/RPB;

        soReadDataBlock(i2, buf);
        for (uint32_t ref=0; ref<RPB; ref++){
            if(allNullRefs && ref < i1idx && buf[ref] != NullReference){
                allNullRefs = false;
            } else if (ref== i1idx) {
                if(buf[ref]!= NullReference){
                    if (grpFreeIndirectFileBlocks(ip,buf[ref],ffbn % RPB)){
                        soFreeDataBlock(buf[ref]);
                        buf[ref] = NullReference;
                        ip->blkcnt --;
                    } else {
                        allNullRefs = false;
                    }       
                }
            } else if (ref > i1idx) {
                if(buf[ref]!= NullReference && grpFreeIndirectFileBlocks(ip,buf[ref],0)){
                    soFreeDataBlock(buf[ref]);
                    buf[ref] = NullReference;
                    ip->blkcnt --;
                }
            }
        }
        soWriteDataBlock(i2,buf);
        return allNullRefs;
    }
#endif

    /* ********************************************************* */
};

