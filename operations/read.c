#include "all.h"
#include "../debug.h"
#include "../manager.h"
#include <stdio.h>
#include <zip.h>
#include <errno.h>
#include <fcntl.h>

int zipfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    debug(printf(DEBUG_MSG "read: %s %zu %zu %zu\n", path, fi->fh, size, offset));

    zip_file_t* file = zipfs_manager_get_by_fd(fi->fh);
    if (file == NULL) return -ENOENT;

    struct stat file_stat;
    int stat_ret = zipfs_getattr(path, &file_stat, fi);
    if (stat_ret != 0) return stat_ret;

    if (offset > file_stat.st_size) return 0;
    size_t to_read = offset + size > file_stat.st_size ? file_stat.st_size - offset : size;

    // TODO: add permission checking

    return zip_fread(file, buf, to_read);
}