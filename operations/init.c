#include "all.h"
#include "../zipfs.h"
#include "../options.h"
#include "../debug.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

void *zipfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
    (void)conn;

    debug(printf("[DEBUG] Init filesystem for file %s.\n", zipfs_options_get()->zip_filename));

    int error = 0;
    zipfs_data_t *data = malloc(sizeof(zipfs_data_t));
    data->zip_file = zip_open(zipfs_options_get()->zip_filename, 0, &error);
    if (error != ZIP_ER_OK || data->zip_file == NULL)
    {
        debug(printf("[DEBUG] Failed to open archive. (%d %p)\n", error, data->zip_file));
        free(data);
        exit(1);
    }

    return data;
}
