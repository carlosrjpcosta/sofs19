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
        return binGetDirEntry(pih, name);
    }
};

