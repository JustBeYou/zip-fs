#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zip.h>
/*
#define FA_RDONLY       0x01            // FILE_ATTRIBUTE_READONLY
#define FA_DIREC        0x10            // FILE_ATTRIBUTE_DIRECTORY

int main(){
    int err = 0;
    zip_t* zip = zip_open("Debug/archive.zip", 0, &err);
    zip_int64_t index = zip_name_locate(zip, "test1.txt", 0);

    zip_uint32_t attr;
    if(zip_file_get_external_attributes(zip, index, ZIP_FL_UNCHANGED, NULL, &attr) == -1){
        printf("Fail");
        zip_close(zip);
        return -1;
    }
    mode_t m = S_IRUSR | S_IRGRP | S_IROTH;
    if (0 == (attr & FA_RDONLY))
      m |= S_IWUSR | S_IWGRP | S_IWOTH;

    if (attr & FA_DIREC)
      m =  S_IXUSR | S_IXGRP | S_IXOTH;
    printf("%u", attr & S_IRUSR);
    zip_close(zip);
    return 0;
}

*/

int main() {
    int err = 0;
    zip_t* zip = zip_open("Debug/archive.zip", 0, &err);
    zip_file_t* f = zip_fopen(zip, "test.txt", 0);

    // Calling ftell will also break fread() after
    printf("offset: %zd\n", zip_ftell(f));
    printf("error: %s | %s\n", zip_strerror(zip), zip_file_strerror(f));

    char buf[512] = {0};
    zip_int64_t bytes;/*
    while ((bytes = zip_fread(f, buf, 511) > 0)) {
        //printf("Read %zd %s\n", bytes, buf);
    }*/

    bytes = zip_fread(f, buf, 511);

    printf("%s\n", buf);

    printf("offset: %zd\n", zip_ftell(f));
    printf("error: %s | %s\n", zip_strerror(zip), zip_file_strerror(f));

    zip_fclose(f);
    zip_close(zip);

    return 0;
}

/*
int main() {
int err = 0;
zip_t* zip = zip_open("Debug/archive.zip", 0, &err);
// pentru fisierele mari (mai mult decat cateva sute de bytes)
// ftell/fseek dau Operation not supported
zip_file_t* f = zip_fopen(zip, "test5.txt", 0);

printf("cursor: %zu\n", zip_ftell(f));
printf("err: %s %s\n", zip_strerror(zip), zip_file_strerror(f));
char buf[512];
zip_fread(f, buf, 10);
// In docs scrie ca poti sa dai ftell/fseek doar pe 
// fisiere uncompressed. fread ar trebui sa ruleze decompresia
// dar degeaba, tot asa da si daca citesc tot

//for (int i = 0; i < 50; i++)
//zip_fread(f, buf, 512);
printf("content: %s\n", buf);
printf("cursor: %zu\n", zip_ftell(f));
printf("err: %s %s\n", zip_strerror(zip), zip_file_strerror(f));

zip_fseek(f, 0, SEEK_SET);
printf("cursor: %zu\n", zip_ftell(f));
printf("err: %s %s\n", zip_strerror(zip), zip_file_strerror(f));
/*
zip_stat_t st;
zip_stat_init(&st);
zip_stat(zip, "source/nested", 0, &st);
printf("name: %s\n", st.name);
zip_fclose(f);
zip_close(zip);
    return 0;
}

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


int fd = open("test/source/file1.txt", O_RDWR);
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