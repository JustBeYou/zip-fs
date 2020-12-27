#ifndef _ZIP_UTILS_H_
#define _ZIP_UTILS_H_

#include <zip.h>

#define FS_ROOT "/"
#define ZIP_ROOT ""

#define zip_path_format(path) (path[0] == '/' ? path + 1 : path)
char* zip_dir_path_format(const char* path);
void zip_dir_path_cleanup(char* path);
int zip_is_dir(const char* dir_name, zip_t* zip);

#endif