#include "all.h"
#include "../zipfs.h"
#include "../zip_utils.h"
#include <errno.h>

int zipfs_unlink(const char* path) {
    struct stat stbuf;
    if(zipfs_getattr(path, &stbuf, NULL)) {
        return -ENOENT;
    }

    if(stbuf.st_mode & S_IFDIR) {
        return -EISDIR;
    }

    zipfs_data_t* data = zipfs_get_data();
    if(zip_delete(data->zip_file, zip_name_locate(data->zip_file, zip_path_format(path), 0))) {
        return -EACCES;
    }
    zip_flush(&(data->zip_file));
    return 0;
}

