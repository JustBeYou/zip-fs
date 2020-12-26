#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>

#include "options.h"

// TODO: implement at least init, open, getattr, read
static struct fuse_operations fuse_operations = {};

int main(int argc, char *argv[])
{
    const zipfs_options_t* options = zipfs_options_init(argc, argv);
    if (options->show_help) {
        zipfs_options_help();
    }

    // TODO: this will crash as no mandatory operation is implemented
    int ret = fuse_main(argc, argv, &fuse_operations, NULL);

    zipfs_options_cleanup();
    return ret;
}
