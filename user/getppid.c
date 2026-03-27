#include "kernel/types.h"
#include "user/user.h"

int main() {
  printf("My PID: %d, Parent PID: %d\n", getpid(), getppid());
  exit(0);
}