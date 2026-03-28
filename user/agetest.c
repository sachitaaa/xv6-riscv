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
    char *msg = "Low priority child (P28) started\n";
    write(1, msg, strlen(msg));
    
    // Perform a lot of work
    for(volatile int i = 0; i < 50000000; i++);
    
    msg = "Low priority child finished! Aging works.\n";
    write(1, msg, strlen(msg));
    exit(0);
  }

  // Parent - high priority, keeps running or yielding
  setpriority(getpid(), 2);
  printf("Parent (P2) started, will now do work...\n");
  for(volatile int i = 0; i < 100000000; i++);
  printf("Parent done, waiting for low priority child...\n");

  wait(0);
  printf("Aging test complete.\n");
  exit(0);
}