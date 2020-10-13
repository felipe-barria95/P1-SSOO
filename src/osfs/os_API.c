#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "os_API.h"


void os_mount(char* diskname) {
  file = fopen(diskname, "rb");
};

void os_bitmap(unsigned num, bool hex){

};

int os_exists(char* path){
  unsigned char new_path[29];
  int last; //1 si solo se referencia a un archivo/carpeta en el directorio path, 0 en otro caso//
  last = strip_path(path, new_path);
  printf("New Path: %s\n", new_path);
  printf("Is last?: %i\n", last);
  for (int i = 0; i < 64; i++){
    unsigned char index[3];
    fread(index, 3, 1, file);
    if (is_valid(index) > 0){
      unsigned char name[29];
      fread(name, 29, 1, file);
      printf("Byte: %X\n", index[0]);
      printf("Number: %d\n", block_number(index));
      printf("Valid: %d\n", is_valid(index));
      printf("Name: %s\n", name);
      int match = strcmp(new_path, name);
      printf("Hay match?: %i\n", match);
      printf("\n");
      if (match == 0){
        if (last == 1){
          return 1;
        }
        else{
          strip_new_path(path);

          //fseek();//
          //os_exists(path);//
        }
      }
    }
  }
  return 0;
};

void os_ls(char* path){

};

osFile* os_open(char* path, char mode){

};

int os_read(osFile* file_desc, void* buffer, int nbytes){

};

int os_write(osFile* file_desc, void* buffer, int nbytes){

};

void os_close(osFile* file_desc){

};

void os_rm(char* path){

};

int os_hardlink(char* orig, char* dest){

};

int os_mkdir(char* path){

};

int os_rmdir(char* path, bool recursive){

};

void os_unload(char* orig, char* dest){

};

void os_load(char* orig){

};

int is_valid (unsigned char* bits){
  return bits[0] >> 6;
}

int block_number(unsigned char *bits)
{
  return (bits[0] & 0x3F << 16) | (bits[1] << 8) | bits[2];
}

int strip_path(char* path, unsigned char new_path[]){
  const char slash = '/';
  int h = 0;
  int j = 0;
  for (int k = 0; k < 29; k++){
    if (j < 2){
      if (path[k] == slash){
        j++;
      }
      else{
        new_path[h] = path[k];
        h++;
      }
    }
  }
  if (j == 2){
    return 0;
  }
  else if (j == 1){
    return 1;
  }
}

void strip_new_path(char* path){
  
}
