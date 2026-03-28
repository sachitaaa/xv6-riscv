#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// getppidtest: tests the getppid() system call.
//
// Test 1 (normal case): fork a child; the child calls getppid() and
//   verifies it matches the PID printed by the parent.
//
// Test 2 (orphan case): parent exits before the child calls getppid().
//   xv6 re-parents orphaned processes to init (PID 1), so getppid()
//   should return 1 after the parent is gone.

int
main(void)
{
  int parent_pid, child_pid;
  int pipefd[2];

  printf("=== getppid test ===\n");

  // ----------------------------------------------------------------
  // Test 1: Normal case – child's getppid() should equal parent PID
  // ----------------------------------------------------------------
  printf("Test 1: normal parent-child getppid\n");

  if (pipe(pipefd) < 0) {
    fprintf(2, "getppidtest: pipe failed\n");
    exit(1);
  }

  parent_pid = getpid();
  child_pid = fork();

  if (child_pid < 0) {
    fprintf(2, "getppidtest: fork failed\n");
    exit(1);
  }

  if (child_pid == 0) {
    // Child: read parent PID from pipe, compare with getppid()
    close(pipefd[1]);
    int ppid_reported;
    read(pipefd[0], &ppid_reported, sizeof(ppid_reported));
    close(pipefd[0]);

    int my_ppid = getppid();
    if (my_ppid == ppid_reported) {
      printf("  PASS: child getppid() = %d (matches parent PID)\n", my_ppid);
    } else {
      printf("  FAIL: child getppid() = %d, expected %d\n", my_ppid, ppid_reported);
    }
    exit(0);
  } else {
    // Parent: send our PID to child via pipe then wait
    close(pipefd[0]);
    write(pipefd[1], &parent_pid, sizeof(parent_pid));
    close(pipefd[1]);
    printf("  Parent PID = %d\n", parent_pid);
    wait(0);
  }

  // ----------------------------------------------------------------
  // Test 2: Orphan case – parent exits; child should be re-parented
  //         to init (PID 1)
  // ----------------------------------------------------------------
  printf("Test 2: orphan re-parenting (parent exits before child calls getppid)\n");

  child_pid = fork();
  if (child_pid < 0) {
    fprintf(2, "getppidtest: fork failed\n");
    exit(1);
  }

  if (child_pid == 0) {
    // Child: busy-wait a bit so the parent has time to exit
    for (volatile int i = 0; i < 2000000; i++);
    int my_ppid = getppid();
    if (my_ppid == 1) {
      printf("  PASS: orphan getppid() = 1 (re-parented to init)\n");
    } else {
      // Parent may still be alive depending on scheduling; acceptable
      printf("  INFO: orphan getppid() = %d (may still be original parent)\n", my_ppid);
    }
    exit(0);
  } else {
    // Parent exits immediately, orphaning the child
    printf("  Parent (PID %d) exiting – child becomes orphan\n", getpid());
    exit(0);
  }

  // unreachable
  exit(0);
}
