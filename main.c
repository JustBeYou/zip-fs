#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>

#include "options.h"

int main(int argc, char *argv[])
{
    zipfs_options_t options = zipfs_options_init(argc, argv);
    if (options.show_help) {
        zipfs_options_help();
    }

    int ret = 0;//fuse_main(argc, argv, NULL, NULL);

    zipfs_options_cleanup();
    return ret;
}
