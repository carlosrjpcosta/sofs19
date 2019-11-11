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
    uint32_t grpDeleteDirEntry(int pih, const char *name)
    {
        soProbe(203, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        //soCheckInodeHandler(pih, __FUNCTION__);
        //No need to check Parent Inode Handler (pih)
        //It is assumed that pih is a valid handler of a inode where the user has write access
        SOInode* inode= soGetInodePointer(pih);
        SODirEntry deblk[DPB];
        uint32_t tfb= inode->size / BlockSize; // Total number of file blocks
        
        for (uint32_t fbn= 0; fbn<tfb; fbn++){
            soReadFileBlock (pih, fbn, deblk);
            for (uint32_t de= 0; de<DPB; de++){
                uint32_t din= deblk[de].in;
                if (strcmp(deblk[de].name, name)==0){
                    char tmp= deblk[de].name[0];
                    deblk[de].name[0]= '\0';
                    deblk[de].name[SOFS19_MAX_NAME]= tmp;
                    deblk[de].in= NullReference;
                    soWriteFileBlock(pih,fbn,deblk);
                    return din;    
                }
            }
        }
        throw SOException(ENOENT,__FUNCTION__); // Name does not exist
    }
};

