#include <stdio.h>
#include <stdlib.h>

int getNumberLines(FILE *file);
int getLine(FILE *file, char *buffer, size_t length);
static int endOfLine(FILE *ifp, int c);
unsigned int bun(unsigned int ir, unsigned int pc, FILE *file);
unsigned int ldw(unsigned int ir, unsigned int pc, unsigned int *mem, unsigned int *reg, FILE *file);
unsigned int stw(unsigned int ir, unsigned int pc, unsigned int *mem, unsigned int *reg, FILE *file);
unsigned int ldb(unsigned int ir, unsigned int pc, unsigned int *mem, unsigned int *reg, FILE *file);
unsigned int cmpi(unsigned int ir, unsigned int pc, unsigned int *reg, unsigned int *fr, FILE *file);
unsigned int addi(unsigned int ir, unsigned int pc, unsigned int *reg, unsigned int *fr, FILE *file);
unsigned int cmp(unsigned int ir, unsigned int pc, unsigned int *reg, unsigned int *fr, FILE *file);
unsigned int bgt(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file);
unsigned int beq(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file);
unsigned int blt(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file);
unsigned int bne(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file);
unsigned int ble(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file);
unsigned int bge(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file);

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
    er = 0;
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
                pc = cmp(ir, pc, reg, &fr, file_out);
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
                pc = addi(ir, pc, reg, &fr, file_out);
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
                pc = cmpi(ir, pc, reg, &fr, file_out);
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
                pc = ldw(ir, pc, memory, reg, file_out);
                break;
            // stw   
            case 0x1A:
                pc = stw(ir, pc, memory, reg, file_out);
                break;
            // ldb   
            case 0x1B:
                pc = ldb(ir, pc, memory, reg, file_out);
                break;
            // stb   
            case 0x1C:
                printf("Not implemented.\n");
                exit_ = 1;
                break;
            // bun
            case 0x20:
                pc = bun(ir, pc, file_out);
                break;
            // beq   
            case 0x21:
                pc = beq(ir, pc, fr, file_out);
                break;
            // blt   
            case 0x22:
                pc = blt(ir, pc, fr, file_out);
                break;
            // bgt   
            case 0x23:
                pc = bgt(ir, pc, fr, file_out);
                break;
            // bne   
            case 0x24:
                pc = bne(ir, pc, fr, file_out);
                break;
            // ble   
            case 0x25:
                pc = ble(ir, pc, fr, file_out);
                break;
            // bge   
            case 0x26:
               pc = bge(ir, pc, fr, file_out);
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

unsigned int addi(unsigned int ir, unsigned int pc, unsigned int *reg, unsigned int *fr, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (ir & 0x1F);
    x = (ir & 0x3E0) >> 5;
    imd = (ir & 0x3FFFC00) >> 10;

    unsigned long long int int_64 = reg[y] + imd;

    char tmp_ = (int_64 & 0xFFFFFFFF00000000) >> 32;
    reg[x] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (*fr & 0x10) >> 4;

    if(tmp_ == 1 && ov == 0)
        *fr = (*fr | 0x10);
    else if (tmp_ == 1 && ov == 1)
        *fr = (*fr | 0x1F);
    else if (tmp_ == 0 && ov == 0)
        *fr = (*fr | 0);
    else if (tmp_ == 0 && ov == 1)
        *fr = (*fr & 0x0F);

    sprintf(instruction, "addi r%d,r%d,%d", x, y, imd);

    printf("[0x%08X]\t%-20s\tFR=0x%08X,R%d=R%d+0x%04X=0x%08X\n", pc * 4, instruction, fr, x, y, imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,R%d=R%d+0x%04X=0x%08X\n", pc * 4, instruction, fr, x, y, imd, reg[x]);

    pc++;
    return pc;
}

unsigned int cmp(unsigned int ir, unsigned int pc, unsigned int *reg, unsigned int *fr, FILE *file)
{
    unsigned int x, y, cmp, tmp, tmp_x, tmp_y;
    char instruction[20];

    x = (ir & 0x3E0) >> 5;
    y = (ir & 0x1F);
    
    tmp = (ir & 0x38000) >> 15;
    tmp_x = (tmp & 0x01);
    tmp_y = (tmp & 0x02);
    
    if (tmp_x == 1) 
        x = (x >> 1) | (tmp_x << 5);
    else if ( tmp_y == 1)
        y = (y >> 1) | (tmp_y << 5);
    
    
    cmp = (*fr & 0xFFFFFFF8);

    if (reg[x] == reg[y])
        cmp = (cmp | 0x00000001);
    else if (reg[x] < reg[y])
        cmp = (cmp | 0x00000002);
    else
        cmp = (cmp | 0x00000004);
    
    *fr = cmp;

    sprintf(instruction, "cmp r%d,r%d", x, y);
    printf("[0x%08X]\t%-20s\tFR=0x%08X\n", pc*4, instruction, *fr);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X\n", pc*4, instruction, *fr);
    
    pc++;
    return pc;
}

unsigned int cmpi(unsigned int ir, unsigned int pc, unsigned int *reg, unsigned int *fr, FILE *file)
{
    unsigned int x, imd, cmp;
    x = (ir & 0x3E0) >> 5;
    imd = (ir & 0x3FFFC00) >> 10;
    cmp = (*fr & 0xFFFFFFF8);

    if (reg[x] == imd)
        cmp = (cmp | 0x00000001);
    else if (reg[x] < imd)
        cmp = (cmp | 0x00000002);
    else
        cmp = (cmp | 0x00000004);
    
    *fr = cmp;
    
    pc++;
    return pc;
}

unsigned int ldw(unsigned int ir, unsigned int pc, unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (ir & 0x1F);
    x = (ir & 0x3E0) >> 5;
    imd = (ir & 0x3FFFC00) >> 10;

    sprintf(instruction, "ldw r%d,r%d,0x%04X", x, y, imd);

    reg[x] = mem[reg[y] + imd];
    
    printf("[0x%08X]\t%-20s\tR%d=MEM[(R%d+0x%04X)<<2]=0x%08X\n", pc * 4, instruction, x, y, imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\tR%d=MEM[(R%d+0x%04X)<<2]=0x%08X\n", pc * 4, instruction, x, y, imd, reg[x]);
    pc++;
    return pc;
}

unsigned int stw(unsigned int ir, unsigned int pc, unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (ir & 0x1F);
    x = (ir & 0x3E0) >> 5;
    imd = (ir & 0x3FFFC00) >> 10;

    sprintf(instruction, "stw r%d,0x%04X,r%d", x, imd, y);

    mem[reg[x] + imd] = reg[y];
    
    printf("[0x%08X]\t%-20s\tMEM[(R%d+0x%04X)<<2]=R%d=0x%08X\n", pc * 4, instruction, x, imd, y, mem[reg[x] + imd]);
    fprintf(file, "[0x%08X]\t%-20s\tMEM[(R%d+0x%04X)<<2]=R%d=0x%08X\n", pc * 4, instruction, x, imd, y, mem[reg[x] + imd]);
    pc++;
    return pc;
}

unsigned int ldb(unsigned int ir, unsigned int pc, unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (ir & 0x1F);
    x = (ir & 0x3E0) >> 5;
    imd = (ir & 0x3FFFC00) >> 10;

    sprintf(instruction, "ldb r%d,r%d,0x%04X", x, y, imd);

    reg[x] = mem[reg[y] + imd];
    
    printf("[0x%08X]\t%-20s\tR%d=MEM[(R%d+0x%04X)<<2]=0x%02X\n", pc * 4, instruction, x, y, imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\tR%d=MEM[(R%d+0x%04X)<<2]=0x%02X\n", pc * 4, instruction, x, y, imd, reg[x]);
    pc++;
    return pc;
}

unsigned int bgt(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file)
{
    unsigned int old = pc, cmp;
    char instruction[20];
    
     cmp = (fr & 0x07);
     sprintf(instruction, "bgt 0x%08X", (ir & 0x3FFFFFF));

     if(cmp == 4)
        pc = (ir & 0x3FFFFFF);
    else
        pc++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);

    return pc;
}

unsigned int beq(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file)
{
    unsigned int old = pc, cmp;
    char instruction[20];
    
     cmp = (fr & 0x07);
     sprintf(instruction, "beq 0x%08X", (ir & 0x3FFFFFF));

     if(cmp == 1)
        pc = (ir & 0x3FFFFFF);
    else
        pc++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);

    return pc;
}

unsigned int blt(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file)
{
    unsigned int old = pc, cmp;
    char instruction[20];
    
     cmp = (fr & 0x07);
     sprintf(instruction, "blt 0x%08X", (ir & 0x3FFFFFF));

     if(cmp == 2)
        pc = (ir & 0x3FFFFFF);
    else
        pc++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);

    return pc;
}

unsigned int bne(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file)
{
    unsigned int old = pc, cmp;
    char instruction[20];
    
     cmp = (fr & 0x07);
     sprintf(instruction, "bne 0x%08X", (ir & 0x3FFFFFF));

     if(cmp != 1)
        pc = (ir & 0x3FFFFFF);
    else
        pc++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);

    return pc;
}

unsigned int ble(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file)
{
    unsigned int old = pc, cmp;
    char instruction[20];
    
     cmp = (fr & 0x07);
     sprintf(instruction, "ble 0x%08X", (ir & 0x3FFFFFF));

     if(cmp == 1 || cmp == 2)
        pc = (ir & 0x3FFFFFF);
    else
        pc++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);

    return pc;
}

unsigned int bge(unsigned int ir, unsigned int pc, unsigned int fr, FILE *file)
{
    unsigned int old = pc, cmp;
    char instruction[20];
    
     cmp = (fr & 0x07);
     sprintf(instruction, "bge 0x%08X", (ir & 0x3FFFFFF));

     if(cmp == 1 || cmp == 4)
        pc = (ir & 0x3FFFFFF);
    else
        pc++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);

    return pc;
}

unsigned int bun(unsigned int ir, unsigned int pc, FILE *file)
{
    unsigned int old = pc;
    pc = (ir & 0x3FFFFFF);
    char instruction[20];
    sprintf(instruction, "bun 0x%08X", pc);
    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, pc * 4);
    return pc;
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
    while ((c = getc(file)) != EOF && !endOfLine(file, c) && dst < end)
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