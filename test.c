#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zip.h>

int main() {
    // "test/" should be a mounted zip-fs filesystem
    FILE *f1, *f2;
    char buf[512];

    // Check that RDWR is not allowed
    int fd = open("test/test.txt", O_RDWR);
    printf("open: %d\n", fd);
    if (fd != -1) {puts("r/w shoudn't be allowed!"); return 1;}

    // Consecutive writes
    // BUG: not yet working, must implement writing buffers
    f1 = fopen("test/f1.txt", "w");
    fprintf(f1, "this is some text(1)\n");
    fflush(f1);
    fprintf(f1, "this is some more text(2)\n");
    fflush(f1);
    fclose(f1);

    // Consecutive reads
    f1 = fopen("test/f1.txt", "r");
    fgets(buf, 100, f1);
    printf("line 1: %s\n", buf);
    fgets(buf, 100, f1);
    printf("line 2: %s\n", buf);
    fclose(f1);

    // Intermittent reads and writes
    f1 = fopen("test/f1.txt", "r");
    f2 = fopen("test/f2.txt", "w");

    fgets(buf, 5, f1);
    printf("(1) f1: %s\n", buf);
    fgets(buf, 5, f1);
    printf("(2) f1: %s\n", buf);

    fprintf(f2, "intermediary text !!!\n");
    fflush(f2);
    fclose(f2); 

    fgets(buf, 5, f1);
    printf("(3) f1: %s\n", buf);

    // Reading after writing
    f2 = fopen("test/f2.txt", "r");

    fgets(buf, 10, f2);
    printf("(4) f2: %s\n", buf);

    fclose(f1);

    return 0;
}


/*int fd = open("test/source/file1.txt", O_RDWR);
char buf[512];
ssize_t ret = read(fd, buf, 512);

printf("fd = %d\nread bytes = %zd\n", fd, ret);*/

/*int err;
zip_t* zip = zip_open("archive.zip", 0, &err);

zip_file_t* f = zip_fopen(zip, "source/file1.txt", 0);
char buf[512];
zip_fread(f, buf, 100);
printf("content: %s\n", buf);

zip_stat_t st;
zip_stat_init(&st);
zip_stat(zip, "source/nested", 0, &st);
printf("name: %s\n", st.name);

zip_fclose(f);
zip_close(zip);*/