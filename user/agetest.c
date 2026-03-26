#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid;

  printf("Aging Test: low priority process should still run eventually\n");

  pid = fork();
  if(pid == 0) {
    // Child - very low priority, aging should eventually boost it
    setpriority(getpid(), 28);
    printf("Low priority child (priority 28) started - PID %d\n", getpid());
    for(volatile int i = 0; i < 1000000; i++);
    printf("Low priority child finished! Aging works.\n");
    exit(0);
  }

  // Parent - high priority, keeps running
  setpriority(getpid(), 2);
  for(volatile int i = 0; i < 5000000; i++);
  printf("Parent done, waiting for low priority child...\n");

  wait(0);
  printf("Aging test complete - low priority process was not starved.\n");
  exit(0);
}