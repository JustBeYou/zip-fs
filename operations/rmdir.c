#include "all.h"
#include "../zip_utils.h"
#include "../zipfs.h"
#include "../debug.h"
#include <assert.h>
#include <errno.h>
#include <string.h>

int zipfs_rmdir(const char *path) {
    if(strcmp(path, FS_ROOT) == 0) {
        return -EACCES;
    }
    
    struct stat stbuf;
    if(zipfs_getattr(path, &stbuf, NULL)) {
        return -EEXIST;
    }

    debug(printf("[DEBUG] rmdir: not exists, ok\n"));

    zipfs_data_t* data = zipfs_get_data();
    char* proper_path = zip_dir_path_format(path);
    zip_int64_t index = zip_name_locate(data->zip_file, proper_path, 0);
    debug(assert(index >= 0));
    zip_dir_path_cleanup(proper_path);

    if(zip_delete(data->zip_file, index)) {
        debug(printf("[DEBUG] rmdir: error %s\n", zip_strerror(data->zip_file)));
        return -EACCES;
    }
    zipfs_manager_flush(&(data->zip_file));
    return 0;
}
