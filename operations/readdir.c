#include "all.h"
#include "../zipfs.h"
#include "../zip_utils.h"
#include "../debug.h"
#include <zip.h>
#include <string.h>
#include <stdlib.h>

int zipfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                    off_t offset, struct fuse_file_info *fi,
                    enum fuse_readdir_flags flags) {

    // TODO: add permission checking

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);

    const int is_root = strcmp(path, FS_ROOT) == 0;
    char* real_path = zip_dir_path_format(path);
    const size_t real_path_len = strlen(real_path);

    zipfs_data_t* data = zipfs_get_data();
    zip_uint64_t entries = zip_get_num_entries(data->zip_file, 0);

    debug(printf("[DEBUG] readdir: %s %s (root? %d)\n", path, real_path, is_root));

    // There is no directory structure in zlib, so we need to iterate
    // over all file entries and choose only direct children of the read directory
    // Example of an entry list (assume we are reading aaa/bbb/):
    //
    // aaa/
    // aaa/file1
    // aaa/file2
    // aaa/bbb/         <- should list files below
    // aaa/bbb/file1
    // aaa/bbb/file2
    // aaa/bbb/ccc/     <- until here
    // aaa/bbb/ccc/file1
    //

    for (zip_uint64_t i = 0; i < entries; ++i) {
        const char* filename = zip_get_name(data->zip_file, i, 0);
        debug(printf("[DEBUG] readdir: compare %s - %s\n", real_path, filename));

        if (is_root || strncmp(real_path, filename, real_path_len) == 0) {
            // should not be itself
            if (!is_root && filename[real_path_len] == '\0') continue;
            debug(printf("[DEBUG] readdir: not self\n"));

            const char* next_slash = strchr(filename + real_path_len, '/');
            debug(printf("[DEBUG] readdir: next slash = %s\n", next_slash));

            // should print only direct children
            if (next_slash != NULL && *(next_slash + 1) != '\0') continue;
            debug(printf("[DEBUG] readdir: found entry in dir %s\n", filename + real_path_len));

            // remove the last '/' from filename
            const char* filename_to_print = filename + real_path_len;
            size_t filename_to_print_len = strlen(filename_to_print);

            char* sanitized_filename = malloc(sizeof(char) * (filename_to_print_len + 1));
            strcpy(sanitized_filename, filename_to_print);

            if (sanitized_filename[filename_to_print_len - 1] == '/') {
                sanitized_filename[filename_to_print_len - 1] = '\0';
            }

            filler(buf, sanitized_filename, NULL, 0, 0);
            free(sanitized_filename);
        }
    }

    zip_dir_path_cleanup(real_path);
    return 0;
}