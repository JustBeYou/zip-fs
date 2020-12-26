#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct zipfs_options {
    bool show_help;
} zipfs_options_t ;

#define zipfs_option(str, field) \
    { str, offsetof(struct zipfs_options, field), 1 }

zipfs_options_t zipfs_options_init(int argc, char** argv);
void zipfs_options_help();
void zipfs_options_cleanup();

#endif
