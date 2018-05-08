#include <stdio.h>
#include <stdlib.h>

int getNumberLines(FILE *file);
int getLine(FILE *file, char *buffer, size_t length);
static int endOfLine(FILE *ifp, int c);

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

    int lines = getNumberLines(file_in);
    unsigned int memory[lines];
    char line[10];
    int i = 0;
    fseek(file_in, 0, SEEK_SET);
   
    while ((getLine(file_in, line, 11)) != EOF)
    {
        memory[i] = (unsigned int) strtoul(line, NULL, 0);
        i++;
    }
    
    return 0;
}

int getNumberLines(FILE *file)
{
    int lines = 0;
    char ch;
    while (!feof(file))
    {
        ch = fgetc(file);
        if (ch == '\n')
            lines++;
    }
    return lines;
}

int getLine(FILE *file, char *buffer, size_t length)
{
    char *end = buffer + length - 1;
    char *dst = buffer;
    int c;
    while ((c = getc(file)) != EOF && !endofline(file, c) && dst < end)
        *dst++ = c;
    *dst = '\0';
    return ((c == EOF && dst == buffer) ? EOF : dst - buffer);
}

static int endOfLine(FILE *ifp, int c)
{
    int eol = (c == '\r' || c == '\n');
    if (c == '\r')
    {
        c = getc(ifp);
        if (c != '\n' && c != EOF)
            ungetc(c, ifp);
    }
    return(eol);
}