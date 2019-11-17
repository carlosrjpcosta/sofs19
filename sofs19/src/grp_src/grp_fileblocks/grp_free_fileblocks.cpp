#include "grp_fileblocks.h"

#include "freelists.h"
#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <inttypes.h>
#include <errno.h>
#include <assert.h>

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
        __uint32_t data_block;

        if (ffbn < N_DIRECT)
        {
            for(__uint32_t i = ffbn; i < N_DIRECT;i++)
            {
                inode->d[i] = NullReference;
                soFreeDataBlock(i);
            }
        }

        uint32_t i2 = N_INDIRECT * RPB + N_DIRECT;

        for(int i = 0; i < N_INDIRECT;i++)
        {
            bool free_in = grpFreeIndirectFileBlocks(inode,i,ffbn);
        }
        
        for(int i = 0; i < N_DOUBLE_INDIRECT;i++)
        {
            bool free_in2 = grpFreeDoubleIndirectFileBlocks(inode,i2,ffbn);
        }
        

        soSaveInode(ih);

        // binFreeFileBlocks(ih, ffbn);
    }

    /* ********************************************************* */

#if true
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        // /* change the following line by your code */
        __uint32_t count = 0;
        bool block_found = false;
        __uint32_t ref[RPB];
        __uint32_t blck = N_DIRECT;

        if(ffbn < N_DIRECT + (i1+1)*RPB)
        {
            soReadDataBlock(ip->i1[i1],ref);
            for(__uint32_t i = 0; i < RPB; i++)
            {
                if(ffbn <= blck)
                {
                    ref[i] = NullReference;
                    soFreeDataBlock(blck);
                    ip->blkcnt--;
                }
                if(ref[i] == NullReference)
                {
                    count++;
                }
                blck++;
            }
            soWriteDataBlock(ip->i1[i1],ref);

            if(count == RPB)
            {
                soFreeDataBlock(ip->i1[i1]);
                ip->blkcnt--;
            }
        }
            
        return false; 
    }
#endif

    /* ********************************************************* */

#if true
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i2, ffbn);

        /* change the following line by your code */
        // throw SOException(ENOSYS, __FUNCTION__); 
    }
#endif

    /* ********************************************************* */
};

