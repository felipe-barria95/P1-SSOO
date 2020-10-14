#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char **argv){
  os_mount("/Users/felipe/Documents/GitHub/P1-SSOO/disk_files/simdiskfilled.bin");
  printf("%i\n", os_exists("/folder"));
  os_ls("/");
  os_bitmap(0, true);
  return 0;
}
