
FILE* file;

typedef struct File {
  int numero; //
  char mode; // Para saber si es 'w' o 'r'
  char* nombre; // Corresponde al nombre del archivo, qyuizas podemnos agrgar una extensión
  unsigned int pos_direct; // posición del bloque directorio
  unsigned int pos_indice; // posición del bloque indice
  int size; //obtener del bloque indice//
  int n_hardlinks;
  unsigned int direccionamiento[504];//
  int read_buffer;
} osFile;

void os_mount(char* diskname);

void os_bitmap(unsigned num, bool hex);

int os_exists(char* path);

void os_ls(char* path);

osFile* os_open(char* path, char mode);

int os_read(osFile* file_desc, void* buffer, int nbytes);

int os_write(osFile* file_desc, void* buffer, int nbytes);

void os_close(osFile* file_desc);

void os_rm(char* path);

int os_hardlink(char* orig, char* dest);

int os_mkdir(char* path);

int os_rmdir(char* path, bool recursive);

void os_unload(char* orig, char* dest);

void os_load(char* orig);

void print_ls();

int is_valid(unsigned char* bits);

int block_number(unsigned char* bits);

void strip_path(char* path, unsigned char new_path[29], int i);

int strip_new_path(unsigned char new_path[29]);

void print_bits(unsigned char val);

int bits_in_char(unsigned char val);

int update_bitmap();

void int_to_bytes(unsigned char index[3], int block_number);
