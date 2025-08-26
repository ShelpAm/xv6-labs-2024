#include "kernel/types.h"

#include "kernel/fcntl.h"
#include "kernel/riscv.h"
#include "user/user.h"

void putc(int fd, char c)
{
    char s[2] = {};
    s[0] = c;
    printf("%s", s);
}

int main(int argc, char *argv[])
{
    // your code here.  you should write the secret to fd 2 using write
    // (e.g., write(2, secret, 8)

    char const *prefix = "is: ";
    int len = strlen(prefix);

    int sz = PGSIZE * 32 * 8;

    char *end = sbrk(sz);
    // end = end + 9 * PGSIZE;

    for (char *p = end;; ++p) {
        // putc(1, *p);
        if (memcmp(prefix, p, len) == 0) {
            char buf[41] = {};
            memcpy(buf, p, 40);
            write(2, p + len, 8);
            break;
        }
    }

    exit(1);
}
