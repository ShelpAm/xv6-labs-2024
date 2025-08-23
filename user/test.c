#include "kernel/types.h"
#include "user/user.h"

int main() {
  int fd[2];
  pipe(fd);

  char s[] = "haha";
  write(fd[1], s, sizeof(s));
  close(fd[1]);

  char buf[10];
  int sz = read(fd[0], buf, sizeof(buf));
  printf("return value after close (with unread data): %d\n", sz);
  sz = read(fd[0], buf, sizeof(buf));
  printf("secondj return value after close (with unread data): %d\n", sz);

  return 0;
}
