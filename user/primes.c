//
// Created by azathoth on 2022/3/27.
//

#include "kernel/types.h"
#include "user/user.h"

#define READ 0
#define WRITE 1
#define stdin 0
#define stdout 1
#define stderr 2

void sieveProc(int left[2]) {
  close(left[WRITE]);

  int prime, temp, pid, right[2];

  if (read(left[READ], &prime, sizeof(int)) == 0) {
    close(left[READ]);
    exit(0);
  }

  printf("prime %d\n", prime);

  pipe(right);
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "fork error...\n");
    close(right[READ]);
    close(right[WRITE]);
    close(left[READ]);
    exit(1);
  } else if (pid > 0) {
    close(right[READ]);
    while (read(left[READ], &temp, sizeof(int))) {
      if (temp % prime == 0)
        continue;
      write(right[WRITE], &temp, sizeof(int));
    }
    close(right[WRITE]);
    wait(0);
    exit(0);
  } else {
    sieveProc(right);
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 1) {
    fprintf(stderr, "Usage: primes\n");
    exit(1);
  }

  int p[2];
  pipe(p);

  int pid = fork();
  if (pid < 0) {
    fprintf(stderr, "fork error...\n");
    exit(1);
  } else if (pid > 0) {
    close(p[READ]);

    for (int i = 2; i <= 35; i++) {
      write(p[WRITE], &i, sizeof(int));
    }

    close(p[WRITE]);
    wait(0);
    exit(0);
  } else {
    sieveProc(p);
    exit(0);
  }
}