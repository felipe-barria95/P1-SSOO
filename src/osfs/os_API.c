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
    printf("I: %i\n", i);
    unsigned char bytes[32];
    fread(bytes, 32, 1, file);
    if (is_valid(bytes) > 0){
      printf("Byte: %X\n", bytes[0]);
      printf("Number: %d\n", block_number(bytes));
      printf("Valid: %d\n", is_valid(bytes));
      printf("Name: ");
      for (int j = 3; j < 32; j++){
        printf("%c", bytes[j]);
      }
      printf("\n");
    }
    fseek(file, 32, SEEK_CUR);
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

int is_valid (unsigned char* bits){
  return bits[0] >> 6;
}

int block_number(unsigned char *bits)
{
  return (bits[0] & 0x3F << 16) | (bits[1] << 8) | bits[2];
}
