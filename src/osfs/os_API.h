
FILE* file;
typedef struct File
{
  int numero;              //
  char mode;               // Para saber si es 'w' o 'r'
  char* nombre;            // Corresponde al nombre del archivo, qyuizas podemnos agrgar una extensión
  unsigned int pos_direct; // posición del bloque directorio
  unsigned int pos_indice; // posición del bloque indice
  int write_buffer;
  int read_buffer;
  int remaining_block;
  unsigned long int size;               // tamaño del archivo
  int n_hardlinks;                      // número de hardlinks
  unsigned int direccionamientos[1024]; //
  int n_direccionaminetos;
  int n_indices_adcicionales;
  int ultima_posicion;
  unsigned char data[2048];    // aqui se escribe la data
  int resto_bloque_data; // lo que queda libre en el último bloque de data
  int resto_BDS;         // lo que queda libre en el último BDS
} osFile;

osFile* OsFile;

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

void os_unmount();

unsigned char* get_folder_path(char* path, unsigned char new_path[29]);

int int_from_byte(unsigned char byte);

unsigned char update_byte(unsigned char byte, int pos_zero);

void update_remove_bitmap(int index);

unsigned char obtain_new_buffer(unsigned char byte, int pos_zero);

void rm_recursive();

void rm_file_mem_dir(int mem_dir);

void os_ls_recursive(char* path);

int os_mkdir_recursive(char* path);

int os_exists_path_before(char* path);

void os_rm_recursive(char* path);

int os_rmdir_recursive(char* path, bool recursive);

int block_number_index(unsigned char* bits);
