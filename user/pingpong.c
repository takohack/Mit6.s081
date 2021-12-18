#include "kernel/types.h"
#include "user/user.h"
// #include <sys/wait.h>

int main() {
    int n,p[2];
    pipe(p);
    char buf[5];

    if(fork() == 0) {
        close(0);
        dup(p[0]); //连接到pipe的读端
        close(p[1]);
        n = read(0, buf, sizeof(buf));
        printf("%d: received ping\n",getpid());
        //子进程还剩下标准0 连接到p[0] p[1] 关闭  1标准输出
        //现在需要向管道写数据
        close(p[0]);
        write(p[1], buf, n);

        exit(0);
    }else{
        close(0);
        dup(p[0]);
        close(p[0]);
        write(p[1],"h",1);
        wait(0);
        //管道读关闭
        close(p[1]);
        n = read(0, buf, sizeof(buf));
        printf("%d: received pong\n",getpid());
        close(p[1]);
    }
    exit(0);
}