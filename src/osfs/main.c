#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char **argv){
  os_mount("/Users/felipe/Documents/GitHub/P1-SSOO/disk_files/simdiskfilled.bin");
  //os_mount("./simdiskfilled.bin");//
  os_ls("/");
  os_bitmap(1, true);
  printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  os_bitmap(1, true);
  printf("Mkdir: %i\n", os_mkdir("/folder/alohuhu"));
  os_bitmap(1, true);
  printf("Mkdir: %i\n", os_mkdir("/folder/aloh432uhu"));
  os_bitmap(1, true);
  printf("Mkdir: %i\n", os_mkdir("/folder/aloh4532uhu"));
  os_bitmap(1, true);
  os_ls("/folder");
  printf("rmdir: %i\n", os_rmdir("/folder", true));
  os_bitmap(1, true);
  os_ls("/");
  os_ls("/folder");
  fclose(file);
  return 0;
}
