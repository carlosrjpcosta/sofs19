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
        //binAddDirEntry(pih, name, cin);

        SOInode* parent = soGetInodePointer(pih);
        SODirEntry dir [DPB];
        bool is_empty = false;
        uint32_t block;
        uint32_t entry;
        // __uint32_t block = (parent->size/sizeof(SODirEntry))/RPB;
        uint32_t total_blocks = parent->size / BlockSize;
        // __uint32_t blck_number = soGetFileBlock(pih,block);

        for(uint32_t i = 0; i < total_blocks; i++)
        {
            soReadFileBlock (pih, i, dir);
            for(uint32_t j = 0; j < DPB; j++)
            {
                if (strcmp(dir[j].name, name)==0){
                    throw SOException(EEXIST,__FUNCTION__);   
                }
                if (!is_empty && dir[j].name[0] == '\0'){
                    block = i;
                    entry= j;
                    is_empty = true;
                }
            }
        }

        if(is_empty)
        {
            soReadFileBlock(pih,block,dir);
            strcpy(dir[entry].name,name);
            dir[entry].in= cin;
            soWriteFileBlock(pih,block,dir);
        } else {
            block = soAllocFileBlock(pih, total_blocks);
            strcpy(dir[0].name, name);
            dir[0].in = cin;
            for(uint32_t j = 1; j < DPB; j++){
                dir[j].in = NullReference;
                memset(dir[j].name,'\0',SOFS19_MAX_NAME);
            }
            soWriteDataBlock(block,dir);
            parent->size+= BlockSize;
        }
        soSaveInode(pih); 
    }
};

