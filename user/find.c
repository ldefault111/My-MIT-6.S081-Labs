#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* newfmtname(char* path) {
    static char buf[DIRSIZ + 1];
    char* p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0; // different from fmtname in ls.c
    return buf;
}

void findfile(char* path, char* name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type) {
    case T_FILE: // check this filename
        if (strcmp(newfmtname(path), name) == 0)
            printf("%s\n", path);
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ); // add file name to path
            p[DIRSIZ] = 0;
            findfile(buf, name);
        }
        break;
    }
    close(fd);
}

int main(int agrc, char* agrv[]) {
    if (agrc != 3) {
        fprintf(2, "wrong path or filename!\n");
        exit(1);
    }
    findfile(agrv[1], agrv[2]);
    exit(0);
}