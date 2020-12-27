#include "all.h"
#include "../manager.h"
#include "../debug.h"

int zipfs_release(const char *path, struct fuse_file_info *fi) {
    debug(printf("[DEBUG] close: %zu\n", fi->fh));
    zipfs_manager_close(fi->fh);
    return 0;
}