#include "zip_utils.h"
#include "debug.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// all zip directories have a '/' at the end
char* zip_dir_path_format(const char* path) {
    // assure zip path format first
    const char* zip_path = zip_path_format(path);

    size_t len = strlen(zip_path);
    // +1 for '/' and +1 for '\0'
    char* proper_dir_name = malloc(sizeof(char) * (len + 2));

    if (strcmp(path, FS_ROOT) == 0) {
        proper_dir_name[0] = '\0';
        return proper_dir_name;
    }
    strcpy(proper_dir_name, zip_path);
    
    debug(assert(zip_path[len - 1] != '/'));
    if (zip_path[len - 1] != '/') strcat(proper_dir_name, "/");

    return proper_dir_name;
}

void zip_dir_path_cleanup(char* path) {
    free(path);
}

int zip_is_dir(const char* dir_name, zip_t* zip) {
    // Add a '/' to the end of the filename
    // All directories in zip files terminate with this char
    char* proper_dir_name = zip_dir_path_format(dir_name);

    zip_stat_t result;
    zip_stat_init(&result);

    int ret = 0;
    int stat_ret = zip_stat(zip, proper_dir_name, 0, &result);
    if (stat_ret == ZIP_ER_OK && result.name != NULL) ret = 1;

    zip_dir_path_cleanup(proper_dir_name);
    return ret;
}