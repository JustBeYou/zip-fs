#ifndef _ZIPFS_H_
#define _ZIPFS_H_

#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>
#include <zip.h>

typedef struct zipfs_data {
    zip_t* zip_file;    
} zipfs_data_t;

#define zipfs_get_data() ((zipfs_data_t*) fuse_get_context()->private_data)

#endif