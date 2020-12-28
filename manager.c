#include "manager.h"
#include "zip_utils.h"
#include "zipfs.h"
#include "options.h"
#include "debug.h"
#include <string.h>
#include <stdlib.h>
#include <zip.h>
#include <assert.h>
#include <stdbool.h>

static zip_file_t* opened_files[MAX_FD] = {NULL};
static char* names[MAX_FD] = {NULL};
static bool for_writing[MAX_FD] = {false};

// libzip does not allow intermitent writes and flushes
// so we need to do some kind of buffering when writing files
typedef struct write_buffer {
    char* memory;
    size_t size;
    FILE* file;
} write_buffer_t;
static write_buffer_t writing_buffers[MAX_FD];

// BUG: at the moment there is no way for using O_APPEND or writing multiple times
// to the same file
// TODO: the buffering mechanism described above must be implemented

static int open_at_fd(const char* path, int fd, bool writing) {
    debug(assert(opened_files[fd] == NULL && names[fd] == NULL));

    const char* proper_path = zip_path_format(path);
    opened_files[fd] = zip_fopen(zipfs_get_data()->zip_file, proper_path, 0);
    if (opened_files[fd] == NULL) {
        debug(printf(DEBUG_MSG "manager open error: %s\n", zip_strerror(zipfs_get_data()->zip_file)));
        return NOT_FOUND;
    }
    names[fd] = strdup(path);
    for_writing[fd] = writing;
    // TODO: initialize writing buffers if it is the case

    return fd;
}

int zipfs_manager_open(const char* path, bool writing) {
    for (int i = 0; i < MAX_FD; ++i) {
        if (opened_files[i] == NULL) {
            return open_at_fd(path, i, writing);
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
    debug(printf(DEBUG_MSG "Free fd %d %p\n", fd, opened_files[fd]));

    if (fd >= 0 && fd < MAX_FD && opened_files[fd] != NULL) {
        zip_fclose(opened_files[fd]);
        opened_files[fd] = NULL;
        free(names[fd]);
        names[fd] = NULL;
        for_writing[fd] = false;
        // TODO: cleanup writing buffers (don't forget to flush)
    }
}


// TODO: is this even legit?
// The is no straight way to reading/writing to files again after one write
// So we have to close all files, close the archive and reopen it
// Here we also save the entire state (fds, offsets, names, etc.)
void zipfs_manager_flush(zip_t** zip_ptr) {
    char* temp_names[MAX_FD] = {NULL};
    zip_int64_t offsets[MAX_FD] = {0};
    bool temp_for_writing[MAX_FD] = {false};

    // save filenames, offsets, writing status and close
    for(int i = 0; i < MAX_FD; ++i) {
        if(names[i] != NULL) {
            temp_names[i] = strdup(names[i]);
            temp_for_writing[i] = for_writing[i];

            // ftell only applied for files opened for reading
            if (for_writing[i]) offsets[i] = zip_ftell(opened_files[i]);
            else                offsets[i] = 0;
            // TODO: save data in writing buffers

            debug(assert(offsets[i] >= 0));
    
            debug(printf(DEBUG_MSG "Save state of %s (off %zu)\n", names[i], offsets[i]));

            zipfs_manager_close(i);
        }
    }
    zip_close(*zip_ptr);
    
    // reopen
    int error = 0;
    *zip_ptr = zip_open(zipfs_options_get()->zip_filename, 0, &error);
    if (error != ZIP_ER_OK || *zip_ptr == NULL)
    {
        debug(printf(DEBUG_MSG "Failed to flush zip. (%d %p)\n", error, *zip_ptr));
        exit(1);
    }

    // restore state
    for(int i = 0; i < MAX_FD; ++i) {
        if(temp_names[i] != NULL) {
            // TODO: be sure that writing buffers are reloaded
            open_at_fd(temp_names[i], i, temp_for_writing[i]);
            zip_fseek(opened_files[i], offsets[i], 0);
            free(temp_names[i]);
        }
    }
}
