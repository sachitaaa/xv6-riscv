#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// #include <stdlib.h>

# define MAXLINE 512

void
head(int fd, int n)
{
  char buf[MAXLINE];
  int i, count = 0;
  char c;

  i = 0;
  while(count < n && read(fd, &c, 1) > 0) {
    if(c == '\n') {
      buf[i] = '\n';
      buf[i+1] = '\0';
      write(1, buf, i+1);
      i = 0;
      count ++;
    } else {
      buf[i++] = c;
      if(i >= MAXLINE - 1) {
        //line too long flush
        write (1, buf, i);
        i = 0;
      }
    }
  }
  //print last line if no newline at end
  if(i > 0) {
    write(1, buf, i);
  }
}

int
main(int argc, char *argv[])
{
  int fd, n = 10; //default 10 lines 
  int i = 1;

  //check for -N argument (for using as head -5 <filename>)
  if(argc > 1 && argv[1][0] == '-'){
    n = atoi(&argv[1][1]) ;
    i = 2;
  }

  if(i >= argc) {
    // no file then read from stdin
    head(0, n);
  } else {
    for (; i < argc; i++) {
      if((fd = open(argv[i], 0)) < 0) {
        fprintf(2, "head: cannot open %s: No such file or directory \n", argv[i]);
        exit(1);
      }
      head(fd, n);
      close(fd);
    }
  }
  exit(0);
}
