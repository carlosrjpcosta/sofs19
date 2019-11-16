#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "direntries.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


namespace sofs19
{
        /* ********************************************************* */
#if true
    static uint32_t recTraversePath(char *path);
#endif
    /* ********************************************************* */
    
    uint32_t grpTraversePath(char *path) {
        soProbe(221, "%s(%s)\n", __FUNCTION__, path);
        if (strcmp(strndupa(path,1),"/")){
            throw SOException(EINVAL, __FUNCTION__);
        }

        return recTraversePath (path);        
        /* change the following line by your code */
        //return binTraversePath(path);
    }

#if true
    static uint32_t recTraversePath(char *path){
        
        char* dname = dirname(strdupa(path));
        char* bname = basename(strdupa(path));
        if (!strcmp(dname,"/")){
            if (!strcmp(dname,bname)){
                return 0;
            }
            int ih = soOpenInode(0);
            SOInode* ip = soGetInodePointer(ih);
            //if the process that calls the operation does not have traverse (x) permission on all the components of the path, 
            //with exception of the rightmost one, error EACCES is thrown; 
            if (!((ip->owner == getuid() || ip->group == getgid()) && soCheckInodeAccess(ih,X_OK))){
                throw SOException(EACCES, __FUNCTION__);
            }
            uint32_t in = soGetDirEntry(ih,bname);
            soCloseInode(ih);
            return in;
        }
        int ih = soOpenInode(recTraversePath(dname));
        SOInode* ip = soGetInodePointer(ih);

        //if one of the path components, with the exception of the rightmost one, 
        //is not a directory, error ENODIR is thrown
        if(!S_ISDIR(ip->mode)){
            throw SOException(ENOTDIR, __FUNCTION__);
        }
        //if the process that calls the operation does not have traverse (x) permission on all the components of the path, 
        //with exception of the rightmost one, error EACCES is thrown; 
        if (!((ip->owner == getuid() || ip->group == getgid()) && soCheckInodeAccess(ih,X_OK))){
            throw SOException(EACCES, __FUNCTION__);
        }

        uint32_t in = soGetDirEntry(ih,bname);
        soCloseInode(ih);
        return in;
    }
#endif    
};

