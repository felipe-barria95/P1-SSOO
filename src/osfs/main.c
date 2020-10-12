#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

FILE file*;

int main(int argc, char **argv){
  os_mount("/Users/felipe/Desktop/simdiskformat.bin");
  os_exist("hola");

  return 0;
}
