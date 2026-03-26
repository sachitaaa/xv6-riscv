#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: rename old new\n");
    exit(1);
  }
  if(rename(argv[1], argv[2]) < 0){
    fprintf(2, "rename: failed\n");
    exit(1);
  }
  exit(0);
}