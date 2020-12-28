#include "manager.h"
#include "zip_utils.h"
#include "zipfs.h"
#include "debug.h"
#include <zip.h>

static zip_file_t* opened_files[MAX_FD] = {NULL};

int zipfs_manager_open(const char* path) {
    const char* proper_path = zip_path_format(path);

    for (int i = 0; i < MAX_FD; ++i) {
        if (opened_files[i] == NULL) {
            opened_files[i] = zip_fopen(zipfs_get_data()->zip_file, proper_path, 0);
            if (opened_files[i] == NULL) return NOT_FOUND;

            return i;
        }
    }

    return NOT_FOUND;
}

zip_file_t* zipfs_manager_get_by_fd(int fd) {
    if (fd < 0 || fd >= MAX_FD) return NULL;
    return opened_files[fd];
}

void zipfs_manager_close(int fd) {
    debug(printf("[DEBUG] Free fd %d %p\n", fd, opened_files[fd]));

    if (fd >= 0 && fd < MAX_FD && opened_files[fd] != NULL) {
        zip_fclose(opened_files[fd]);
        opened_files[fd] = NULL;
    }
}