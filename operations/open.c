#include "all.h"
#include "../debug.h"
#include "../manager.h"
#include <stdio.h>
#include <errno.h>

int zipfs_open(const char *path, struct fuse_file_info *fi) {
    debug(printf("[DEBUG] open: %s\n", path));

    int fd = zipfs_manager_open(path);
    if (fd == NOT_FOUND) return -ENOENT;

    fi->fh = fd;
    debug(printf("[DEBUG] open done: %s %zu\n", path, fi->fh));

    // TODO: add permission checking

    return 0;
}