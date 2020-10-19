#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char **argv)
{
  os_mount("./simdiskformat.bin");
  os_ls("/");
  os_bitmap(1, true);
  os_open("/notfile.txt", 'w');
  os_bitmap(1, true);
  //printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  //unsigned char hola[4];
  //hola[0] = "c";
  //os_write(os_open("/folder/nap", 'w'), hola, 128);
  //os_close(os_open("/folder/barry.txt", 'w'));
  //printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloalo"));
  //os_open("/IMPORTANT.txt", 'r');//
  os_desmontar();

  return 0;
}
