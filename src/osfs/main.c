#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char **argv){
  os_mount("/Users/felipe/Documents/GitHub/P1-SSOO/disk_files/simdiskfilled.bin");
  //os_mount("./simdiskfilled.bin");//
  os_ls("/");
  ///os_bitmap(1, true);
  printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  printf("EXISTE?: %i\n", os_exists("/folder/god/shrek.mp4ffff"));
  //os_open("/IMPORTANT.txt", 'r');//
  return 0;
}
