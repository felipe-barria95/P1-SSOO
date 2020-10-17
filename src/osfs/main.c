#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char **argv){
  os_mount("/Users/felipe/Documents/GitHub/P1-SSOO/disk_files/simdiskfilled.bin");
  //os_mount("./simdiskfilled.bin");//
  os_ls("/");
  ///os_bitmap(1, true);
  printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  printf("Mkdir: %i\n", os_mkdir("/folder/aloalo"));
  //os_open("/IMPORTANT.txt", 'r');//
  return 0;
}
