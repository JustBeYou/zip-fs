#include "all.h"
#include "../zip_utils.h"
#include "../zipfs.h"
#include "../debug.h"
#include <errno.h>
#include <string.h>

int zipfs_opendir(const char* path, struct fuse_file_info* fi) {
    if (strcmp(path, FS_ROOT) == 0) return 0;

    const char* real_path = zip_path_format(path);
    zipfs_data_t* data = zipfs_get_data();
    zip_stat_t result;
    zip_stat_init(&result);

    // Check if file
    int ret = zip_stat(data->zip_file, real_path, 0, &result);
    debug(printf(DEBUG_MSG "opendir stat: %d %p\n", ret, result.name));
    if (ret == ZIP_ER_OK && result.name != NULL) return -ENOTDIR;
    
    // Check if exists
    return zip_is_dir(real_path, data->zip_file) ? 0 : -ENOENT;
}