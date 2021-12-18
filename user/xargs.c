#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LEN 100

int main(int argc, char *argv[]) {
    char paramv[MAXARG][MAX_LEN]; //为exec准备内存 声明二维数组才会分配内存
    char bf;
    char *m[MAXARG]; //read读取字符

    while (1) {
        int count = argc - 1;
        memset(paramv, 0, MAXARG * MAX_LEN);
        //将xargs参数保存在数组中
        for (int i = 1; i < argc; i++) {
            strcpy(paramv[i - 1], argv[i]);
            // printf("----->%s\n", paramv[i - 1]);
        }

        int cursor = 0; //在读单词内的位置
        int flag = 0; //开始读单词没
        int read_result;

        while ((read_result = read(0, &bf, 1)) > 0 && bf != '\n') {
            if (bf == ' ' && flag == 1) {
                count++;

                cursor = 0;
                flag = 0;
            } else if (bf != ' ') {
                paramv[count][cursor++] = bf;
                flag = 1;
            }
        }
        if (read_result <= 0) {
            break;
        }
        for (int i = 0; i < MAXARG - 1; i++) {
            m[i] = paramv[i];
        }
        m[MAXARG - 1] = 0;
        if (fork() == 0) {
            exec(argv[1], m);
            exit(0);
        } else {
            wait(0);
        }
    }
    exit(0);
}