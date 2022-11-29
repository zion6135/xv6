
#if 1
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif
#define READ 0
#define WRITE 1

int func(int* val, int count)
{
    // printf("count=%d ", count);
    // for(int a=0; a < count; a++) {
    //     printf("%d ", val[a]);
    // }
    // printf("\n");

    if (count == 0) {
        printf("over\n");
        exit(0);
    }
    printf("prime %d\n", val[0]);

    int fd[2];
    pipe(fd);

    int ret = fork();
    if (ret == -1) {
        printf("fork error\n");
    } else if (ret == 0) {  // son
        close(fd[WRITE]);
        int tmp=0;
        int buf[64] = {'\0'};
        while (1)
        {
            int a = read(fd[READ], &buf[tmp], 4);
            if (4 == a) {
                // printf("[%d]read[%d]\n", a, buf[tmp]);
                tmp++;
            }
            if (a == 1 && buf[tmp] == '\n') {
                // printf("read finish flag\n");
                break;
            }
        }
        // printf("finish \n");
        close(fd[READ]);

        func(buf, tmp);

        exit(0);   // 必须有，否则会进入usertrap
    } else if (ret > 0) {  // parent
        close(fd[READ]);
        // feeds the numbers 2 through 35 into the pipeline.
        for(int i=0; i < count; i++) //2 3 4 
        {
            if (val[i] % val[0] == 0) //去掉被val[0]整除的数据
            {
                continue;
            } else {
                write(fd[WRITE], &val[i], 4);
                // printf("write=%d\n", j);
            }
        }
        // 结束标志
        char val = '\n';
        write(fd[WRITE], &val, 1);
        close(fd[WRITE]);

        wait(0);  // 等待子进程结束
        exit(0);   // 必须有，否则会进入usertrap
    }
    return 0;
}

int main()
{

    int buf[34];
    for (int i = 0; i < 34; i++)
    {
        buf[i]=i+2;
    }
    func(buf, 34);

    return 0;
}
// #endif