#include <stdbool.h>
#include <stdio.h>
#include "os_API.h"


void os_mount(char* diskname) {
  file = fopen(diskname, "rb");
};

void os_bitmap(unsigned num, bool hex){

};

void os_exists(char* path){
  for (int i = 0; i < 64; i++){
    unsigned char name[32];
    fread(name, 32, 1, file);
    fseek(file, 32, SEEK_CUR);
    for (int j = 0; j < 32; j++){
      printf("%c", name[j]);
    }
    printf("\n");
  }
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
