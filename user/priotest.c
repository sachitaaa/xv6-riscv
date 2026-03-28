#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// priotest: tests the priority scheduler.
//
// Forks three child processes and assigns them priorities 2, 10, and 28.
// Each child performs a CPU-bound loop and prints a completion message.
// The expected order of completion is P2 first, then P10, then P28.

int
main(void)
{
  int pid1, pid2, pid3;

  printf("Priority Test: forking 3 processes with priorities 2, 10, 28\n");
  printf("Expected completion order: P2 first, then P10, then P28.\n");

  pid1 = fork();
  if(pid1 == 0) {
    setpriority(getpid(), 2);
    char *msg = "Child 1 (P2) started\n";
    write(1, msg, strlen(msg));
    for(volatile int i = 0; i < 100000000; i++); 
    msg = "Child 1 (P2) finished\n";
    write(1, msg, strlen(msg));
    exit(0);
  }

  pid2 = fork();
  if(pid2 == 0) {
    setpriority(getpid(), 10);
    char *msg = "Child 2 (P10) started\n";
    write(1, msg, strlen(msg));
    for(volatile int i = 0; i < 100000000; i++);
    msg = "Child 2 (P10) finished\n";
    write(1, msg, strlen(msg));
    exit(0);
  }

  pid3 = fork();
  if(pid3 == 0) {
    setpriority(getpid(), 28);
    char *msg = "Child 3 (P28) started\n";
    write(1, msg, strlen(msg));
    for(volatile int i = 0; i < 100000000; i++);
    msg = "Child 3 (P28) finished\n";
    write(1, msg, strlen(msg));
    exit(0);
  }

  // Parent waits for all children
  wait(0);
  wait(0);
  wait(0);
  printf("All children done.\n");
  exit(0);
}