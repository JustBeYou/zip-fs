#include "all.h"
#include "../zip_utils.h"
#include "../zipfs.h"
#include "../debug.h"
#include "../manager.h"
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>

int zipfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

    zip_file_t* file = zipfs_manager_get_by_fd(fi->fh);
    if (file == NULL) return -ENOENT;

    zipfs_data_t* data = zipfs_get_data();
    zip_int64_t index = zip_name_locate(data->zip_file, zip_path_format(path), 0);

    // TODO: use the associated writing buffer to store data
    // TODO: make use of "offset"
    zip_source_t* s = zip_source_buffer(data->zip_file, buf, size, 0);
    debug(assert(s != NULL));

    if (zip_file_replace(data->zip_file, index, s, 0) < 0) {
        zip_source_free(s);
        return -EACCES;
    }

    zipfs_manager_flush(&(data->zip_file));

    return size;
}
