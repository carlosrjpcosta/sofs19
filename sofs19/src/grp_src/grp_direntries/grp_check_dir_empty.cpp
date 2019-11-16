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

        uint32_t temp;

        bool point = 0;
        bool pointpoint = 0;

        uint32_t fileblocks = inode->size/BlockSize;
        for(uint32_t i = 0; i < fileblocks; i++){
            temp = soGetFileBlock(ih, i);

            if(temp != NullReference){
                soReadFileBlock(ih, i, dir);

                for(uint32_t j = 0; j < DPB; j++){
                    if(dir[j].name[0] != '\0'){
                        if(!strcmp(dir[j].name, ".")){
                            if(point){ return false; }
                            point = 1;
                        }
                        else if (!strcmp(dir[j].name, "..")){
                            if(pointpoint){ return false; }
                            pointpoint = 1;
                        }
                        else{
                            return false;
                        }
                    }
                }
            }
        }

        return (point && pointpoint);
    }
};

