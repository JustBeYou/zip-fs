#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct zipfs_options {
    char* zip_filename;
    bool show_help;
} zipfs_options_t ;

#define zipfs_option_spec(str, field) \
    { str, offsetof(struct zipfs_options, field), 1 }

#define zipfs_option(field) \
    (zipfs_options_get()->field)

const zipfs_options_t* zipfs_options_init(int argc, char** argv);
const zipfs_options_t* zipfs_options_get();
void zipfs_options_help();
void zipfs_options_cleanup();

#endif
