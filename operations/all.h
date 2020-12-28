#ifndef _ALL_OPERATIONS_H_
#define _ALL_OPERATIONS_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>

#define DEFAULT_DIR_PERM 0755
#define DEFAULT_FILE_PERM 0755

void *zipfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg);
void zipfs_destroy(void *private_data);

int zipfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi);

int zipfs_opendir(const char *path, struct fuse_file_info *fi);
int zipfs_releasedir(const char *path, struct fuse_file_info *fi);
int zipfs_open(const char *path, struct fuse_file_info *fi);
int zipfs_release(const char *path, struct fuse_file_info *fi);

int zipfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                  off_t offset, struct fuse_file_info *fi,
                  enum fuse_readdir_flags flags);
int zipfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);

int zipfs_create(const char *path, mode_t mode, struct fuse_file_info *fi);
int zipfs_unlink(const char *path);

int zipfs_mkdir(const char *path , mode_t mode);

#endif