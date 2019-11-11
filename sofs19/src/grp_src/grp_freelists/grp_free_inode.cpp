/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    void grpFreeInode(uint32_t in)
    {
        soProbe(402, "%s(%u)\n", __FUNCTION__, in);

        
        /* change the following line by your code */
        SOSuperBlock *super_b = soGetSuperBlockPointer(); // Get pointer to SuperBlock

        int inode_handler = soOpenInode(in);            // Open inode
        SOInode *inode = soGetInodePointer(inode_handler);   // Get pointer to inode

        inode->mode = INODE_FREE;                // Free inode
        inode->lnkcnt = 0;
        inode->owner = 0;
        inode->group = 0;
        inode->size = 0;
        inode->blkcnt = 0;
        inode->atime = 0;
        inode->mtime = 0;
        inode->ctime = 0;
        inode->next = NullReference;

        for(int i = 0; i < N_DIRECT; i++)
        {
            inode->d[i] = NullReference;
        }
        for(int i = 0; i < N_INDIRECT; i++)
        {
            inode->i1[i] = NullReference;
        }
        for(int i = 0; i < N_DOUBLE_INDIRECT; i++)
        {
            inode->i2[i] = NullReference;
        }

        soSaveInode(inode_handler);
        soCloseInode(inode_handler);

        int inode_handler_tail = soOpenInode(super_b->tail_idx);// Open inode
        SOInode *inode_tail = soGetInodePointer(inode_handler_tail);
        inode_tail->next = inode_tail->next++;
        soSaveInode(inode_handler_tail);
        soCloseInode(inode_handler_tail);

        super_b->ifree++;                       // Increment list of Free Inodes
        super_b->tail_idx = in;
        soSaveSuperBlock();                     // Save SuperBlock
    
    }
};

