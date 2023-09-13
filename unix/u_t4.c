#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

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

    int fd2 = open("./file2.txt", O_RDWR);
    
    char buffer[1024];
    int n = 0;
    memset(buffer, 0, sizeof(buffer));
    while( (n = read(fd2, buffer, 16)) > 0 ) {
        write(STDOUT_FILENO, buffer, n);
    }
    printf("\n");
    lseek(fd2, 0, SEEK_SET); // 将指针重新指向文件开始
    n = read(fd2, buffer, 16);  // 读16个字节
    write(STDOUT_FILENO, buffer, n); // 向标准输出写16个字节
    printf("\n");

    close(fd2);
    close(fd);
    return 0;
}