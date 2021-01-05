#include "all.h"
#include <../zipfs.h>
#include <errno.h>
#include <sys/time.h>

int zipfs_utimens(const char *path, const struct timespec tv[2], struct fuse_file_info *fi) {
    zip_int64_t index = zip_name_locate(zipfs_get_data()->zip_file, path, 0);
    if(index == -1){
        return -ENOENT;
    }

    struct timeval time;
    TIMESPEC_TO_TIMEVAL(&time, &tv[0]);
    time_t final_time = (time_t)time.tv_sec;

    if(zip_file_set_mtime(zipfs_get_data()->zip_file, index, final_time, 0) == -1) {
        printf("Could not change time\n");
        return -1;
    }
    return 0;
}