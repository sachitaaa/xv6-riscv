#include "kernel/types.h"
#include "user/user.h"

int 
main(void)
{
  printf("free memory: %lu bytes\n", meminfo());
  exit(0);
}
