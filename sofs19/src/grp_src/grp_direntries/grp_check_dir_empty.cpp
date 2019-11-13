#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs19
{
    bool grpCheckDirEmpty(int ih)
    {
        soProbe(205, "%s(%d)\n", __FUNCTION__, ih);

        /* change the following line by your code 
        return binCheckDirEmpty(ih);    */

        SOInode *inode = soGetInodePointer(ih);
        SODirEntry dir[DPB];

        uint32_t block_counter = 0;
        uint32_t block_number = 0;
        uint32_t temp;

        uint32_t fileblocks = inode->size/BlockSize;
        for(uint32_t i = 0; i < fileblocks; i++){
            temp = soGetFileBlock(ih, block_number);

            if(temp != NullReference){
                soReadFileBlock(ih, block_number, dir);

                for(uint32_t j = 2; j < DPB; i++){
                    if(strcmp(dir[i].name, "\0") != 0){
                        return false;
                    }
                }
                block_counter++;
            }
            block_number++;
        }
        return true;
    }
};

