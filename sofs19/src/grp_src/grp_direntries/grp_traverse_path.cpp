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
    uint32_t grpTraversePath(char *path)
    {
        soProbe(221, "%s(%s)\n", __FUNCTION__, path);
        uint32_t u = getuid();
        uint32_t g = getgid();
        char dirs = strdupa(path);
        char bases = strdupa(path);
        char *dname = dirname(dirs);
        char *bname = basename(bases);
        uint32_t bnode;
        if (strcpm(dname,"/") == 0) {
            return soGetDirEntry(soOpenInode(0),bname);
        }
        bnode = grpTraversePath(dname);
        if (strcpm(dname,".") == 0) {
            throw SOException(EINVAL, __FUNCTION__);
        }
        if (checkDir(bnode) == false){
            throw SOException(ENOTDIR, __FUNCTION__);
        }
        if (checkTraverse(bnode, u, g) == false){
            throw SOException(EACCES, __FUNCTION__);
        }
        return soGetDirEntry(soOpenInode(bnode),bname);

        /* change the following line by your code */
        //return binTraversePath(path);
    }

    bool checkDir(uint32_t inode){
        uint32_t ih = soOpenInode(inode);
        SOInode* ip = soGetInodePointer(ih);
        if((ip->mode & S_IFDIR) != S_IFDIR){
            return false;
        }
        return true;
    }
    bool checkTraverse(uint32_t inode, uint32_t u, uint32_t g){
        uint32_t ih = soOpenInode(inode);
        SOInode* ip = soGetInodePointer(ih);
        if ((ip->user == u) & (ip->mode & 0100 != 0100)){
            return false;
        }
        if ((ip->group == g) & (ip->mode & 0010 != 0010)){
            return false;
        }
        if ((ip->group != g) & (ip->user != u) & (ip->mode & 0001 != 0001)){
            return false
        }
        return true;
    }
};

