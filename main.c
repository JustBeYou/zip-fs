#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>

#include "options.h"
#include "operations/all.h"

static struct fuse_operations fuse_operations = {
    .init = zipfs_init,
    .destroy = zipfs_destroy,
    .getattr = zipfs_getattr,
    .open = zipfs_open,
    .release = zipfs_release,
    .read = zipfs_read,
    .readdir = zipfs_readdir,
    .opendir = zipfs_opendir,
    .releasedir = zipfs_releasedir,
    .create = zipfs_create,
    .unlink = zipfs_unlink,
    .mkdir = zipfs_mkdir,
};

int main(int argc, char *argv[])
{
    const zipfs_options_t *options = zipfs_options_init(argc, argv);
    if (options->show_help)
    {
        zipfs_options_help();
    }

    int ret = fuse_main(argc, argv, &fuse_operations, NULL);

    zipfs_options_cleanup();
    return ret;
}
