#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
  os_mount(argv[1]);
  //os_ls("/folder/god/");
  ///os_bitmap(1, true);
  printf("EXISTE?: %i\n", os_exists("/folder/napkin.txt"));
  unsigned char hola[4];
  hola[0] = "c";
  //os_write(os_open("/folder/nap", 'w'), hola, 128);
  //os_close(os_open("/folder/barry.txt", 'w'));
  //printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloalo"));
  //os_open("/IMPORTANT.txt", 'r');//
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
  printf("Mkdir: %i\n", os_mkdir("/folder"));
  os_bitmap(1, true);
  printf("Mkdir: %i\n", os_mkdir("/folder/alohuhu"));
  os_bitmap(1, true);
  printf("Mkdir: %i\n", os_mkdir("/folder/aloh432uhu"));
  os_bitmap(1, true);
  printf("Mkdir: %i\n", os_mkdir("/folder/aloh4532uhu"));
  os_ls("/folder");
  os_ls("/");
  os_ls("/memes");
  os_rm("/memes/generic (1).jpg");
  os_ls("/memes");
  os_rm("/memes/generic (3).png");
  os_ls("/");
  os_ls("/memes");
  os_desmontar();
  return 0;
}
