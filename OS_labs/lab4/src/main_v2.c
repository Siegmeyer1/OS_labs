#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main() {

    FILE* tmp_file = fopen("file", "w+");
    if (!tmp_file) {
        printf("can`t open file\n");
        return -1;
    }
    int tmp_fd = fileno(tmp_file);

    char buf[160];
    gets(buf);

    pid_t pid;
    pid = fork();

    //================================entering-multiprocess-zone==================================//
    if (pid < 0) {
        printf("failed to fork\n");
        return -1;
    }
    //===================parent=======================//
    else if (pid > 0) {

        printf("trying to enter parent...\n");
        wait(NULL);
        printf("now in parent\n");

        struct stat statbuf;
        if ( fstat(tmp_fd, &statbuf) < 0 ) {
            printf("fstat error\n");
            return -1;
        }
        int size = (int)statbuf.st_size;

        char* file_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, tmp_fd, 0);
        if (file_ptr == MAP_FAILED) {
            printf("failed to map\n");
            return -1;
        }

        char c;
        for(int i = 0; i < size; i++) {
            c = file_ptr[i];
            if (('a' <= (int)c) && ((int)c <= 'z')) {
                c = c - 32;
            }
            printf("%c", c);
        }
        system("rm file");
        return 0;
    }
    //===================child========================//
    else {
        printf("now in child\n");

        if (dup2(tmp_fd, 1) < 0) {
            printf("failed to execute dup2\n");
            exit(-1);
            }
            execlp(buf, buf, NULL);
            exit(0);
        }
}