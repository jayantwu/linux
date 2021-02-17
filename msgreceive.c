#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
 
struct msg_st
{
    long int msg_type;
    char text[BUFSIZ];
};
 
int main(int argc, char **argv)
{
    int msgid = -1;
    struct msg_st data;
    long int msgtype = 1;   // 注意1
 
    // 建立消息队列
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed width error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
 
    // 从队列中获取消息，直到遇到end消息为止
    printf("----%d %d----\n", IPC_NOWAIT,  ENOMSG);
    while (1)
    {
        if (msgrcv(msgid, (void *)&data, BUFSIZ, msgtype, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed width erro: %d", errno);
            break;
        }
 
        printf("You wrote: %s\n", data.text);
 
        // 遇到end结束
        if (strncmp(data.text, "end", 3) == 0)
        {
            break;
        }
    }
 
    // 删除消息队列
    if (msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
    }
 
    exit(EXIT_SUCCESS);
}