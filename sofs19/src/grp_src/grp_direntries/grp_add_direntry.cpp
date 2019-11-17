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
        bool is_empty = false;
        __uint32_t block;
        // __uint32_t block = (parent->size/sizeof(SODirEntry))/RPB;
        __uint32_t total_blocks = parent->size / BlockSize;
        // __uint32_t blck_number = soGetFileBlock(pih,block);

        for(int i = 0; i < total_blocks; i++)
        {
            soReadFileBlock (pih, i, dir);
            for(int j = 0; j < DPB; j++)
            {
                if (strcmp(dir[j].name, name)==0){
                    throw SOException(EINVAL,__FUNCTION__);   
                }
                if (dir[j].name[0] == '\0' and is_empty == false){
                    block = soGetFileBlock(pih,i);
                    is_empty = true;
                }
            }
        }

        if(is_empty == true)
        {
            soReadFileBlock(pih,block,dir);
            for(int j = 0; j < DPB ; j++)
            {
                if (dir[j].name[0] == '\0'){
                    strcpy(dir[j].name, name);
                    dir[j].in = cin;
                    soWriteFileBlock(pih,block,dir);
                    soSaveInode(pih);
                    return;
                }
            }
        }
        block = soAllocFileBlock(pih, total_blocks);
        strcpy(dir[0].name, name);
        dir[0].in = cin;
        for(int j = 1; j < DPB; j++)
        {
            dir[0].in = NullReference;
            memset(dir[j].name,'\0',SOFS19_MAX_NAME);
        }
        
        soWriteFileBlock(pih,block,dir);
        parent->size = parent->size + sizeof(SODirEntry);
        soSaveInode(pih);
    }
};

