#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
    printf("SE MONTA\n");
    os_mount(argv[1]);
    os_ls("/folder");
    printf("SE ELIMINA test EN ROOT/FOLDER\n");
    os_rmdir("/folder/test", false);
    printf("PODEMOS VER QUE SE ELIMINA test EN ROOT/FOLDER\n");
    os_ls("/folder");
    printf("INTENTAMOS ELIMINAR test nuevamente EN ROOT/FOLDER\n");
    os_rmdir("/folder/test", false);
    os_ls("/");
    printf("SE ELIMINA memes EN ROOT CON RECURSIÓN\n");
    os_rmdir("/memes", true);
    os_ls("/");
    printf("SE DESMONTA\n");
    os_unmount();
    return 0;
}