#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>
#include <stdio.h>
#include <assert.h>

#include "options.h"

static const struct fuse_opt options_spec[] = {
        zipfs_option("-h", show_help),
        zipfs_option("--help", show_help),
        FUSE_OPT_END
};

static void show_help(const char* program_name) {
    printf("Usage: %s [options] <mountpoint>\n\n", program_name);
}

static struct fuse_args fuse_args;
static zipfs_options_t options;
const zipfs_options_t zipfs_options_init(int argc, char** argv) {
   fuse_args.argc = argc;
   fuse_args.argv = argv;
   fuse_args.allocated = 0;

   assert(fuse_opt_parse(&fuse_args, &options, options_spec, NULL) != -1);

   return options;
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
