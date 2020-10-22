#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
    printf("SE MONTA\n");
    os_mount(argv[1]);
    os_ls("/folder");
    printf("SE ELIMINA napkin.tkt QUE ESTÁ EN ROOT/FOLDER\n");
    os_rm("/folder/napkin.txt");
    printf("PODEMOS VER QUE YA NO EXISTE EN ROOT/FOLDER\n");
    os_ls("/folder");
    printf("SE INTENTA ELIMINAR noexisto.txt QUE ESTÁ EN ROOT\n");
    os_rm("/noexisto.txt");
    printf("SE DESMONTA\n");
    os_unmount();
    return 0;
}