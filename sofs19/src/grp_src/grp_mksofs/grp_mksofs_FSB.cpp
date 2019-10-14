#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillSuperBlock(const char *name, uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(602, "%s(%s, %u, %u, %u)\n", __FUNCTION__, name, ntotal, itotal, nbref);

        /* change the following line by your code 
        binFillSuperBlock(name, ntotal, itotal, nbref); */


        SOSuperBlock sb;

        sb.magic = MAGIC_NUMBER;
        sb.version = VERSION_NUMBER;
        sb.mntstat = 0; // mount status (1: properly unmounted; 0: otherwise)
        sb.mntcnt = 0; // number of mounts since last file system check
        sb.ntotal = ntotal; // total number of blocks in the device
        sb.it_size = itotal/; // number of blocks that inode table comprises
        sb.itotal = itotal; // total number of inodes
        sb.ifree = ; // number of free inodes
        sb.ihead = ; // number of first free inode
        sb.itail = ; // number of last free inode
        sb.dz_free = ntotal-1; // number of free blocks in data zone
        sb.dz_start = 1; // physical number of the block where the data zone starts
        sb.dz_total = ntotal - nbref; // total number of data blocks
        sb.head_blk = ; // number of head reference data block
        sb.head_idx = ; // first occupied position in head reference data block
        sb.tail_blk ; // number of tail reference data block
        sb.tail_idx = ; // first empty position in tail reference data block
    }
};

