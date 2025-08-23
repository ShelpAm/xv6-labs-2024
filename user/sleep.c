#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(2,
            "You must provide at least one integer argument to specify sleep "
            "ticks.\n");
    exit(1);
  }

  sleep(atoi(argv[1]));
  exit(0);
}
