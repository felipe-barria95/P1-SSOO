#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "os_API.h"
#include <stdlib.h>
#include <math.h>

void os_mount(char* diskname)
{
  file = fopen(diskname, "rb+");
}

void os_bitmap(unsigned num, bool hex)
{ //FALTA imprimir en stderr//
  unsigned char* buffer;
  buffer = calloc(2048, sizeof(char));
  if (num == 0)
  {
    int count = 0;
    for (int j = 1; j < 65; j++)
    {
      int number = 2048 * j;
      fseek(file, number, SEEK_SET);
      fread(buffer, 2048, 1, file);
      printf("Bloque %i\n", j);
      if (hex == true)
      {
        for (int i = 0; i < 2048; i++)
        {
          count += bits_in_char(buffer[i]);
          printf("%02X ", buffer[i]);
        }
      }
      else
      {
        for (int i = 0; i < 2048; i++)
        {
          count += bits_in_char(buffer[i]);
          print_bits(buffer[i]);
          printf(" ");
        }
      }
      printf("\n");
    }
    fseek(file, 0, SEEK_SET);
    printf("Contador: Bloques usados: %i. Bloques libres: %i.\n", count, 1048576 - count);
  }
  else if (num > 0 && num < 65)
  {
    int number = 2048 * num;
    fseek(file, number, SEEK_SET);
    fread(buffer, 2048, 1, file);
    if (hex == true)
    {
      for (int i = 0; i < 2048; i++)
      {
        printf("%02X ", buffer[i]);
      }
    }
    else
    {
      for (int i = 0; i < 2048; i++)
      {
        print_bits(buffer[i]);
        printf(" ");
      }
    }
    printf("\n");
    fseek(file, 0, SEEK_SET);
  }
  free(buffer);
}

int os_exists(char* path)
{
  int posicion = ftell(file);
  printf("##  ##### %i\n", posicion);
  const char slash = '/';
  unsigned char name[29];
  unsigned char index[3];
  unsigned char new_path[29];
  path = path + 1;
  if (path[0] == NULL)
  {
    return 1;
  }
  for (int i = 0; i < 64; i++)
  {
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0)
    {
      if (strchr(path, slash) != NULL)
      {
        strip_path(path, new_path, 1);
        int match = strcmp(new_path, name);
        if (match == 0)
        {
          path = path + strip_new_path(new_path); //obtengo nuevo path, eliminando la carpeta que ya accedi//
          int disk_number = 2048 * block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return os_exists(path);
        }
      }
      else if (strchr(path, slash) == NULL)
      {
        int match = strcmp(path, name);
        if (match == 0)
        {
          fseek(file, 0, SEEK_SET);
          return 1;
        }
      }
    }
  }
  fseek(file, 0, SEEK_SET);
  return 0;
}

void os_ls(char* path)
{
  unsigned char index[3];
  unsigned char name[29];
  for (int i = 0; i < 64; i++)
  {
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0)
    { //si llegamos al directorio destino o si estamos en directorio raiz//
      //if (path[0] == NULL || path[0] == "/")
      int n = 0;
      while (path[n] != NULL)
      {
        n++;
      }
      if (path[0] == NULL || (path[0] == '/' && n == 1))
      {
        printf("%s\n", name); //falta comparar directorio inicial//
      }
      else
      {
        unsigned char new_path[29];
        strip_path(path, new_path, 2);
        int match = strcmp(new_path, name); //si es 0, hay match//
        //FALTA LAST//
        if (match == 0)
        {
          path = path + strip_new_path(new_path) + 1;
          int disk_number = 2048 * block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return os_ls(path);
        }
      }
    }
  }
  printf("==================================\n");
  fseek(file, 0, SEEK_SET);
}

int ret_pos(char* path)
{
  const char slash = '/';
  int pos;
  unsigned char name[29];
  unsigned char index[3];
  unsigned char index_anterior[3];
  unsigned char new_path[29];
  path = path + 1;
  if (path[0] == NULL)
  {
    return 1;
  }
  for (int i = 0; i < 64; i++)
  {
    // copiamos el index anterior
    index_anterior[0] = index[0];
    index_anterior[1] = index[1];
    index_anterior[3] = index[2];
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0)
    {
      if (strchr(path, slash) != NULL)
      {
        strip_path(path, new_path, 1);
        int match = strcmp(new_path, name);
        if (match == 0)
        {
          path = path + strip_new_path(new_path); //obtengo nuevo path, eliminando la carpeta que ya accedi//
          int disk_number = 2048 * block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return ret_pos(path);
        }
      }
      else if (strchr(path, slash) == NULL)
      {
        int match = strcmp(path, name);
        if (match == 0)
        {
          fseek(file, 0, SEEK_SET);
          pos = 2048 * block_number(index);
          return pos;
        }
      }
    }
  }
  fseek(file, 0, SEEK_SET);
  return 0;
}

osFile* os_open(char* path, char mode)
{
  // RETORNAUN PUNTERO DEL ARCHIVO O NULL SI ES QUE HUBO UN ERROR
  // definimos la variable
  OsFile = malloc(sizeof(osFile));
  OsFile->mode = mode;
  printf("puntero al osFile %p\n", OsFile);
  printf("modo: %c\n", OsFile->mode);
  OsFile->pos_direct = 0;
  OsFile->read_buffer = 0;
  printf("abriendo acrhivo: %s\n", path);
  printf("existe? %i\n", os_exists(path));
  // Si es 'r' y el archivo existe
  if (mode == 'r' && os_exists(path))
  {
    fseek(file, 0, SEEK_SET);
    //OsFile->file = fopen(path, 'r');
    int pos;
    // El siguiente bloque es una copia de os exist para encontrar la ubicación del archivo.
    pos = ret_pos(path);
    OsFile->pos_indice = pos;   // averiguamos la posición del directorio del archivo
    fseek(file, pos, SEEK_SET); // llevamos la lectura hasta el bloque de directorio
    unsigned char hardlinks[1];
    fread(hardlinks, 1, 1, file); // leemos el número de hardlinks
    OsFile->n_hardlinks = (int)(hardlinks[0]);
    printf("## numero de hard links: %i\n", OsFile->n_hardlinks);
    unsigned char size[7];
    fread(size, 7, 1, file); // leemos el size del archivo
    OsFile->size = (size[3] << 24) | (size[4] << 16) | (size[5] << 8) | size[6];
    printf("## Tamaño: %i\n", OsFile->size);
    // calculamos la cantidad de punteros a bloques de direccionamineto
    double bloques = ceil(((double)OsFile->size) / 2048);
    printf("## cantidad de bloques; %f\n", bloques);
    OsFile->resto_bloque_data = 2048 - (OsFile->size % 2048);
    printf("## RESTO bloque data: %i\n", OsFile->resto_bloque_data);
    double direccionaminetos = ceil(bloques / 512);
    OsFile->resto_BDS = 512 - (((int)bloques) % 512);
    printf("## RESTO BDS: %i\n", OsFile->resto_BDS);
    OsFile->n_direccionaminetos = (int)direccionaminetos;
    printf("## cantidad de blouqes de direccionamiento indirecto simple: %i\n", OsFile->n_direccionaminetos);
    double resto = OsFile->n_direccionaminetos - 509;
    int adicionales = 0;
    if (resto > 0)
    {
      adicionales = ceil(resto / 511);
    }
    printf("## bloques  que no caben en el indice principal: %f\n", resto);
    OsFile->n_indices_adcicionales = resto;

    fseek(file, 0, SEEK_SET);
    return OsFile;
  }

  // Si es 'w' y el archivo no existe
  else if (mode == 'w')
  {
    fseek(file, 0, SEEK_SET);
    unsigned char folder_path[29];
    printf("## logramos copiar el path?: %s\n", get_folder_path(path, folder_path));

    //memcpy(new_path, get_folder_path(path), 29);
    printf("## logramos copiar el path?: %s\n", folder_path);
    fseek(file, 0, SEEK_SET);
    if (os_exists(folder_path))
    {
      if (!os_exists(path))
      {
        // encontrar 3 bloques desocupados
        int posicion_vacia1;
        int posicion_vacia2;
        int posicion_vacia3;
        posicion_vacia1 = update_bitmap() * 2048;
        printf("## posicon vacia: %i\n", posicion_vacia1);
        posicion_vacia2 = update_bitmap() * 2048;
        printf("## posicon vacia: %i\n", posicion_vacia2);
        posicion_vacia3 = update_bitmap() * 2048;
        printf("## posicon vacia: %i\n", posicion_vacia3);

        int pos_folder;
        pos_folder = ret_pos(folder_path);
        fseek(file, pos_folder, SEEK_SET);
        unsigned char name[29];
        unsigned char index[3];
        for (int i = 0; i < 64; i++)
        {
          fread(index, 3, 1, file);
          fread(name, 29, 1, file);
          if (is_valid(index) == 0)
          {
            fseek(file, pos_folder + 32 * (i - 1), SEEK_SET);
            unsigned char index_1[3];
            int uno = 1;
            index_1[0] = uno << 6;
            index_1[1] = posicion_vacia1 >> 8;
            index_1[3] = posicion_vacia1;
            //printf("## es vacio?: %i\n", is_valid(index_1));
            fwrite(index_1, 3, 1, file);
            unsigned char name[29];
            name[0] = "notfile.txt";
            name[1] = 'n';
            name[2] = 'o';
            name[3] = 't';
            name[4] = 'f';
            name[5] = 'i';
            name[6] = 'l';
            name[7] = 'e';
            name[8] = '.';
            name[9] = 't';
            name[10] = 'x';
            name[11] = 't';
            fwrite(name, 29, 1, file);
            printf("## posicion del indice: %i\n", posicion_vacia1);
            fseek(file, posicion_vacia1, SEEK_SET);
            // ahora en el bloque indice
            unsigned char hardlinks[1];
            hardlinks[0] = 1;
            fwrite(hardlinks, 1, 1, file);
            unsigned char size[7];
            size[0] = 0;
            size[1] = 0;
            size[2] = 0;
            size[3] = 0;
            size[4] = 0;
            size[5] = 0;
            size[6] = 0;
            fwrite(size, 7, 1, file);
            unsigned char puntero_BDS[4];
            puntero_BDS[0] = posicion_vacia2 >> 24;
            puntero_BDS[1] = posicion_vacia2 >> 16;
            puntero_BDS[2] = posicion_vacia2 >> 8;
            puntero_BDS[3] = posicion_vacia2;
            fwrite(puntero_BDS, 4, 1, file);
            fseek(file, posicion_vacia2, SEEK_SET); // nos movemos al bloque indice
            // un vez en el bloque induce
            unsigned char puntero_DATA[4];
            puntero_DATA[0] = posicion_vacia3 >> 24;
            puntero_DATA[1] = posicion_vacia3 >> 16;
            puntero_DATA[2] = posicion_vacia3 >> 8;
            puntero_DATA[3] = posicion_vacia3;
            fwrite(puntero_DATA, 4, 1, file);
            fseek(file, posicion_vacia3, SEEK_SET); // nos movemos al bloque DATA
            return OsFile;
          }
        }
      }
    }
    //fseek(file, 0, SEEK_SET);
    // tengo que seguir el path hasta la ubicación del archivo
    printf("## completamos\n");
  }

  // Si no es ninguo de los dos o el achivo existo o no exite
  else
  {
    printf("ERROR No se pudo abrir el archivo\n");
    free(OsFile);
  }
}

/*
int **get_and_fill_empt_block(int cantidad)
{
  //int arg_pos[cantidad];
  unsigned char bytemap[2048];
  int bitmap;
  //int contador;
  int empty_pos;
  //int contador = 0;
  fseek(file, 2048, SEEK_SET);
  for (int i = 0; i < 64; i++)
  {
    fread(bytemap, 2048, 1, file);
    for (int j = 0; j < 2048; j++)
    {
      if (bytemap[j] != 0xFF)
      {
        bitmap = int_from_byte(bytemap[j]);
        empty_pos = i * 2048 * 8 + j * 8 + bitmap;
        return empty_pos * 2048;
        update_bitmap
        //arg_pos[contador] = empty_pos * 2048;
        //contador++;

        if (contador == cantidad)
        {
          return &arg_pos;
        }
      }
    }
  }
}
*/

int os_read(osFile* file_desc, void* buffer, int nbytes) {
  //int bloques = ceil(nbytes / (float)2048);
  if (file_desc->mode == 'r') {
    if (file_desc->read_buffer == file_desc->size) {
      fprintf(stderr, "Error: No bytes will be read");
      return 0;
    }
    int read = nbytes;
    if ((file_desc->read_buffer + nbytes) >= file_desc->size)
      read = file_desc->size - file_desc->read_buffer;
    int read_block = read;
    int pending_read = read;
    long int aux_bds;
    int aux_DATA;
    unsigned char next_BDS[4];
    unsigned char next_DATA[4];
    //fread(buffer, read, 1, file);
    /* READ = 200
       RESTO = 100 */
    long int sum_bds = 0;
    int contador = 0;
    long int sum_index = 0;
    char* src = malloc(read);
    while (pending_read > 0) { // 200 -> 100
      fseek(file, (long int)(file_desc->pos_indice + 8 + sum_index), SEEK_SET); // NOS PONE EN EL PRIMER PUNTERO A BDS
      //printf("POS INDICE: %i\n", file_desc->pos_indice + 8 + sum_index);
      fread(next_BDS, 4, 1, file);
      aux_bds = (next_BDS[0] << 24) | (next_BDS[1] << 16) | (next_BDS[2] << 8) | next_BDS[3];
      //printf("AUX BDS: %i\n", aux_bds);
      fseek(file, (long int)(aux_bds + sum_bds), SEEK_SET); //NOS PONE EN EL PRIMER PUNTERO AL BLOQUE DATA
      fread(next_DATA, 4, 1, file);
      aux_DATA = (next_DATA[0] << 24) | (next_DATA[1] << 16) | (next_DATA[2] << 8) | next_DATA[3];
      //printf("next_DATA: %s\n", next_DATA);
      //printf("AUX DATA: %i\n", aux_DATA);
      fseek(file, (long int)aux_DATA, SEEK_SET);
      if (pending_read > file_desc->resto_bloque_data) { // 200 > 100? -> 100 > 100?
        read_block = file_desc->resto_bloque_data; // 100
      }
      fread(src, read_block, 1, file); // SE LEEN 100
      //printf("READ: %i\n", read);
      //printf("READ BLOCK: %i\n", read_block);
      //printf("SRC: %c\n", src[0]);
      pending_read -= read_block; // 100
      sum_bds += 4;
      contador++;
      if (contador == file_desc->resto_BDS) {
        contador = 0;
        sum_index += 4;
      }
    }
    file_desc->read_buffer += read;
    src[read] = '\0';
    memcpy(buffer, src, read);
    free(src);
    return read;
  }
  else {
    fprintf(stderr, "Custom error: Incorrect mode");
    return  -1;
  }
};

int os_write(osFile* file_desc, void* buffer, int nbytes) {
  //int bloques = ceil(nbytes / (float)2048);
  if (file_desc->mode == 'w') {
    if (file_desc->write_buffer == file_desc->size) {
      fprintf(stderr, "Error: No bytes will be written");
      return 0;
    }
    int write = nbytes;
    if ((file_desc->write_buffer + nbytes) >= file_desc->size)
      write = file_desc->size - file_desc->write_buffer;
    int write_block = write;
    int pending_write = write;
    long int aux_bds;
    int aux_DATA;
    unsigned char next_BDS[4];
    unsigned char next_DATA[4];
    //fread(buffer, write, 1, file);
    long int sum_bds = 0;
    int contador = 0;
    long int sum_index = 0;
    char* src = malloc(write);
    while (pending_write > 0) {
      fseek(file, (long int)(file_desc->pos_indice + 8 + sum_index), SEEK_SET); // NOS PONE EN EL PRIMER PUNTERO A BDS
      //printf("POS INDICE: %i\n", file_desc->pos_indice + 8 + sum_index);
      fread(next_BDS, 4, 1, file);
      aux_bds = (next_BDS[0] << 24) | (next_BDS[1] << 16) | (next_BDS[2] << 8) | next_BDS[3];
      //printf("AUX BDS: %i\n", aux_bds);
      fseek(file, (long int)(aux_bds + sum_bds), SEEK_SET); //NOS PONE EN EL PRIMER PUNTERO AL BLOQUE DATA
      fread(next_DATA, 4, 1, file);
      aux_DATA = (next_DATA[0] << 24) | (next_DATA[1] << 16) | (next_DATA[2] << 8) | next_DATA[3];
      //printf("next_DATA: %s\n", next_DATA);
      //printf("AUX DATA: %i\n", aux_DATA);
      fseek(file, (long int)aux_DATA, SEEK_SET);
      if (pending_write > file_desc->resto_bloque_data) {
        write_block = file_desc->resto_bloque_data;
      }
      fread(src, write_block, 1, file);
      //printf("READ: %i\n", write);
      //printf("READ BLOCK: %i\n", write_block);
      //printf("SRC: %c\n", src[0]);
      pending_write -= write_block;
      sum_bds += 4;
      contador++;
      if (contador == file_desc->resto_BDS) {
        contador = 0;
        sum_index += 4;
      }
    }
    file_desc->write_buffer += write;
    src[write] = '\0';
    memcpy(src, buffer, write);
    free(src);
    return write;
  }
  else {
    fprintf(stderr, "Custom error: Incorrect mode");
    return  -1;
  }
};

//int os_write(osFile* file_desc, void* buffer, int nbytes)
//{
//  unsigned char* aux_buffer;
//  aux_buffer = calloc(2048, sizeof(char));
//  int count = 0;
//  for (int j = 1; j < 65; j++)
//  {
//    int number = 2048 * j;
//    fseek(file, number, SEEK_SET);
//    fread(aux_buffer, 2048, 1, file);
//    for (int i = 0; i < 2048; i++)
//      count += bits_in_char(aux_buffer[i]);
//  }
//  free(aux_buffer);
//  fseek(file, 0, SEEK_SET);
//  int free_blocks = 1048576 - count; // CANTIDAD DE BLOQUES DISPONIBLES
//  float temp_blocks = (float)nbytes / 2048;
//  int blocks = ceil(temp_blocks) + 1; // CANTIDAD DE BLOQUES QUE SE NECESITAN
//  if (file_desc->mode == 'w')
//  {
//    fseek(file, file_desc->pos_indice + 8, SEEK_SET); // NOS PONE EN EL PRIMER PUNTERO A BDS
//    unsigned char next_BDS[4];
//    fread(next_BDS, 4, 1, file);
//    int aux_bds;
//    aux_bds = (next_BDS[0] << 24) | (next_BDS[1] << 16) | (next_BDS[2] << 8) | next_BDS[3];
//    fseek(file, aux_bds, SEEK_SET); //NOS PONE EN EL PRIMER PUNTERO AL BLOQUE DATA
//    unsigned char next_DATA[4];
//    fread(next_DATA, 4, 1, file);
//    int aux_DATA;
//    aux_DATA = (next_DATA[0] << 24) | (next_DATA[1] << 16) | (next_DATA[2] << 8) | next_DATA[3];
//    fseek(file, aux_DATA, SEEK_SET);
//    char DATA_1[2048];
//    fread(DATA_1, 2048, 1, file);
//    if (free_blocks >= blocks) // SI LA CANTIDAD DE BLOQUES DISPONIBLES ALCANZA PARA ESCRIBIR
//    {
//      if (nbytes <= file_desc->resto_bloque_data)
//      { // SI SE ALCANZA A ESCRIBIR EN UN BLOQUE
//        char* src = malloc(nbytes + 1);
//        memcpy(src, buffer, nbytes);
//        src[nbytes] = '\0';
//        memcpy(DATA_1, src, nbytes);
//        free(src);
//        return nbytes;
//      }
//      else
//      { // SI SE NECESITA ESCRIBIR RECURSIVAMENTE EN MÁS BLOQUES
//        int count = 0;
//        char* src = malloc(file_desc->resto_bloque_data + 1);
//        memcpy(src, buffer + file_desc->write_buffer, free_blocks * 2048 - 1);
//        file_desc->write_buffer += free_blocks * 2048;
//        src[free_blocks * 2048] = '\0';
//        memcpy(DATA_1, src, file_desc->resto_bloque_data);
//        free(src);
//        count += os_write(file_desc, buffer, nbytes);
//        return count;
//      }
//    }
//    else
//    { // SI NO ME ALCANZA, NO SE ESCRIBE
//      fprintf(stderr, "Error: No space available\n");
//      return 0;
//    }
//  }
//  return 0;
//}

void os_close(osFile* file_desc)
{
  free(file_desc);
}

void os_rm(char* path)
{
  const char slash = '/';
  unsigned char name[29];
  unsigned char index[3];
  unsigned char new_path[29];
  path = path + 1;
  for (int i = 0; i < 64; i++)
  {
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0)
    {
      if (strchr(path, slash) != NULL)
      {
        strip_path(path, new_path, 1);
        int match = strcmp(new_path, name);
        if (match == 0)
        {
          path = path + strip_new_path(new_path); //obtengo nuevo path, eliminando la carpeta que ya accedi//
          int disk_number = 2048 * block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return os_rm(path);
        }
      }
      else if (strchr(path, slash) == NULL)
      {
        printf("Path1 rm: %s\n", name);
        printf("Path2 rm: %s\n", path);
        int match = strcmp(path, name);
        if (match == 0)
        {
          printf("path: %s\n", path);
          unsigned long position;
          fflush(file);
          position = ftell(file) - 32;
          rm_file_mem_dir(block_number(index));
          fseek(file, position, SEEK_SET);
          unsigned char zero[0];
          zero[0] = NULL;
          fwrite(zero, 1, 32, file);
          fseek(file, 0, SEEK_SET);
          break;
        }
      }
    }
  }
}

int os_hardlink(char* orig, char* dest)
{
}



int os_mkdir(char* path)
{
  int count = 0;
  const char slash = '/';
  unsigned char new_path[29];
  path = path + 1;
  strip_path(path, new_path, 1);
  printf("Path 1: %s\n", path);
  if (strchr(path, slash) != NULL)
  { //si no llegamos ¡al directorio destino o si estamos en directorio raiz//
    for (int i = 0; i < 64; i++)
    {
      unsigned char index[3];
      unsigned char name[29];
      printf("new path: %s\n", new_path);
      fread(index, 3, 1, file);
      fread(name, 29, 1, file);
      if (is_valid(index) > 0)
      {
        printf("name: %s\n", name);
        int match = strcmp(new_path, name);
        if (match == 0)
        {
          path = path + strip_new_path(new_path);
          int disk_number = 2048 * block_number(index);
          printf("Disk numerb: %i\n", disk_number);
          fseek(file, disk_number, SEEK_SET);
          return os_mkdir(path);
        }
      }
    }
  }
  else
  {
    unsigned char index_2[3];
    unsigned char name_2[29];
    for (int j = 0; j < 64; j++)
    {
      fread(index_2, 3, 1, file);
      fread(name_2, 29, 1, file);
      if (is_valid(index_2) == 0)
      {
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

int os_rmdir(char* path, bool recursive)
{
  const char slash = '/';
  unsigned char name[29];
  unsigned char index[3];
  unsigned char new_path[29];
  path = path + 1;
  if (path[0] == NULL)
  {
    return 1;
  }
  for (int i = 0; i < 64; i++)
  {
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0)
    {
      if (strchr(path, slash) != NULL)
      {
        strip_path(path, new_path, 1);
        int match = strcmp(new_path, name);
        if (match == 0)
        {
          path = path + strip_new_path(new_path); //obtengo nuevo path, eliminando la carpeta que ya accedi//
          int disk_number = 2048 * block_number(index);
          fseek(file, disk_number, SEEK_SET);
          return os_rmdir(path, recursive);
        }
      }
      else if (strchr(path, slash) == NULL)
      {
        int match = strcmp(path, name);
        if (match == 0)
        {
          unsigned long position_2;
          fflush(file);
          position_2 = ftell(file);
          printf("nombre jfda: %s\n", name);
          if (recursive == true) {
            int disk_number_2 = 2048 * block_number(index);
            rm_recursive(disk_number_2);
          }
          fseek(file, position_2, SEEK_SET);
          unsigned char name_2[29];
          unsigned char index_2[3];
          unsigned char zero[0];
          zero[0] = NULL;
          int disk_number = 2048 * block_number(index);
          printf("Block number rmrem: %i\n", block_number(index));
          unsigned long position;
          fflush(file);
          position = ftell(file) - 32;
          update_remove_bitmap(block_number(index));
          printf("positibon: %i\n", position);
          fseek(file, disk_number, SEEK_SET);
          for (int j = 0; j < 64; j++) {
            fread(index_2, 3, 1, file);
            fread(name_2, 29, 1, file);
            if (is_valid(index_2) > 0) {
              printf("La carpeta no esta vacia\n");
              return 0;
            }
          }
          fseek(file, position, SEEK_SET);
          fwrite(zero, 1, 32, file);
          fseek(file, 0, SEEK_SET);
          return 1;
        }
      }
    }
  }
  fseek(file, 0, SEEK_SET);
  return 0;
}

void os_unload(char* orig, char* dest)
{
}

void os_load(char* orig)
{
}

void print_ls()
{
  for (int i = 0; i < 64; i++)
  {
    unsigned char name[29];
    unsigned char index[3];
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0)
    {
      printf("%s\n", name);
    }
  }
  fseek(file, 0, SEEK_SET);
}

int is_valid(unsigned char* bits)
{
  return bits[0] >> 6;
}

unsigned char* get_folder_path(char* path, unsigned char new_path[29])
{
  //unsigned char new_path[29];
  new_path[0] = '/';
  // contar cantidad de "/"s
  int n = 0;
  int cantidad = 0;
  while (path[n] != NULL)
  {
    if (path[n] == '/')
    {
      cantidad++;
    }
    n++;
  }
  printf("## cantidad de slashs: %i\n", cantidad);

  // ahora dejamos el path no n-1 slash
  const char slash = '/';
  int j = 0;
  int h = 1;
  printf("## path: %s\n", path);
  for (int k = 0; k < 29; k++)
  {
    if (j < cantidad)
    {
      printf("## caracter del path: %c\n", path[k]);
      if (path[k] == slash)
      {
        printf("## J: %i\n", j);
        j++;
        printf("un slaaaaash\n");
      }
      else
      {

        new_path[h] = path[k];
        h++;
      }
    }
    else
    {
      new_path[h] = NULL;
      h++;
    }
  }
  printf("## nuevo path: %s\n", new_path);
  return new_path;
}

int block_number(unsigned char* bits)
{
  return ((bits[0] & 0x3F) << 16) | (bits[1] << 8) | bits[2];
}

void strip_path(char* path, unsigned char new_path[29], int i) {

  const char slash = '/';
  int h = 0;
  int j = 0;
  for (int k = 0; k < 29; k++)
  {
    if (j < i)
    {
      if (path[k] == slash)
      {
        j++;
      }
      else
      {
        new_path[h] = path[k];
        h++;
      }
    }
    else
    {
      new_path[h] = NULL;
      h++;
    }
  }
}

int strip_new_path(unsigned char new_path[29])
{
  int i = 0;
  while (new_path[i] != NULL)
  {
    i++;
  }
  return i;
}

void print_bits(unsigned char val)
{
  for (int i = 7; i >= 0; i--)
  {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}

int bits_in_char(unsigned char val)
{
  int count = 0;
  for (int i = 7; i >= 0; --i)
  {
    int bit = ((val & (1 << i)) >> i);
    count += bit;
  }
  return count;
}

int update_bitmap()
{
  unsigned char* buffer;
  buffer = calloc(2048, sizeof(char));
  int count = 0;
  for (int j = 1; j < 65; j++)
  {
    int number = 2048 * j;
    fseek(file, number, SEEK_SET);
    fread(buffer, 2048, 1, file);
    for (int i = 0; i < 2048; i++)
    {
      if (buffer[i] != 0xFF)
      {
        unsigned char buffer_update[0];
        int pos_zero = int_from_byte(buffer[i]);
        buffer_update[0] = update_byte(buffer[i], pos_zero);
        fseek(file, number + i, SEEK_SET);
        fwrite(buffer_update, 1, 1, file);
        count = count + pos_zero;
        return count;
      }
      count = count + 8;
    }
  }
  free(buffer);
}

void int_to_bytes(unsigned char index[3], int block_number)
{
  index[0] = (block_number >> 16) & 0xFF;
  index[1] = (block_number >> 8) & 0xFF;
  index[2] = block_number & 0xFF;
  index[0] = index[0] | 0x80;
}

int int_from_byte(unsigned char byte)
{
  for (int i = 7; i >= 0; --i)
  {
    int bit = ((byte & (1 << i)) >> i);
    if (bit == 0)
    {
      return 7 - i;
    }
  }
}

unsigned char update_byte(unsigned char byte, int pos_zero)
{
  unsigned char byte_2 = "\0";
  if (pos_zero == 0)
  {
    byte_2 = byte | 0x80;
  }
  else if (pos_zero == 1)
  {
    byte_2 = byte | 0x40;
  }
  else if (pos_zero == 2)
  {
    byte_2 = byte | 0x20;
  }
  else if (pos_zero == 3)
  {
    byte_2 = byte | 0x10;
  }
  else if (pos_zero == 4)
  {
    byte_2 = byte | 0x08;
  }
  else if (pos_zero == 5)
  {
    byte_2 = byte | 0x04;
  }
  else if (pos_zero == 6)
  {
    byte_2 = byte | 0x02;
  }
  else
  {
    byte_2 = byte | 0x01;
  }
  return byte_2;
}

void update_remove_bitmap(int index) {
  unsigned char buffer[0];
  int blocks_per_blocks = 16384;
  int result = index / blocks_per_blocks;
  int resto = index % blocks_per_blocks;
  int resto_2 = resto % 8;
  resto = resto / 8;
  printf("resultado :%i\n", resto_2);
  int disk_number_2 = (result + 1) * 2048 + resto;
  printf("disk numerb 2: %i\n", disk_number_2);
  fseek(file, disk_number_2, SEEK_SET);
  fread(buffer, 1, 1, file);
  unsigned char new_buffer[0];
  new_buffer[0] = obtain_new_buffer(buffer[0], resto_2);
  fseek(file, disk_number_2, SEEK_SET);
  fwrite(new_buffer, 1, 1, file);
  printf("Bufferfsafsfsa: %X\n", new_buffer[0]);
}

unsigned char obtain_new_buffer(unsigned char byte, int pos_zero) {
  unsigned char byte_2 = "\0";
  if (pos_zero == 0)
  {
    byte_2 = byte & 0x7F;
  }
  else if (pos_zero == 1)
  {
    byte_2 = byte & 0xBF;
  }
  else if (pos_zero == 2)
  {
    byte_2 = byte & 0xDF;
  }
  else if (pos_zero == 3)
  {
    byte_2 = byte & 0xEF;
  }
  else if (pos_zero == 4)
  {
    byte_2 = byte & 0xF7;
  }
  else if (pos_zero == 5)
  {
    byte_2 = byte & 0xFB;
  }
  else if (pos_zero == 6)
  {
    byte_2 = byte & 0xFD;
  }
  else
  {
    byte_2 = byte & 0xFE;
  }
  return byte_2;
}

void rm_recursive(int mem_dir) {
  fseek(file, mem_dir, SEEK_SET);
  unsigned char name[29];
  unsigned char index[3];
  for (int i = 0; i < 64; i++) {
    fread(index, 3, 1, file);
    fread(name, 29, 1, file);
    if (is_valid(index) > 0) {
      printf("nombre dentro del : %s\n", name);
    }
    if (is_valid(index) == 1) {
      printf("Arcivho\n");
      rm_file_mem_dir(block_number(index));
      fseek(file, mem_dir + 32 * (i - 1), SEEK_SET);
      unsigned char zero[0];
      zero[0] = NULL;
      fwrite(zero, 1, 32, file);
      fseek(file, mem_dir + 32 * i, SEEK_SET);
    }
    else if (is_valid(index) == 2) {
      printf("Carpeta\n");
      int disk_number_3 = 2048 * block_number(index);
      rm_recursive(disk_number_3);
      fseek(file, mem_dir + 32 * (i - 1), SEEK_SET);
      unsigned char zero[0];
      zero[0] = NULL;
      fwrite(zero, 1, 32, file);
      fseek(file, mem_dir + 32 * i, SEEK_SET);
    }
  }
  update_remove_bitmap(mem_dir / 2048);
}

void rm_file_mem_dir(int mem_dir) {
  unsigned char hardlinks[0];
  unsigned char file_size[7];
  unsigned char* pointers_ref;
  pointers_ref = calloc(2036, sizeof(char));
  unsigned char index_block[4];
  fread(hardlinks, 1, 1, file);
  fread(file_size, 7, 1, file);
  fread(pointers_ref, 2036, 1, file);
  fread(index_block, 4, 1, file);
  if (hardlinks == 0) {
    update_remove_bitmap(mem_dir / 2048);
    //borrar todo los bloques dentro del archivo
  }
  free(pointers_ref);
}

void os_desmontar()
{
  fclose(file);
}