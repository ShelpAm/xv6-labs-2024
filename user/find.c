#include "kernel/types.h"

#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void rm_trailing_slash(char *buf)
{
    int len = strlen(buf);
    if (buf[len - 1] == '/') {
        buf[len - 1] = '\0';
    }
}

void add_trailing_slash(char *buf)
{
    int len = strlen(buf);
    if (buf[len - 1] != '/') {
        buf[len] = '/';
        buf[len + 1] = '\0';
    }
}

int ends_with(char const *s, char const *t)
{
    char const *p = s + strlen(s);
    char const *q = t + strlen(t);
    while (p != s && q != t) {
        if (*--p != *--q) {
            return 0;
        }
    }
    return q == t;
}

void find(char *path, char *target)
{
    // printf("find: entering %s\n", path);
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(2, "find: cannot open %s", path);
        return;
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
        close(fd);
        fprintf(2, "find: cannot stat %s", path);
        return;
    }

    switch (st.type) {
    case T_DEVICE:
    case T_FILE:
        rm_trailing_slash(path);
        if (ends_with(path, target)) {
            printf("%s\n", path);
        }
        break;

    case T_DIR: {
        add_trailing_slash(path);
        uint len = strlen(path);
        struct dirent de;
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.name[0] == '.') {
                continue;
            }
            if (de.inum == 0) {
                continue;
            }
            char *p = path + len;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(path, target);
        }
        break;
    }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(2, "usage: find <path> <target>\n");
        exit(1);
    }

    ++argv;
    char *path = *argv++;
    char *target = *argv++;

    char pathbuf[256];
    strcpy(pathbuf, path);
    pathbuf[strlen(pathbuf)] = '\0';

    find(pathbuf, target);

    exit(0);
}
