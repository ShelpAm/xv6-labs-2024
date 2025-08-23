// TODO: this program can only run until 283 under xv6, but won't exit on Linux
// (tested).

#include "kernel/types.h"
#include "user/user.h"

void subprocess(int left[2]) __attribute__((noreturn));

#define N 300

void wait_for_pipe(int *fd) {
  while (pipe(fd) == -1) {
    fprintf(2, "error(pid=%d): cannot create pipe\n", getpid());
    sleep(10);
  }
}

// Before subprocess, read end and write end are both open.
// subprocess should close read end from left and write end to the right
// (while keeping read end of right open).
void subprocess(int left[2]) {
  close(left[1]);
  int p;
  read(left[0], &p, sizeof(p));
  printf("prime %d\n", p);

  int has_subprocess = 0;
  int right[2];
  int n;
  while (read(left[0], &n, sizeof(n))) {
    if (n % p != 0) {
      if (!has_subprocess) {
        wait_for_pipe(right);
        int pid = fork();
        if (pid == 0) {
          close(left[0]);
          subprocess(right);
        } else if (pid < 0) {
          fprintf(2, "failed to fork\n");
        }
        close(right[0]);
        has_subprocess = 1;
      }
      write(right[1], &n, sizeof(n));
    }
  }
  close(left[0]);
  if (has_subprocess) {
    close(right[1]);
    wait(0);
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  int fd[2];
  wait_for_pipe(fd);
  int pid = fork();
  if (pid == 0) {
    subprocess(fd);
  }
  close(fd[0]);
  for (int i = 2; i != N + 1; ++i) {
    // printf("writing %d\n", i);
    write(fd[1], &i, sizeof(i));
  }
  close(fd[1]);
  wait(0);

  exit(0);
}
