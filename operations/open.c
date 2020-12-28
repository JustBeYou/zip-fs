#include "all.h"
#include "../debug.h"
#include "../manager.h"
#include <stdio.h>
#include <errno.h>

int zipfs_open(const char *path, struct fuse_file_info *fi) {
    debug(printf(DEBUG_MSG "open: %s\n", path));

    // This filesystem does not support R/W flag
    if (fi->flags & O_RDWR) return -EINVAL;

    int fd = zipfs_manager_open(path, fi->flags & O_WRONLY);
    if (fd == NOT_FOUND) return -ENOENT;

    fi->fh = fd;
    debug(printf(DEBUG_MSG "open done: %s %zu\n", path, fi->fh));

    // TODO: add permission checking

    return 0;
}