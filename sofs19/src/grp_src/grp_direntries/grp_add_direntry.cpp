#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
using namespace std;

namespace sofs19
{
    void grpAddDirEntry(int pih, const char *name, uint32_t cin)
    {
        soProbe(202, "%s(%d, %s, %u)\n", __FUNCTION__, pih, name, cin);

        /* change the following line by your code */
        // binAddDirEntry(pih, name, cin);

        SOInode* parent = soGetInodePointer(pih);
        SODirEntry dir [DPB];
        __uint32_t dir_inode = soGetDirEntry(pih,name);
        
        if(dir_inode != NullReference)
        {
            throw SOException(EINVAL,__FUNCTION__);
        }

        __uint32_t block = (parent->size/sizeof(SODirEntry))/RPB;
        __uint32_t blck_number = soGetFileBlock(pih,block);

        if(blck_number != NullReference)
        {
            blck_number = soAllocFileBlock(pih,block);
            
            dir[0].in = cin;
            if(strlen(name) < SOFS19_MAX_NAME)
            {
                strcpy(dir[0].name,name);
            }
            else
            {
                printf("\n Nome demasiado grande. \n");
            }
            
            soWriteDataBlock(blck_number, &dir);
            parent->size = parent->size + sizeof(SODirEntry);

            for(int i = 1; i < DPB; i++)
            {
                dir[i].in = NullReference;
                strcpy(dir[0].name,"\0");
            }
        }

        else
        {
            soReadFileBlock(pih, block, &dir);

            __uint32_t blck_index = (parent->size/sizeof(SODirEntry)) % RPB;
            strcpy(dir[blck_index].name,name);
            dir[blck_index].in = cin;

            soWriteDataBlock(blck_number, &dir);
            parent->size = parent->size + sizeof(SODirEntry);    
        }
        
    }
};

