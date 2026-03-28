#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// renametest: tests the rename() system call.
//
// Test 1 (normal case): create a file, rename it, verify the new name
//   is accessible and the old name is gone.
//
// Test 2 (overwrite case): rename to a filename that already exists;
//   the target should be atomically replaced.

static void
create_file(const char *path, const char *content)
{
  int fd = open(path, O_CREATE | O_WRONLY);
  if (fd < 0) {
    fprintf(2, "renametest: open %s failed\n", path);
    exit(1);
  }
  write(fd, content, strlen(content));
  close(fd);
}

static int
file_exists(const char *path)
{
  struct stat st;
  return stat(path, &st) == 0;
}

int
main(void)
{
  printf("=== rename test ===\n");

  // Clean up any leftover files from previous runs
  unlink("oldname.txt");
  unlink("newname.txt");
  unlink("target.txt");
  unlink("source.txt");

  // ----------------------------------------------------------------
  // Test 1: Normal rename – old name disappears, new name appears
  // ----------------------------------------------------------------
  printf("Test 1: normal rename (oldname -> newname)\n");

  create_file("oldname.txt", "hello");

  if (!file_exists("oldname.txt")) {
    printf("  FAIL: could not create oldname.txt\n");
    exit(1);
  }

  if (rename("oldname.txt", "newname.txt") < 0) {
    fprintf(2, "renametest: rename failed\n");
    exit(1);
  }

  if (file_exists("newname.txt") && !file_exists("oldname.txt")) {
    printf("  PASS: newname.txt exists and oldname.txt is gone\n");
  } else if (!file_exists("newname.txt")) {
    printf("  FAIL: newname.txt does not exist after rename\n");
    exit(1);
  } else {
    printf("  FAIL: oldname.txt still exists after rename\n");
    exit(1);
  }

  // Clean up
  unlink("newname.txt");

  // ----------------------------------------------------------------
  // Test 2: Rename over an existing file (overwrite / replace)
  // ----------------------------------------------------------------
  printf("Test 2: rename over existing file (source -> target)\n");

  create_file("source.txt", "source content");
  create_file("target.txt", "old target content");

  if (rename("source.txt", "target.txt") < 0) {
    fprintf(2, "renametest: rename (overwrite) failed\n");
    exit(1);
  }

  if (!file_exists("source.txt") && file_exists("target.txt")) {
    // Verify target now has the source content
    char buf[64];
    int fd = open("target.txt", O_RDONLY);
    int n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    buf[n] = '\0';
    if (strcmp(buf, "source content") == 0) {
      printf("  PASS: target.txt replaced with source content\n");
    } else {
      printf("  FAIL: target.txt content wrong after overwrite rename\n");
      exit(1);
    }
  } else if (file_exists("source.txt")) {
    printf("  FAIL: source.txt still exists after rename-over\n");
    exit(1);
  } else {
    printf("  FAIL: target.txt missing after rename-over\n");
    exit(1);
  }

  // Clean up
  unlink("target.txt");

  printf("=== rename test PASSED ===\n");
  exit(0);
}
