#include "all.h"
#include "../zipfs.h"
#include "../zip_utils.h"
#include "../debug.h"
#include <errno.h>
#include <string.h>

int zipfs_mkdir(const char *path , mode_t mode) {
    struct stat stbuf;
    if(!zipfs_getattr(path, &stbuf, NULL)) {
        return -EEXIST;
    }
    debug(printf(DEBUG_MSG "mkdir: not exists, ok\n"));

    zipfs_data_t* data = zipfs_get_data();
    if(zip_dir_add(data->zip_file, zip_path_format(path), 0) < 0) {
        return -EACCES;
    }
    zipfs_manager_flush(&(data->zip_file));
    return 0;
}
