#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    
    int fd = open("./test_file1", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    printf("fd: %d\n", fd);
    char buf[] = "hello world";
    write(fd, buf, sizeof(buf));
    off_t cur_pos = lseek(fd, 1000, SEEK_SET);
    write(fd, buf, sizeof(buf));
    cur_pos = lseek(fd, 2000, SEEK_SET);
    write(fd, buf, sizeof(buf));

    close(fd);
    return 0;
}