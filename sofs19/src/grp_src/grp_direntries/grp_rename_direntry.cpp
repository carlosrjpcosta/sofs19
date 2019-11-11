#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <string.h>
#include <errno.h>
#include <sys/stat.h>

namespace sofs19
{
    void grpRenameDirEntry(int pih, const char *name, const char *newName)
    {
        soProbe(204, "%s(%d, %s, %s)\n", __FUNCTION__, pih, name, newName);

        /* change the following line by your code */
        //binRenameDirEntry(pih, name, newName);

        SOInode *inode = soGetInodePointer(pih);
        SODirEntry dir[DPB];
        bool exists = false;
        for(uint32_t i = 0; i < inode -> blkcnt; i++){
            soReadFileBlock(pih, i, dir);
            for(uint32_t j = 0; j < DPB; j++){
                if(strcmp(dir[j].name, name) == 0){
                    exists = true;
                    strcpy(dir[j].name, newName);
                    soWriteFileBlock(pih, i, dir);
                    break;
                }
            }
        }
        if(!exists){
            throw SOException(ENOENT, __FUNCTION__);
        }
    }
};

