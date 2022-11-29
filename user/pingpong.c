#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// 

int main()
{
    int fd[2];
    pipe(fd); //fd[1]-->fd[0]
    
    int fd1[2];
    pipe(fd1);
    int ret = fork(); // get child pid
    if (ret == -1) {
        printf("fork error\n");
    } else if(ret == 0){
        int buf[8] = {'\0'};
        close(fd[1]);
        read(fd[0], buf, 5);
        printf("%d: receive %s", getpid(), buf);

        close(fd1[0]);
        write(fd1[1], "pong\n", 5);
        exit(0);   // 必须有，否则会进入usertrap

    } else {
        int buf[8] = {'\0'};
        close(fd[0]);
        write(fd[1], "ping\n", 5);


        close(fd1[1]);

        read(fd1[0], buf, 5);
        printf("%d: receive %s", getpid(), buf);
        

        exit(0);   // 必须有，否则会进入usertrap
    }
    return 0;
}