#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char **argv)
{
  //os_mount("./simdiskformat.bin");
  os_mount("./simdiskfilled.bin"); //
  os_ls("/");
  ///os_bitmap(1, true);
  printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  printf("EXISTE?: %i\n", os_exists("/music"));
  //os_open("/esto no fue mi idea.rar", 'r');
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloalo"));
  os_open("/folder/god/shrek.mp4", 'r');  //
  os_open("/folder/god/matrix.mp4", 'w'); //

  os_desmontar();
  return 0;
}
