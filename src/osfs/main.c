#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

FILE *file;

int main(int argc, char **argv){
  os_mount("/Users/felipe/Documents/GitHub/P1-SSOO/disk_files/simdiskfilled.bin");
  os_exists("hola");
  return 0;
}
