//
// Created by azathoth on 2022/3/27.
//

#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

#define READ 0
#define WRITE 1
#define stdin 0
#define stdout 1
#define stderr 2

int main(int argc, char *argv[]) {
  if (argc != 1) {
    fprintf(stderr, "Usage: pingpong\n");
    exit(1);
  }

  int p[2];
  int pid;

  pipe(p);
  if (fork() == 0) // child (recv -> send)
  {
    pid = getpid();
    char buf[2];

    if (read(p[READ], buf, 1) != 1) {
      fprintf(stderr, "failed to read in child\n");
      exit(1);
    }

    close(p[READ]);
    printf("%d: received ping\n", pid);

    if (write(p[WRITE], buf, 1) != 1) {
      fprintf(stderr, "failed to write in child\n");
      exit(1);
    }
    close(p[WRITE]);
    exit(0);

  } else // parent (send -> recv)
  {
    pid = getpid();
    char info[2] = "A";
    char buf[2];
    buf[1] = 0;
    if (write(p[WRITE], info, 1) != 1) {
      fprintf(stderr, "failed to write in parent\n");
      exit(1);
    }

    close(p[WRITE]);
    wait(0);

    if (read(p[READ], buf, 1) != 1) {
      fprintf(stderr, "failed to read in parent\n");
      exit(1);
    }
    printf("%d: received pong\n", pid);
    close(p[READ]);
    exit(0);
  }
}