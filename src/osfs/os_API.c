#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "os_API.h"
#include <stdlib.h>

void os_mount(char* diskname) {
  file = fopen(diskname, "rw");
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
    fseek(file, 0, SEEK_SET);
  }
  free(buffer);
}

int os_exists(char* path){
  const char slash = '/';
  unsigned char name[29];
  unsigned char index[3];
  unsigned char new_path[29];
  path = path + 1;
  if (path[0] == NULL){
    return 1;
  }
  for (int i = 0; i < 64; i++){
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0){
      if (strchr(path, slash) != NULL){
        strip_path(path, new_path, 1);
        int match = strcmp(new_path, name);
        if (match == 0){
          path = path + strip_new_path(new_path); //obtengo nuevo path, eliminando la carpeta que ya accedi//
          int disk_number = 2048*block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return os_exists(path);
        }
      }
      else if (strchr(path, slash) == NULL){
        int match = strcmp(path, name);
        if (match == 0){
          fseek(file, 0, SEEK_SET);
          return 1;
        }
      }
    }
  }
  fseek(file, 0, SEEK_SET);
  return 0;
}

void os_ls(char* path){
  const char slash = '/';
  unsigned char index[3];
  unsigned char name[29];
  for (int i = 0; i < 64; i++){
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0){ //si llegamos al directorio destino o si estamos en directorio raiz//
      if (path[0] == NULL || path == "/"){
        printf("%s\n", name); //falta comparar directorio inicial//
      }
      else {
        unsigned char new_path[29];
        strip_path(path, new_path, 2);
        int match = strcmp(new_path, name); //si es 0, hay match//
        //FALTA LAST//
        if (match == 0){
          path = path + strip_new_path(new_path)+1;
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

int ret_pos(char* path){
  const char slash = '/';
  int pos;
  unsigned char name[29];
  unsigned char index[3];
  unsigned char new_path[29];
  path = path + 1;
  if (path[0] == NULL){
    return 1;
  }
  for (int i = 0; i < 64; i++){
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0){
      if (strchr(path, slash) != NULL){
        strip_path(path, new_path, 1);
        int match = strcmp(new_path, name);
        if (match == 0){
          path = path + strip_new_path(new_path); //obtengo nuevo path, eliminando la carpeta que ya accedi//
          int disk_number = 2048*block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return ret_pos(path);
        }
      }
      else if (strchr(path, slash) == NULL){
        int match = strcmp(path, name);
        if (match == 0){
          fseek(file, 0, SEEK_SET);
          pos = 2048*block_number(index);
          return pos;
        }
      }
    }
  }
  fseek(file, 0, SEEK_SET);
  return 0;
}

osFile* os_open(char* path, char mode){
  // RETORNAUN PUNTERO DEL ARCHIVO O NULL SI ES QUE HUBO UN ERROR

  // definimos la variable
  osFile* OsFile;
  OsFile = malloc(sizeof(osFile));
  OsFile->mode = mode;
  OsFile->pos_direct = 0;

  printf("existe? %i\n", os_exists(path));
  // Si es 'r' y el archivo existe
  if (mode == 'r' && os_exists(path)){
    //OsFile->file = fopen(path, 'r');
    int pos;
    // El siguiente bloque es una copia de os exist para encontrar la ubicación del archivo.
    OsFile->pos_direct = ret_pos(path);// averiguamos la posición del directorio del archivo
    fseek(file, OsFile->pos_direct, SEEK_SET);// llevamos la lectura hasta el bloque de directorio
    // una vez en el
    unsigned char index[3];
    fread(index, 3, 1, file);// leemos el número de bloque de el bloque indice
    OsFile->pos_indice = 2048*block_number(index);
    fseek(file, OsFile->pos_indice, SEEK_SET);// nos movemos al bloque incice
    // una vez en el bloque indice
    unsigned char hardlinks[1];
    fread(hardlinks, 1, 1, file);// leemos el número de hardlinks
    OsFile->n_hardlinks = (int) hardlinks;
    return OsFile;
  }

  // Si es 'w' y el archivo no existe
  else if (mode == 'w'&& (!os_exists(path))){
    //OsFile->file = fopen(path, 'w');
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
    //int avance = fread(buffer, nbytes, 1, file_desc->file);
    //file_desc->pos_direct += avance;
    //return avance;
  }
  else{
    printf("ERROR, este archivo no está en modo lectura\n");
  }
}

int os_write(osFile* file_desc, void* buffer, int nbytes){

}

void os_close(osFile* file_desc){
  //Si está en modo w y el tamaño es mayor a 0
  if (file_desc->mode == 'w'){
    if (file_desc->size > 0){
      //Falta chequear si el archivo existe
      uint32_t block_size = 4; //en bytes

    }
  }

  //Si está en modo
}

void os_rm(char* path){
}

int os_hardlink(char* orig, char* dest){

}

int os_mkdir(char* path){
  const char slash = '/';
  unsigned char new_path[29];
  path = path + 1;
  strip_path(path, new_path, 1);
  printf("Path 1: %s\n", path);
  if (strchr(path, slash) != NULL){ //si no llegamos al directorio destino o si estamos en directorio raiz//
    for (int i = 0; i < 64; i++){
      unsigned char index[3];
      unsigned char name[29];
      printf("new path: %s\n", new_path);
      fread(index, 3, 1, file);
      fread(name, 29, 1, file);
      if (is_valid(index) > 0){
        printf("name: %s\n", name);
        int match = strcmp(new_path, name);
        if (match == 0){
          path = path + strip_new_path(new_path);
          int disk_number = 2048*block_number(index);
          printf("Disk numerb: %i\n", disk_number);
          fseek(file, disk_number, SEEK_SET);
          return os_mkdir(path);
        }
      }
    }
  }
  else{
    unsigned char index_2[3];
    unsigned char name_2[29];
    for (int j = 0; j < 64; j++){
      fread(index_2, 3, 1, file);
      fread(name_2, 29, 1, file);
      if (is_valid(index_2) == 0){
        printf("llego aca\n");
        unsigned long position;
        fflush(file);
        position = ftell(file) - 32;
        printf("Disk numer: %i\n", position);
        int asigned_block = update_bitmap();
        printf("Block asginated: %i\n", asigned_block);
        int_to_bytes(index_2, asigned_block);
        printf("%X %X %X\n", index_2[0], index_2[1], index_2[2]);
        //obtenemos el valor de index que deberia tener y lo guardamos en index_2. Asignamos tambien el bloque de direccion y actualizamos el bitmap// FALTA
        fseek(file, position, SEEK_SET);
        fwrite(index_2, 3, 1, file);
        printf("new new newpath: %s\n", new_path);
        fwrite(new_path, 29, 1, file);
        fseek(file, 0, SEEK_SET);
        return 1;
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

void strip_path(char* path, unsigned char new_path[29], int i){
  const char slash = '/';
  int j = 0;
  int h = 0;
  for (int k = 0; k < 29; k++){
    if (j < i){
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
}

int strip_new_path(unsigned char new_path[29]){
  int i = 0;
  while (new_path[i] != NULL){
    i++;
  }
  return i;
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

int update_bitmap(){
  unsigned char *buffer;
  buffer = calloc(2048, sizeof(char));
  int count = 0;
  for (int j = 1; j < 65; j++){
    int number = 2048*j;
    fseek(file, number, SEEK_SET);
    fread(buffer, 2048, 1, file);
    for (int i = 0; i < 2048; i++){
      if (buffer[i] != 0xFF){
        unsigned char buffer_update;
        printf("Byte ejecucion: %X\n", buffer[i]);
        int pos_zero = int_from_byte(buffer[i]);
        buffer_update = update_byte(buffer[i], pos_zero);
        printf("num: %i\n", number+i-1);
        printf("Byte 10: %X\n", buffer_update);
        fseek(file, number+i-1, SEEK_SET);
        fwrite(buffer_update, 1, 1, file);
        count = count + pos_zero;
        return count;
      }
      count = count + 8;
    }
  }
  free(buffer);
}

void int_to_bytes(unsigned char index[3], int block_number){
  index[0] = (block_number >> 16) & 0xFF;
  index[1] = (block_number >> 8) & 0xFF;
  index[2] = block_number & 0xFF;
  index[0] = index[0] | 0x80;
}

int int_from_byte(unsigned char byte){
  for (int i = 7; i >= 0; --i){
    int bit = ((byte & (1 << i)) >> i);
    if (bit == 0){
      return 7-i;
    }
  }
}

unsigned char update_byte(unsigned char byte, int pos_zero){
  unsigned char byte_2;
  printf("Byte 1: %X\n", byte);
  if (pos_zero == 0){
    byte_2 = byte | 0x80;
  }
  else if (pos_zero == 1){
    byte_2 = byte | 0x40;
  }
  else if (pos_zero == 2){
    byte_2 = byte | 0x20;
  }
  else if (pos_zero == 3){
    byte_2 = byte | 0x10;
  }
  else if (pos_zero == 4){
    byte_2 = byte | 0x08;
  }
  else if (pos_zero == 5){
    byte_2 = byte | 0x04;
  }
  else if (pos_zero == 6){
    byte_2 = byte | 0x02;
  }
  else if (pos_zero == 7){
    byte_2 = byte | 0x01;
  }
  printf("Byte 2: %X\n", byte);
  return byte_2;
}
