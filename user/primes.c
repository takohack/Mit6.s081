#include "kernel/types.h"
#include "user/user.h"

#define PRIME_NUM 35
#define READEND 0
#define WRITEEND 1

void child(int *pl);

int main() {
    int p[2];
    pipe(p); //创建管道
    if (fork() == 0) {
        child(p);
    } else {
        close(p[READEND]);

        for (int i = 2; i <= PRIME_NUM; i++) {
            write(p[WRITEEND], &i, sizeof(int));
        }
        close(p[WRITEEND]);
        wait((int *)0);
    }
    exit(0);
}

void child(int *pl) {
    //递归函数 basecase
    int n;
    int pr[2];
    //肯定不可能写左边的
    close(pl[WRITEEND]);
    //读第一个数字
    int read_num = read(pl[READEND], &n, sizeof(int));
    if (read_num == 0)
        exit(0);
    //开始创建右边的框框
    pipe(pr);

    if (fork() == 0) {
        //递归执行右边
        child(pr);
    } else {
        //关闭读才开始写
        close(pr[READEND]);
        int prime = n;
        printf("prime %d\n", prime);
        while (read(pl[READEND], &n, sizeof(int)) != 0) {
            if (n % prime != 0) {
                //printf("left %d\n",left);
                write(pr[WRITEEND], &n, sizeof(int));
            }
        }
        close(pr[WRITEEND]);
        wait((int *)0);
        exit(0);
    }
}