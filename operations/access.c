#include "all.h"
#include "../zip_utils.h"
#include "../zipfs.h"
#include "../debug.h"
#include "../manager.h"
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>



int zipfs_access(const char *path, int mode) {
    //Because permissions are not yet implemented we
    //only check for the existence of the file

    struct stat stbuf;
    if(!zipfs_getattr(path, &stbuf, NULL)) {
        return -ENOENT;
    }
    return 0;
}