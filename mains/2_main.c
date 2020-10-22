#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
    printf("SE MONTA\n");
    os_mount(argv[1]);
    printf("BITMAP DE BLOQUE 1 BIN: \n");
    os_bitmap(1, false);
    printf("BITMAP DE BLOQUE 1 HEX: \n");
    os_bitmap(1, true);
    printf("BITMAP COMPLETO HEX: \n");
    os_bitmap(0, true);
    printf("SE DESMONTA\n");
    os_unmount();
    return 0;
}