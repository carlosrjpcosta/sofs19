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
    uint32_t grpGetDirEntry(int pih, const char *name)
    {
        soProbe(201, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        /* change the following line by your code */
        // return binGetDirEntry(pih, name);

        SOInode* pi = soGetInodePointer(pih);
        SODirEntry dir[DPB];

        if(strchr(name,'/') != NULL){
            throw SOException(EINVAL, __FUNCTION__);
        }

        uint32_t nblocks = pi->size/BlockSize;

        for(uint32_t i = 0; i < nblocks; i++){
            soReadFileBlock(pih, i, dir);

            for(uint32_t j = 0; j < DPB; j++){
                if(strcmp(name, dir[j].name) == 0){
                    return dir[j].in;
                }
            }

        }
        
        return NullReference;


        


    }
};

