#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
  os_mount(argv[1]);
  os_ls("/");
  ///os_bitmap(1, true);
  //printf("EXISTE?: %i\n", os_exists("/folder/napkin.txt"));
  unsigned char hola[56000];

  printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  //unsigned char hola[4];
  //hola[0] = "c";
  //os_write(os_open("/folder/nap", 'w'), hola, 128);
  //os_close(os_open("/folder/barry.txt", 'w'));
  //printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloalo"));
  //printf("%i\n", os_read(os_open("/IMPORTANT.txt", 'r'), hola, 56000));//
  //printf("%s\n", hola);
  //os_mount("./simdiskfilled.bin");//
  //os_ls("/");
  //os_bitmap(1, true);
  //printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  //printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  //os_bitmap(1, true);
  //printf("Mkdir: %i\n", os_mkdir("/folder/alohuhu"));
  //os_bitmap(1, true);
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloh432uhu"));
  os_ls("/memes/generic (3).png");
  os_ls("/memes");
  os_bitmap(1, true);
  //printf("EXISTE?: %i\n", os_exists("/esto no fue mi idea.rar"));
  //printf("EXISTE?: %i\n", os_exists("/music/UN Owen Was Her.mp3"));
  //os_bitmap(1, true);
  os_ls("/folder");
  os_ls("/folder");
  //os_bitmap(1, true);
  printf("Mkdir: %i\n", os_mkdir("/memes/g bfgdsf777"));
  os_ls("/memes");
  os_ls("/folder");
  os_rm("/folder/napkin.txt");
  os_ls("/folder");
  os_rmdir("/folder/61fdshjnfke", false);
  os_ls("/folder");
  os_rmdir("/folder", true);
  os_ls("/folder");
  os_ls("/");
  printf("Mkdir: %i\n", os_mkdir("/memes/g bfgdsfgdsf777"));
  os_bitmap(1, true);
  //os_bitmap(1, true);
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloh4532uhu"));
  //os_bitmap(1, true);
  //os_ls("/folder");
  //printf("rmdir: %i\n", os_rmdir("/folder", true));
  //os_bitmap(1, true);
  //os_ls("/");
  //os_ls("/folder");
  //printf("Mkdir: %i\n", os_mkdir("/folder"));
  //os_bitmap(1, true);
  //printf("Mkdir: %i\n", os_mkdir("/folder/alohuhu"));
  //os_bitmap(1, true);
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloh432uhu"));
  //os_bitmap(1, true);
  //printf("Mkdir: %i\n", os_mkdir("/folder/aloh4532uhu"));
  //os_ls("/folder");
  //os_ls("/");
  //os_ls("/memes");
  //os_rm("/memes/generic (1).jpg");
  //os_ls("/memes");
  //os_rm("/memes/generic (3).png");
  //os_ls("/");
  //os_ls("/memes");
  os_unmount();
  return 0;
}
