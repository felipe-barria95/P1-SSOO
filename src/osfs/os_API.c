#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "os_API.h"
#include <stdlib.h>


void os_mount(char* diskname) {
  file = fopen(diskname, "rb");
}

void os_bitmap(unsigned num, bool hex){ //FALTA imprimir en stderr//
  unsigned char *buffer;
  buffer = calloc(2048, sizeof(char));
  if (num == 0){
    int count = 0;
    for (int j = 1; j < 65; j++){
      int number = 2048*j;
      fseek(file, number, SEEK_SET);
      fread(buffer, 2048, 1, file);
      printf("Bloque %i\n", j);
      if(hex == true){
        for (int i = 0; i < 2048; i++){
          count += bits_in_char(buffer[i]);
          printf("%02X ", buffer[i]);
        }
      }
      else{
        for (int i = 0; i < 2048; i++){
          count += bits_in_char(buffer[i]);
          print_bits(buffer[i]);
          printf(" ");
        }
      }
      printf("\n");
    }
    fseek(file, 0, SEEK_SET);
    printf("Contador: Bloques usados: %i. Bloques libres: %i.\n", count, 1048576-count);
  }
  else if (num > 0 && num < 65){
    int number = 2048*num;
    fseek(file, number, SEEK_SET);
    fread(buffer, 2048, 1, file);
    if(hex == true){
      for (int i = 0; i < 2048; i++){
        printf("%02X ", buffer[i]);
      }
    }
    else{
      for (int i = 0; i < 2048; i++){
        print_bits(buffer[i]);
        printf(" ");
      }
    }
    printf("\n");
  }
  free(buffer);
}

int os_exists(char* path){
  for (int i = 0; i < 64; i++){
    unsigned char name[29];
    unsigned char index[3];
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0){
      unsigned char new_path[29];
      int last;
      last = strip_path(path, new_path); //retorna 1 si hay SOLO UNA referencia a un archivo/carpeta en el directorio path, 0 en otro caso//
      printf("New char: %s\n", path);
      printf("Char search: %s\n", new_path);
      int match = strcmp(new_path, name); //si es 0, hay match//
      printf("match: %i\n", match);
      printf("last: %i\n", last);
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
}

void os_ls(char* path){
  for (int i = 0; i < 64; i++){
    unsigned char index[3];
    unsigned char name[29];
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);

    if (is_valid(index) > 0){ //si llegamos al directorio destino o si estamos en directorio raiz//
      if (path[0] == NULL || strcmp(path, "/") == 0){
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
  printf("==================================\n");
  fseek(file, 0, SEEK_SET);
}

osFile* os_open(char* path, char mode){
  // RETORNAUN PUNTERO DEL ARCHIVO O NULL SI ES QUE HUBO UN ERROR

  // definimos la variable
  osFile* OsFile;
  OsFile = malloc(sizeof(osFile));
  OsFile->mode = mode;
  OsFile->posicion = 0;

  printf("existe? %i\n", os_exists(path));
  // Si es 'r' y el archivo existe
  if (mode == 'r' && os_exists(path)){
    OsFile->file = fopen(path, 'r');
    return OsFile;
  }

  // Si es 'w' y el archivo no existe
  else if (mode == 'w'&& (!os_exists(path))){
    OsFile->file = fopen(path, 'w');
    return OsFile;
  }

  // Si no es ninguo de los dos o el achivo existo o no exite
  else{
    printf("ERROR No se pudo habrir el archivo\n");
    free(OsFile);
    return NULL;
  }
}

int os_read(osFile* file_desc, void* buffer, int nbytes){
  // Esta funcion deviera funcionar solo si el archivo está abierto y existe el buffer
  // retorna la cantidad de byts leidos
  if (file_desc->mode == 'r')
  {
    int avance = fread(buffer, nbytes, 1, file_desc->file);
    file_desc->posicion += avance;
    return avance;
  }
  else{
    printf("ERROR, este archivo no está en modo lectura\n");
  }

}

int os_write(osFile* file_desc, void* buffer, int nbytes){

}

void os_close(osFile* file_desc){

}

void os_rm(char* path){        

}

int os_hardlink(char* orig, char* dest){

}

int os_mkdir(char* path){
  for (int i = 0; i < 64; i++){
    unsigned char index[3];
    unsigned char name[29];
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);

    if (is_valid(index) > 0){ //si llegamos al directorio destino o si estamos en directorio raiz//
      unsigned char new_path[29];
      int last;
      last = strip_path(path, new_path);
      if (last == 1){
        fseek(file, -32*(i+1), SEEK_SET); //devolvemos hacia atras todo lo que recorrio inicialmente//
        unsigned char index_2[3];
        unsigned char name_2[29];
        for (int j = 0; j < 64; j++){
          fread(index_2, 3, 1, file);
          fread(name_2, 29, 1, file);
          if (is_valid(index_2) == 0){
            fseek(file, -32, SEEK_SET);
            int asigned_block = update_bitmap();
            //obtenemos el valor de index que deberia tener y lo guardamos en index_2. Asignamos tambien el bloque de direccion y actualizamos el bitmap// FALTA
            fwrite(index_2, 3, 1, file);
            fwrite(new_path, 29, 1, file);
            fseek(file, 0, SEEK_SET);
            break;
          }
        }
        break;
      }
      else{
        unsigned char new_path[29];
        int last;
        last = strip_path(path, new_path);
        int match = strcmp(new_path, name); //si es 0, hay match//
        if (match == 0){
          path = path + strip_new_path(new_path);
          int disk_number = 2048*block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return os_mkdir(path);
        }
      }
    }
  }
}

int os_rmdir(char* path, bool recursive){

}

void os_unload(char* orig, char* dest){

}

void os_load(char* orig){

}

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
  int count = 0;
  for (int k = 0; k < 29; k++){
    if (j < 2){
      if (path[k] == slash){
        printf("path actual %i: %c\n",path[k], k);
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
  for (int k = 0; k < 29; k++){
    if (new_path[k] == slash){
      count++;
    }
  }
  printf("Count: %i\n", j);
  if (count > 0){
    return 0;
  }
  else if (count == 0){
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

void print_bits(unsigned char val)
{
  for (int i = 7; i >= 0; i--){
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}

int bits_in_char(unsigned char val)
{
  int count = 0;
  for (int i = 7; i >= 0; --i){
    int bit = ((val & (1 << i)) >> i);
    count += bit;
  }
  return count;
}

int update_bitmap(){}
