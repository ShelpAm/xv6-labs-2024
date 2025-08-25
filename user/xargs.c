#include "kernel/types.h"

#include "kernel/param.h"
#include "user/user.h"

#define MAX_STRLEN 256

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "usage: xargs <cmd> ...\n");
        exit(1);
    }

    int children = 0;

    // Shifts all element left.
    for (int i = 1; i != argc; ++i) {
        argv[i - 1] = argv[i];
    }
    // Keeps last pointing to arg.
    argv[argc - 1] = malloc(MAX_STRLEN);
    char *p = argv[argc - 1];

    char buf[MAX_STRLEN];
    int size;
    do {
        size = read(0, buf, sizeof buf);
        for (int i = 0; i != size; ++i) {
            if (buf[i] == '\n' || size == 0) {
                *p++ = '\0';
                ++children;
                if (fork() == 0) {
                    exec(argv[0], argv);
                    fprintf(2, "xargs: cannot run %s\n", argv[0]);
                    exit(1);
                }
                p = argv[argc - 1];
            }
            else {
                // printf("%d\n", buf[i]);
                *p++ = buf[i];
            }
        }
    } while (size != 0);

    for (int i = 0; i != children; ++i) {
        wait(0);
    }

    exit(0);
}
