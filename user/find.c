#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int readDir_findObj(char* path, char* obj)
{
    // printf("path[%s] obj[%s]\n", path, obj);
// (1) .
    int fd = open(path, 0);
// (2) ./
    char stat_buffer[64] = {'0'};
    strcpy(stat_buffer, path);
    char* p = stat_buffer+strlen(stat_buffer);
    *p++ = '/';

    struct dirent de;
    struct stat st;
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        // (3) ./test
        memmove(p, de.name, DIRSIZ);
        stat(stat_buffer, &st);
        switch (st.type)
        {
            case T_DIR: //目录
                {
                    //不处理.和..目录
                    if(!strcmp(de.name, ".") ||!strcmp(de.name, "..")) {
                        continue;
                    }
                    // printf("dir %d %d %d [%s]\n", st.type, st.ino, st.size, de.name);

                    char pathtmp[64] = {'\0'};
                    strcpy(pathtmp, stat_buffer);
                    memmove(pathtmp+strlen(path), "/", 1);
                    memmove(pathtmp+strlen(path)+1, de.name, strlen(de.name));

                    readDir_findObj(pathtmp, obj);
                }
                break;
            case T_FILE: //文件
                // printf("file %d %d %d[%s]\n", st.type, st.ino, st.size, de.name);
                // find obj file
                if (!strcmp(de.name, obj)) {
                    printf("file [%s] find at [%s]\n", de.name, path);
                }
                break;
            default:
                break;
        }

    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("please input arg\n");
        exit(0);
    }
    char* objname = argv[1];
// find objname 在当前目录下
    readDir_findObj(".", objname);
    exit(0);
    return 0;
}