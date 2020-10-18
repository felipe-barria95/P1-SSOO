#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "os_API.h"
#include <stdlib.h>
#include <math.h>

void os_mount(char* diskname)
{
  file = fopen(diskname, "r");
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
  }
  free(buffer);
}

int os_exists(char* path)
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
  const char slash = '/';
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

    // aqui calculamos los bytes que quedan en el bloque de data:

    //vamos al primer puntero a bloque de direccionamiento siemple
    //unsigned char dir_simp[4];
    //fread(dir_simp, 4, 1, file);

    fseek(file, 0, SEEK_SET);
    return OsFile;
  }

  // Si es 'w' y el archivo no existe
  else if (mode == 'w')
  {
    get_folder_path(path);
    //fseek(file, 0, SEEK_SET);
    // tengo que seguir el path hasta la ubicación del archivo
    printf("## completamos\n");
    return OsFile;
  }

  // Si no es ninguo de los dos o el achivo existo o no exite
  else
  {
    printf("ERROR No se pudo abrir el archivo\n");
    free(OsFile);
    return NULL;
  }
}

int os_read(osFile* file_desc, void* buffer, int nbytes)
{
  // Esta funcion deviera funcionar solo si el archivo está abierto y existe el buffer
  // retorna la cantidad de byts leidos
  if (file_desc->mode == 'r')
  {
    //int avance = fread(buffer, nbytes, 1, file_desc->file);
    //file_desc->pos_direct += avance;
    //return avance;
  }
  else
  {
    printf("ERROR, este archivo no está en modo lectura\n");
  }
}

int os_write(osFile* file_desc, void* buffer, int nbytes)
{
  unsigned char* aux_buffer;
  aux_buffer = calloc(2048, sizeof(char));
  int count = 0;
  for (int j = 1; j < 65; j++)
  {
    int number = 2048 * j;
    fseek(file, number, SEEK_SET);
    fread(buffer, 2048, 1, file);
    for (int i = 0; i < 2048; i++)
      count += bits_in_char(aux_buffer[i]);
  }
  free(aux_buffer);
  fseek(file, 0, SEEK_SET);
  int free_blocks = 1048576 - count; // CANTIDAD DE BLOQUES DISPONIBLES
  float temp_blocks = (float)nbytes / 2048;
  int blocks = ceil(temp_blocks) + 1; // CANTIDAD DE BLOQUES QUE SE NECESITAN
  if (file_desc->mode == 'r')
  {
    printf("%i\n", file_desc->pos_indice);
    fseek(file, file_desc->pos_indice + 8, SEEK_SET); // NOS PONE EN EL PRIMER PUNTERO A BDS
    unsigned char next_BDS[4];
    fread(next_BDS, 4, 1, file);
    int aux_bds;
    aux_bds = (next_BDS[0] << 24) | (next_BDS[1] << 16) | (next_BDS[2] << 8) | next_BDS[3];
    printf("ESTO ES EL PUNTERO A DATA: %i\n", aux_bds);
    fseek(file, aux_bds, SEEK_SET); //NOS PONE EN EL PRIMER PUNTERO AL BLOQUE DATA
    unsigned char next_DATA[4];
    fread(next_DATA, 4, 1, file);
    int aux_DATA;
    aux_DATA = (next_DATA[0] << 24) | (next_DATA[1] << 16) | (next_DATA[2] << 8) | next_DATA[3];
    fseek(file, aux_DATA, SEEK_SET);
    char DATA_1[2048];
    fread(DATA_1, 2048, 1, file);
    printf("ESTO ES DATA: %c, %c, %c\n", DATA_1[0], DATA_1[1], DATA_1[2]);
    if (free_blocks >= blocks) // SI LA CANTIDAD DE BLOQUES DISPONIBLES ALCANZA PARA ESCRIBIR
    {
      if (nbytes <= free_blocks)
      { // SI SE ALCANZA A ESCRIBIR EN UN BLOQUE
        char* src = malloc(nbytes + 1);
        printf("Se escribió parte del archivo\n");
        memcpy(src, buffer + 0, nbytes);
        src[nbytes] = '\0';
        printf("Se escribió parte del archivo 1\n");
        //memcpy("archivo", src, nbytes);
        printf("Se escribió parte del archivo 2\n");
        free(src);
        return nbytes;
      }
      else
      { // SI SE NECESITA ESCRIBIR RECURSIVAMENTE EN MÁS BLOQUES
        int count = 0;
        char* src = malloc(free_blocks * 2048);
        memcpy(src, buffer + file_desc->write_buffer, free_blocks * 2048 - 1);

        file_desc->write_buffer += free_blocks * 2048;
        src[free_blocks * 2048] = '\0';
        printf("Se escribió parte del archivo\n");
        free(src);
        count += os_write(file_desc, buffer, nbytes);
        return count;
      }
    }
    else
    { // SI NO ME ALCANZA, NO SE ESCRIBE
      fprintf(stderr, "Error: No space available\n");
      return 0;
    }
  }
  return 0;
}

void os_close(osFile* file_desc)
{
  if (file_desc->mode == 'w')
  {
    if (file_desc->size > 0)
    {
      printf("");
    }
  }
  free(file_desc);
}

void os_rm(char* path)
{
}

int os_hardlink(char* orig, char* dest)
{
}

int os_mkdir(char* path)
{
  const char slash = '/';
  unsigned char new_path[29];
  path = path + 1;
  strip_path(path, new_path, 1);
  printf("Path 1: %s\n", path);
  if (strchr(path, slash) != NULL)
  { //si no llegamos al directorio destino o si estamos en directorio raiz//
    for (int i = 0; i < 64; i++)
    {
      unsigned char index[3];
      unsigned char name[29];
      fread(index, 3, 1, file);
      fread(name, 29, 1, file);
      if (is_valid(index) > 0)
      {
        int match = strcmp(new_path, name);
        if (match == 0)
        {
          path = path + strip_new_path(new_path);
          int disk_number = 2048 * block_number(index);
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
        fseek(file, -32, SEEK_SET);
        unsigned long position;
        fflush(file);
        position = ftell(file);

        int asigned_block = update_bitmap();
        int_to_bytes(index_2, asigned_block);
        //obtenemos el valor de index que deberia tener y lo guardamos en index_2. Asignamos tambien el bloque de direccion y actualizamos el bitmap// FALTA
        fseek(file, position, SEEK_SET);
        fwrite(index_2, 3, 1, file);

        fseek(file, 0, SEEK_SET);
        return 1;
      }
    }
  }
}

int os_rmdir(char* path, bool recursive)
{
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

int block_number(unsigned char* bits)
{
  return ((bits[0] & 0x3F) << 16) | (bits[1] << 8) | bits[2];
}

void strip_path(char* path, unsigned char new_path[29], int i)
{
  const char slash = '/';
  int j = 0;
  int h = 0;
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

void get_folder_path(char* path)
{
  unsigned char new_path[29];
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
  int h = 0;
  printf("## path: %s\n", path);
  for (int k = 0; k < 29; k++)
  {
    if (j < cantidad - 1)
    {
      if (path[k] == slash)
      {
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
        printf("agregar desface en byte\n");
        return count;
      }
      count = count + 8;
    }
  }
}

void int_to_bytes(unsigned char index[3], int block_number)
{
  index[0] = (block_number >> 16) & 0xFF;
  index[1] = (block_number >> 8) & 0xFF;
  index[2] = block_number & 0xFF;
  index[0] = index[0] + 0x40;
}

void os_desmontar()
{
  fclose(file);
}
