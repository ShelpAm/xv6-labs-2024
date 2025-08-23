#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

  int c2p[2];
  int p2c[2];
  pipe(c2p);
  pipe(p2c);

  int pid = fork();

  char const byte = 'b';

  if (pid == 0) {
    char buf;
    read(p2c[0], &buf, 1);
    printf("%d: received ping\n", getpid());
    write(c2p[1], &byte, 1);
  } else {
    write(p2c[1], &byte, 1);
    char buf;
    read(c2p[0], &buf, 1);
    printf("%d: received pong\n", getpid());
  }

  exit(0);
}
