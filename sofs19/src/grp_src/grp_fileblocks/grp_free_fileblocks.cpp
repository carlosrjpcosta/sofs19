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
        // SOInode* inode = soGetInodePointer(ih);

        // if (ffbn < N_DIRECT)
        // {
        //     for(int i = ffbn; i < N_DIRECT;i++)
        //     {
        //         data_block = soGetFileBlock(ih,i)
        //         soWriteDataBlock(ih,data_block,NullReference);
        //     }
        // }

        // uint32_t i2 = N_INDIRECT * RPB + N_DIRECT;
        // bool free_in = grpFreeIndirectFileBlocks(inode,N_DIRECT,ffbn);
        // bool free_in2 = grpFreeDoubleIndirectFileBlocks(inode,i2,ffbn);

        // soSaveInode(ih);
        // soCloseInode(ih);

        binFreeFileBlocks(ih, ffbn);
    }

    /* ********************************************************* */

#if true
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        // /* change the following line by your code */

        // if(ffbn < (N_INDIRECT*RPB + N_DIRECT))
        // {
        //     for(int i = 0; i < N_INDIRECT; i++)
        //     {
        //         data_block = soGetFileBlock(ip->i1[i],ffbn)
        //         for(int j = 0; j < RPB; j++)
        //         {

        //         }
        //     }
        // } 
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

