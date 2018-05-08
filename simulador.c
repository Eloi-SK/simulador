#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Argumento inválido.");
        return -1;
    }

    FILE *file_in = fopen(argv[1], "r");
    FILE *file_out = fopen(argv[2], "w");

    if (file_in == NULL)
    {
        printf("Erro ao carregar arquivo.");
        return -1;
    }
    return 0;
}