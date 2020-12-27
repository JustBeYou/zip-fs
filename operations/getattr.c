#include "all.h"
#include "../zipfs.h"
#include "../debug.h"
#include "../zip_utils.h"
#include <errno.h>
#include <zip.h>
#include <fcntl.h>
#include <stddef.h>

#define DEFAULT_DIR_PERM 0755
#define DEFAULT_FILE_PERM 0755

int zipfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    zipfs_data_t* data = zipfs_get_data();
    
    zip_stat_t result;
    const char* real_path = zip_path_format(path);

    if (real_path[0] == '\0' || zip_is_dir(real_path, data->zip_file)) {
        stbuf->st_mode = S_IFDIR | DEFAULT_DIR_PERM;
        stbuf->st_nlink = 2;
    } else {
        int ret = zip_stat(data->zip_file, real_path, 0, &result);
        if (ret != ZIP_ER_OK || result.name == NULL) return -ENOENT;

        stbuf->st_mode = S_IFREG | DEFAULT_FILE_PERM;
        stbuf->st_nlink = 1;
        stbuf->st_size = result.size;
    }

    return 0;
}