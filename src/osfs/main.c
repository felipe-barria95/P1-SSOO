#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
  os_mount("./disk_files/simdiskfilled.bin");
  //os_ls("/folder/god/");
  ///os_bitmap(1, true);
  printf("EXISTE?: %i\n", os_exists("/folder/napkin.txt"));
  unsigned char hola[4];
  hola[0] = 'c';
  hola[1] = 'h';
  hola[2] = 'a';
  hola[3] = 'o';
  os_write(os_open("/folder/napkin.txt", 'r'), hola, 4);
  //printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloalo"));
  //os_open("/IMPORTANT.txt", 'r');//
  return 0;
}
