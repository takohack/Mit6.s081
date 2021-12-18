#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *key) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) { //这里打开的是一个目录
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) { //用fstat打开这个目录node
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    //这里st.type = 1
    switch (st.type) {
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("ls: path too long\n");
            break;
        }
        //先把绝对路径拷贝到buf里
        strcpy(buf, path);
        // printf("2--->%s\n", buf);
        //指针加上路径长度 如.就是1 然后加上/ 就表示路径 p移到buf strlen(buf)长度
        //现在p就是绝对路径后面那个位置
        p = buf + strlen(buf);
        // *p++符号整体对外表现的值是*p的值，运算完成后p再加1.
        *p++ = '/';
        // printf("3--->%c\n", *p);
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            //p是相对路径 buf是绝对路径
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            // printf("4--->%s\n", buf);
            // p 是buf 传入路径/(这一部分)
            if (stat(buf, &st) < 0) {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }

            switch (st.type) {
            case T_FILE:
                if (!strcmp(p, key)) {
                    printf("%s\n", buf);
                }
                break;

            case T_DIR:
                if(!strcmp(p,".") || !strcmp(p,".."))
                    break;
                find(buf,key);
                break;
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc == 3) {
        find(argv[1], argv[2]);
        exit(0);
    }
    else if (argc == 2){
        find(".",argv[1]);
        exit(0);
    }
    exit(0);
}