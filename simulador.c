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

    printf("[START OF SIMULATION]\n");
    fprintf(file_out, "[START OF SIMULATION]\n");

    unsigned int pc = 0, tmp, opcode, ir, er, fr;
    fr = 0;
    unsigned int reg[32];
    for (i = 0; i < 32; i++) reg[i] = 0;
    int exit_ = 0;

    while(exit_ == 0)
    {
        ir = memory[pc];
        opcode = (ir & 0xFC000000) >> 26;

        switch(opcode)
        {
            // nop/add
            case 0x00:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // sub
            case 0x01:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // mul
            case 0x02:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // div
            case 0x03:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // cmp
            case 0x04:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // shl
            case 0x05:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // shr
            case 0x06:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // and
            case 0x07:
                printf("Not implemented.\n");
                pc++;
                break;
            // not
            case 0x08:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // or
            case 0x09:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // xor
            case 0x0A:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // addi
            case 0x10:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // subi
            case 0x11:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // muli
            case 0x12:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // divi
            case 0x13:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // cmpi
            case 0x14:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // andi
            case 0x15:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // noti
            case 0x16:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // ori
            case 0x17:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // ori
            case 0x18:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // ldw   
            case 0x19:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // stw   
            case 0x1A:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // ldb   
            case 0x1B:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // stb   
            case 0x1C:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // bun
            case 0x20:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // beq   
            case 0x21:
                printf("Not implemented.\n");
                pc++;
                break;
            // blt   
            case 0x22:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // bgt   
            case 0x23:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // bne   
            case 0x24:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // ble   
            case 0x25:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // bge   
            case 0x26:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // int   
            case 0x3F:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            default:
                exit_ = 1;
                break;
        }
        
    }

    printf("[END OF SIMULATION]\n");
    fprintf(file_out, "[END OF SIMULATION]");

    system("pause");
    fclose(file_in);
    fclose(file_out);

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