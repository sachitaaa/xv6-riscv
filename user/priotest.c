#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid1, pid2, pid3;

  printf("Priority Test: forking 3 processes with priorities 2, 10, 28\n");

  pid1 = fork();
  if(pid1 == 0) {
    // Child 1 - highest priority
    setpriority(getpid(), 2);
    printf("Child 1 (priority 2)  is running - PID %d\n", getpid());
    for(volatile int i = 0; i < 1000000; i++);  // busy work
    printf("Child 1 (priority 2)  finished\n");
    exit(0);
  }

  pid2 = fork();
  if(pid2 == 0) {
    // Child 2 - medium priority
    setpriority(getpid(), 10);
    printf("Child 2 (priority 10) is running - PID %d\n", getpid());
    for(volatile int i = 0; i < 1000000; i++);
    printf("Child 2 (priority 10) finished\n");
    exit(0);
  }

  pid3 = fork();
  if(pid3 == 0) {
    // Child 3 - lowest priority
    setpriority(getpid(), 28);
    printf("Child 3 (priority 28) is running - PID %d\n", getpid());
    for(volatile int i = 0; i < 1000000; i++);
    printf("Child 3 (priority 28) finished\n");
    exit(0);
  }

  // Parent waits for all children
  wait(0);
  wait(0);
  wait(0);
  printf("All children done. Child 1 should have finished first.\n");
  exit(0);
}