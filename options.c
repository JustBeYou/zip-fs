#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "options.h"

#define DEFAULT_ZIP_NAME "archive.zip"

static const struct fuse_opt options_spec[] = {
    zipfs_option_spec("--zip=%s", zip_filename),
    zipfs_option_spec("-h", show_help),
    zipfs_option_spec("--help", show_help),
    FUSE_OPT_END
};

static void show_help(const char* program_name) {
    printf("Usage: %s [options] <mountpoint>\n\n", program_name);
}

static struct fuse_args fuse_args;
static zipfs_options_t options;
const zipfs_options_t* zipfs_options_init(int argc, char** argv) {
    assert(argc > 0 && argv != NULL);
    fuse_args.argc = argc;
    fuse_args.argv = argv;
    fuse_args.allocated = 0;

    options.zip_filename = strdup(DEFAULT_ZIP_NAME);

    assert(fuse_opt_parse(&fuse_args, &options, options_spec, NULL) != -1);
    if (options.zip_filename == NULL) {
        zipfs_options_help();
        exit(1);
    }

    // Get absolute file path
    char abs_filename[PATH_MAX + 1];
    realpath(options.zip_filename, abs_filename);
    free(options.zip_filename);
    // This field will be freed by fuse
    options.zip_filename = strdup(abs_filename);

    return &options;
}

const zipfs_options_t* zipfs_options_get() {
    return &options;
}

void zipfs_options_help() {
    /**
     * Hacky trick to show help manual.
     * When --help is specified, first print our own file-system
     * specific help text, then signal fuse_main to show
     * additional help (by adding `--help` to the options again)
     * without usage: line (by setting argv[0] to the empty
     * string)
     **/
    show_help(fuse_args.argv[0]);
    assert(fuse_opt_add_arg(&fuse_args, "--help") == 0);
    fuse_args.argv[0][0] = '\0';
}

void zipfs_options_cleanup() {
    fuse_opt_free_args(&fuse_args);
}
