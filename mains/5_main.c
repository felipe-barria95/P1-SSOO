#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
    printf("SE MONTA\n");
    os_mount(argv[1]);
    printf("SE ABRE ARCHIVO EN MODO R");
    os_open("/IMPORTANT.txt", 'r');
    printf("SE CIERRAN ARCHIVOS Y SE DESMONTA\n");
    os_unmount();
    return 0;
}