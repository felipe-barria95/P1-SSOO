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
  for (int i = 0; i < 64; i++){
    unsigned char name[29];
    unsigned char index[3];
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0){
      unsigned char new_path[29];
      int last;
      last = strip_path(path, new_path); //1 si hay SOLO UNA referencia a un archivo/carpeta en el directorio path, 0 en otro caso//
      int match = strcmp(new_path, name); //si es 0, hay match//
      if (match == 0){
        if (last == 1){
          fseek(file, 0, SEEK_SET);
          return 1;
        }
        else{
          path = path + strip_new_path(new_path); //obtengo nuevo path, eliminando la carpeta que ya accedi//
          int disk_number = 2048*block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return os_exists(path);
        }
      }
      if (strip_new_path(new_path) == 1){ //si se llama al path "/"//
        fseek(file, 0, SEEK_SET);
        return 1;
      }
    }
  }
  fseek(file, 0, SEEK_SET);
  return 0;
};

void os_ls(char* path){
  for (int i = 0; i < 64; i++){
    unsigned char name[29];
    unsigned char index[3];
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0){
      if (path[0] == NULL){
        printf("%s\n", name); //falta comparar directorio inicial//
      }
      else {
        unsigned char new_path[29];
        int last;
        last = strip_path(path, new_path);
        int match = strcmp(new_path, name); //si es 0, hay match//
        if (match == 0){
          path = path + strip_new_path(new_path);
          int disk_number = 2048*block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return os_ls(path);
        }
      }
    }
  }
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

void print_ls(){
  for (int i = 0; i < 64; i++){
    unsigned char name[29];
    unsigned char index[3];
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0){
      printf("%s\n", name);
    }
  }
  fseek(file, 0, SEEK_SET);
}

int is_valid (unsigned char* bits){
  return bits[0] >> 6;
}

int block_number(unsigned char *bits){
  return ((bits[0] & 0x3F) << 16) | (bits[1] << 8) | bits[2];
}

int strip_path(char* path, unsigned char new_path[29]){
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
    else{
      new_path[h] = NULL;
      h++;
    }
  }
  if (j == 2){
    return 0;
  }
  else if (j == 1){
    return 1;
  }
}

int strip_new_path(unsigned char new_path[29]){
  int i = 0;
  while (new_path[i] != NULL){
    i++;
  }
  return i+1;
}
