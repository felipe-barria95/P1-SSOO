#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
    printf("SE MONTA\n");
    os_mount(argv[1]);
    os_ls("/folder");
    printf("SE CREA test EN ROOT/FOLDER\n");
    os_mkdir("/folder/test2");
    printf("PODEMOS VER QUE SE CREA test YA EN ROOT/FOLDER\n");
    os_ls("/folder");
    printf("INTENTAMOS CREAR folder EN ROOT\n");
    os_mkdir("/folder");
    printf("SE DESMONTA\n");
    os_unmount();
    return 0;
}