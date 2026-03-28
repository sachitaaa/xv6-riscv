#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// meminfotest: tests the meminfo() system call.
//
// Normal case: call meminfo() before and after allocating a large
//   buffer with sbrk().  The second reading should be smaller,
//   confirming that freemem() tracks physical page consumption.

#define ALLOC_BYTES (64 * 4096)   // 64 pages = 256 KB

int
main(void)
{
  printf("=== meminfo test ===\n");

  // ----------------------------------------------------------------
  // Test 1: meminfo() decreases after sbrk() allocation
  // ----------------------------------------------------------------
  printf("Test 1: free memory decreases after sbrk() allocation\n");

  uint64 before = meminfo();
  printf("  Free memory before sbrk: %lu bytes\n", before);

  // Allocate ALLOC_BYTES of heap space
  char *buf = sbrk(ALLOC_BYTES);
  if (buf == SBRK_ERROR) {
    fprintf(2, "meminfotest: sbrk failed\n");
    exit(1);
  }

  // Touch every page so the kernel actually maps them
  for (int i = 0; i < ALLOC_BYTES; i += 4096)
    buf[i] = 1;

  uint64 after = meminfo();
  printf("  Free memory after  sbrk: %lu bytes\n", after);
  printf("  Difference:              %lu bytes\n", before - after);

  if (after < before) {
    printf("  PASS: meminfo() correctly reflects page consumption\n");
  } else {
    printf("  FAIL: free memory did not decrease after allocation\n");
    exit(1);
  }

  // ----------------------------------------------------------------
  // Test 2: meminfo() returns a positive (non-zero) value
  // ----------------------------------------------------------------
  printf("Test 2: meminfo() returns a positive value\n");
  uint64 fmem = meminfo();
  if (fmem > 0) {
    printf("  PASS: meminfo() = %lu (> 0)\n", fmem);
  } else {
    printf("  FAIL: meminfo() returned 0 – out of memory or bug\n");
    exit(1);
  }

  printf("=== meminfo test PASSED ===\n");
  exit(0);
}
