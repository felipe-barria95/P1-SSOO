#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv) {
  os_mount("./disk_files/simdiskfilled.bin");
  //os_ls("/folder/god/");
  ///os_bitmap(1, true);
  //printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  unsigned char* hola;
  os_write(os_open("/folder/god/shrek.mp4", 'w'), hola, 2048);
  //printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloalo"));
  //os_open("/IMPORTANT.txt", 'r');//
  return 0;
}
