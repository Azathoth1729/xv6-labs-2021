//
// Created by azathoth on 22-6-9.
//
#include "kernel/param.h"
#include "kernel/types.h"

#include "user/user.h"

#define READ 0
#define WRITE 1
#define stdin 0
#define stdout 1
#define stderr 2

int main(int argc, char* argv[]) {
  if (argc == 1) {
    fprintf(stderr, "Usage: %s [command [initial-arguments]]\n", argv[0]);
    exit(1);
  }

  if (argc - 1 >= MAXARG) {
    fprintf(stderr, "xargs : maxium amount of args is %d !\n", MAXARG);
    exit(1);
  }

  char buf[1024], *args[MAXARG];
  int num_args;
  int n;
  for (int i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }

  num_args = argc - 1;
  char* p = buf;
  while ((n = read(stdin, p, 1)) > 0) {
    if (*p == '\n') {
      *p = 0;
      if (fork() == 0) {
        args[num_args] = buf;
        exec(args[0], args);
        exit(0);
      } else {
        wait(0);
      }
      p = buf;
    } else {
      ++p;
    }
  }
  exit(0);
}