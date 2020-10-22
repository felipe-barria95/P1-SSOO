#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
    printf("SE MONTA\n");
    os_mount(argv[1]);
    printf("LS A ROOT: \n");
    os_ls("/");
    printf("LS A ROOT/MEMES: \n");
    os_ls("/memes");
    printf("LS A ROOT/FOLDER/GOD: \n");
    os_ls("/folder/god");
    printf("LS A INEXISTENTE: \n");
    os_ls("/noexisto");
    printf("SE DESMONTA\n");
    os_unmount();
    return 0;
}