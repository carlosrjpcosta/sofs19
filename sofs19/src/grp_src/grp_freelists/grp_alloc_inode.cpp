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
#include <sys/stat.h>
#include <string.h>

#include <iostream>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    uint32_t grpAllocInode(uint32_t type, uint32_t perm)
    {
        soProbe(401, "%s(0x%x, 0%03o)\n", __FUNCTION__, type, perm);
        // Type does not represent a file, directory or a symbolic link
        if(!(type == S_IFREG || type == S_IFDIR || type == S_IFLNK)){
            throw SOException(EINVAL,__FUNCTION__);
        }
        // Permission out of range [0000-0777]
        if(perm < 0000 || perm > 0777){
            throw SOException(EINVAL,__FUNCTION__);
        }
        SOSuperBlock *sbp= soGetSuperBlockPointer();
        // If there are no free inodes
        if(sbp->ifree==0){
            throw SOException(ENOSPC,__FUNCTION__);
        }
        sbp->ifree--; // Decrement the free inodes field
        uint32_t in= sbp->ihead; // Get the next free inode
        int ih= soOpenInode(in);
        //soCheckInodeHandler(ih, __FUNCTION__);
        SOInode *inode= soGetInodePointer(ih);
        sbp->ihead= inode->next;
        inode->mode= type | perm;
        inode->owner= getuid();
        inode->group= getgid();
        inode->atime= time(NULL);
        inode->mtime= time(NULL);
        inode->ctime= time(NULL);
        soSaveInode(ih);
        soCloseInode(ih);
        soSaveSuperBlock();
        return in;
    }
};

