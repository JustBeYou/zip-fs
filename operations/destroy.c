#include "all.h"
#include "../zipfs.h"
#include "../debug.h"

#include <stdlib.h>

void zipfs_destroy(void* private_data) {
    debug(printf("[DEBUG] Destroy filesystem. (%p)\n", private_data));
    
    zipfs_data_t* data = private_data;
    zip_close(data->zip_file);
    free(data);
}