#ifndef _MANAGER_H_
#define _MANAGER_H_

#define MAX_FD 256
#define NOT_FOUND -1

#include <zip.h>
#include <stdbool.h>

int zipfs_manager_open(const char* path, bool writing);
zip_file_t* zipfs_manager_get_by_fd(int fd);
void zipfs_manager_close(int fd);

#endif