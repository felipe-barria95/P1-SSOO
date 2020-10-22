#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
    printf("SE MONTA\n");
    os_mount(argv[1]);
    printf("SE ABRE ARCHIVO EN MODO R\n");
    unsigned char read_buffer[56000];
    int read = os_read(os_open("/IMPORTANT.txt", 'r'), read_buffer, 56000);
    printf("CANTIDAD DE BYTES LEIDOS ? -> %i\n", read);
    if (read > 0) {
        printf("CONTENIDO DEL ARCHIVO\n");
        printf("%s", read_buffer);
    }
    printf("\n");
    printf("SE CIERRAN ARCHIVOS Y SE DESMONTA\n");
    os_unmount();
    return 0;
}