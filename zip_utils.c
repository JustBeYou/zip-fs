#include "zip_utils.h"
#include "debug.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

char* zip_dir_path_format(const char* path) {
    size_t len = strlen(path);
    char* proper_dir_name = malloc(sizeof(char) * (len + 1));

    strcpy(proper_dir_name, path);
    
    debug(assert(path[len - 1] != '/'));
    if (path[len - 1] != '/') strcat(proper_dir_name, "/");

    return proper_dir_name;
}

void zip_dir_path_cleanup(char* path) {
    free(path);
}

int zip_is_dir(const char* dir_name, zip_t* zip) {
    char* proper_dir_name = zip_dir_path_format(dir_name);

    debug(printf("[DEBUG] Found %zu files in archive (%p).\n", zip_get_num_entries(zip, 0), zip));

    int ret = 0;
    for (zip_uint64_t i = 0; i < zip_get_num_entries(zip, 0); ++i) {
        const char *filename = zip_get_name(zip, i, 0);
        
        debug(printf("[DEBUG] Compare dir names %s - %s - %s\n", proper_dir_name, filename, strstr(filename, proper_dir_name)));
        
        if (strstr(filename, proper_dir_name) != NULL) {
            ret = 1; 
            break;
        }
    }

    zip_dir_path_cleanup(proper_dir_name);
    return ret;
}