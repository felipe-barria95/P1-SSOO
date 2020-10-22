#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

int main(int argc, char** argv)
{
    printf("SE MONTA\n");
    os_mount(argv[1]);
    printf("EXISTE /IMPORTANT.txt ? -> %i\n", os_exists("/IMPORTANT.txt"));
    printf("EXISTE /memes/generic (3).png ? -> %i\n", os_exists("/memes/generic (3).png"));
    printf("EXISTE /folder/god/shrek.mp4 ? -> %i\n", os_exists("/folder/god/shrek.mp4"));
    printf("EXISTE /noexisto ? -> %i\n", os_exists("/noexisto"));
    printf("SE DESMONTA\n");
    os_unmount();
    return 0;
}