#ifndef _ALL_OPERATIONS_H_
#define _ALL_OPERATIONS_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>

void* zipfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg);
void zipfs_destroy(void* private_data);
int zipfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi);

#endif