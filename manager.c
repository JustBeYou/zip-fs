#include "manager.h"
#include "zip_utils.h"
#include "zipfs.h"
#include "options.h"
#include "debug.h"
#include <string.h>
#include <stdlib.h>
#include <zip.h>

static zip_file_t* opened_files[MAX_FD] = {NULL};
static char* names[MAX_FD] = {NULL};

int zipfs_manager_open(const char* path) {
    const char* proper_path = zip_path_format(path);

    for (int i = 0; i < MAX_FD; ++i) {
        if (opened_files[i] == NULL) {
            opened_files[i] = zip_fopen(zipfs_get_data()->zip_file, proper_path, 0);
            if (opened_files[i] == NULL) {
                debug(printf("[DEBUG] manager open error: %s\n", zip_strerror(zipfs_get_data()->zip_file)));
                return NOT_FOUND;
            }
            names[i] = strdup(path);

            return i;
        }
    }

    return NOT_FOUND;
}

zip_file_t* zipfs_manager_get_by_fd(int fd) {
    if (fd < 0 || fd >= MAX_FD) return NULL;
    return opened_files[fd];
}

char* zipfs_manager_get_name(int fd){
    if(fd < 0 || fd >= MAX_FD) return NULL;
    return names[fd];
}

void zipfs_manager_close(int fd) {
    debug(printf("[DEBUG] Free fd %d %p\n", fd, opened_files[fd]));

    if (fd >= 0 && fd < MAX_FD && opened_files[fd] != NULL) {
        zip_fclose(opened_files[fd]);
        opened_files[fd] = NULL;
        free(names[fd]);
        names[fd] = NULL;
    }
}


// TODO: is this even legit?
void zipfs_manager_flush(zip_t** zip_ptr) {
    char* temp_names[MAX_FD] = {NULL};
    for(int i = 0; i < MAX_FD; ++i) {
        if(names[i] != NULL) {
            temp_names[i] = strdup(names[i]);
            zipfs_manager_close(i);
        }
    }
    zip_close(*zip_ptr);
    
    int error = 0;
    *zip_ptr = zip_open(zipfs_options_get()->zip_filename, 0, &error);
    if (error != ZIP_ER_OK || *zip_ptr == NULL)
    {
        debug(printf("[DEBUG] Failed to flush zip. (%d %p)\n", error, *zip_ptr));
        exit(1);
    }

    for(int i = 0; i < MAX_FD; ++i) {
        if(temp_names[i] != NULL) {
            zipfs_manager_open(temp_names[i]);
            free(temp_names[i]);
        }
    }
}
