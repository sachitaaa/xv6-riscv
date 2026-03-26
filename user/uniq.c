#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXLINE 512

void
uniq(int fd) {
  char buf[MAXLINE];
  char prev[MAXLINE];
  char linebuf[MAXLINE];
  int n, i;
  int prevlen = 0;
  int linelen = 0;

  prev[0] = '\0';

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      if (buf[i] == '\n') {
        linebuf[linelen] = '\0';
        if (linelen != prevlen || memcmp(linebuf, prev, linelen) != 0) {
          write(1, linebuf, linelen);
          write(1, "\n", 1);
          memcpy(prev, linebuf, linelen + 1);
          prevlen = linelen;
        }
        linelen = 0;
      } else {
        if (linelen < MAXLINE - 1)
          linebuf[linelen++] = buf[i];
      }
    }
  }

  if (linelen > 0) {
    linebuf[linelen] = '\0';
    if (linelen != prevlen || memcmp(linebuf, prev, linelen) != 0) {
      write(1, linebuf, linelen);
      write(1, "\n", 1);
    }
  }
}

int
main(int argc, char *argv[]) {
  int fd;

  if (argc <= 1) {
    uniq(0);
  } else {
    fd = open(argv[1], 0);
    if (fd < 0) {
      fprintf(2, "uniq: cannot open %s\n", argv[1]);
      exit(1);
    }
    uniq(fd);
    close(fd);
  }

  exit(0);
}
