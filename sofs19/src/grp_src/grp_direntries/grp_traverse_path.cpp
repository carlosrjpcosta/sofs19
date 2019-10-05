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

        /* change the following line by your code */
        return binTraversePath(path);
    }
};

