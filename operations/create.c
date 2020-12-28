#ifndef _CREATE_H_
#define _CREATE_H_

#include "all.h"
#include <errno.h>
#include <assert.h>
#include "../zipfs.h"
#include "../zip_utils.h"
#include "../debug.h"

int zipfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    debug(printf(DEBUG_MSG "create: %s %u\n", path, mode));

    struct stat stbuf;
    if(!zipfs_getattr(path, &stbuf, fi)) {
        return -EEXIST;
    }

    debug(printf(DEBUG_MSG "create: not exists, ok\n"));

    zipfs_data_t* data = zipfs_get_data();
    const char buf[] = {0};
    zip_source_t* s = zip_source_buffer(data->zip_file, buf, 1, 0);
    debug(assert(s != NULL));

    if (zip_file_add(data->zip_file, zip_path_format(path), s, 0) < 0) {
        zip_source_free(s);
        return -EACCES;
    }

    zipfs_manager_flush(&(data->zip_file));

    zipfs_open(path, fi);

    return 0;
}

#endif