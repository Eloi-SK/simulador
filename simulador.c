#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int getNumberLines(FILE *file);
int getLine(FILE *file, char *buffer, size_t length);
static int endOfLine(FILE *ifp, int c);
char * indexToName(int index, int upperCase);

void add(unsigned int *reg, FILE *file);                        // Implemented
void sub(unsigned int *reg, FILE *file);                        // Implemented
void mul(unsigned int *reg, FILE *file);                        // Implemented
void _div(unsigned int *reg, FILE *file);                       // Implemented
void cmp(unsigned int *reg, FILE *file);                        // Implemented
void shl(unsigned int *reg, FILE *file);                        // Implemented
void shr(unsigned int *reg, FILE *file);                        // Implemented
void and(unsigned int *reg, FILE *file);                        // Implemented
void not(unsigned int *reg, FILE *file);                        // Implemented
void or(unsigned int *reg, FILE *file);                         // Implemented
void xor(unsigned int *reg, FILE *file);                        // Implemented
void addi(unsigned int *reg, FILE *file);                       // Implemented
void subi(unsigned int *reg, FILE *file);                       // Implemented
void muli(unsigned int *reg, FILE *file);                       // Implemented
void divi(unsigned int *reg, FILE *file);                       // Implemented
void cmpi(unsigned int *reg, FILE *file);                       // Implemented
void andi(unsigned int *reg, FILE *file);                       // Implemented
void noti(unsigned int *reg, FILE *file);                       // Implemented
void ori(unsigned int *reg, FILE *file);                        // Implemented
void xori(unsigned int *reg, FILE *file);                       // Implemented
void ldw(unsigned int *mem, unsigned int *reg, FILE *file);     // Implemented
void stw(unsigned int *mem, unsigned int *reg, FILE *file);     // Implemented
void ldb(unsigned int *mem, unsigned int *reg, FILE *file);     // Implemented
void stb(unsigned int *mem, unsigned int *reg, FILE *file);     // Implemented
void push(unsigned int *mem, unsigned int *reg, FILE *file);    // Not Implemented
void pop(unsigned int *mem, unsigned int *reg, FILE *file);     // Not Implemented
void bun(unsigned int *reg, FILE *file);                        // Implemented
void bgt(unsigned int *reg, FILE *file);                        // Implemented
void beq(unsigned int *reg, FILE *file);                        // Implemented
void blt(unsigned int *reg, FILE *file);                        // Implemented
void bne(unsigned int *reg, FILE *file);                        // Implemented
void ble(unsigned int *reg, FILE *file);                        // Implemented
void bge(unsigned int *reg, FILE *file);                        // Implemented
void call(unsigned int *reg, FILE *file);                       // Implemented
void ret(unsigned int *reg, FILE *file);                        // Implemented
void _int(unsigned int *reg, FILE *file);                       // Implemented
void invalid(unsigned int pc, FILE *file);                      // Implemented

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: <filename>.hex <filename>.out");
        return -1;
    }

    FILE *file_in = fopen(argv[1], "r");
    FILE *file_out = fopen(argv[2], "w");

    if (file_in == NULL)
    {
        printf("Error loading file.");
        return -1;
    }

    int lines = getNumberLines(file_in) + 1;
    unsigned int *memory = (unsigned int *) malloc(sizeof(unsigned int) * lines);
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

    unsigned int opcode;
    unsigned int reg[64];
    for (i = 0; i < 64; i++) reg[i] = 0;
    int exit_ = 0;

    while(exit_ == 0)
    {
        reg[33] = memory[reg[32]];
        opcode = (reg[33] & 0xFC000000) >> 26;

        switch(opcode)
        {
            // nop/add
            case 0x00:
                add(reg, file_out);
                break;
            // sub
            case 0x01:
                sub(reg, file_out);
                break;
            // mul
            case 0x02:
                mul(reg, file_out);
                break;
            // div
            case 0x03:
                _div(reg, file_out);
                break;
            // cmp
            case 0x04:
                cmp(reg, file_out);
                break;
            // shl
            case 0x05:
                shl(reg, file_out);
                break;
            // shr
            case 0x06:
                shr(reg, file_out);
                break;
            // and
            case 0x07:
                and(reg, file_out);
                break;
            // not
            case 0x08:
                not(reg, file_out);
                break;
            // or
            case 0x09:
                or(reg, file_out);
                break;
            // xor
            case 0x0A:
                xor(reg, file_out);
                break;
            // push
            case 0x0B:
                push(memory, reg, file_out);
                break;
            // pop
            case 0x0C:
                push(memory, reg, file_out);
                break;
            // addi
            case 0x10:
                addi(reg, file_out);
                break;
            // subi
            case 0x11:
                subi(reg, file_out);
                exit_ = 1;
                break;
            // muli
            case 0x12:
                muli(reg, file_out);
                break;
            // divi
            case 0x13:
                divi(reg, file_out);
                break;
            // cmpi
            case 0x14:
                cmpi(reg, file_out);
                break;
            // andi
            case 0x15:
                andi(reg, file_out);
                break;
            // noti
            case 0x16:
                noti(reg, file_out);
                break;
            // ori
            case 0x17:
                ori(reg, file_out);
                break;
            // xori
            case 0x18:
                xori(reg, file_out);
                break;
            // ldw   
            case 0x19:
                ldw(memory, reg, file_out);
                break;
            // stw   
            case 0x1A:
                stw(memory, reg, file_out);
                break;
            // ldb   
            case 0x1B:
                ldb(memory, reg, file_out);
                break;
            // stb   
            case 0x1C:
                stb(memory, reg, file_out);
                break;
            // bun
            case 0x20:
                bun(reg, file_out);
                break;
            // beq   
            case 0x21:
                beq(reg, file_out);
                break;
            // blt   
            case 0x22:
                blt(reg, file_out);
                break;
            // bgt   
            case 0x23:
                bgt(reg, file_out);
                break;
            // bne   
            case 0x24:
                bne(reg, file_out);
                break;
            // ble   
            case 0x25:
                ble(reg, file_out);
                break;
            // bge   
            case 0x26:
                bge(reg, file_out);
                break;
            // call   
            case 0x30:
                call(reg, file_out);
                break;
            // ret   
            case 0x31:
                ret(reg, file_out);
                break;
            // int   
            case 0x3F:
                _int(reg, file_out);
                exit_ = 1;
                break;
            default:
                invalid(reg[32], file_out);
                break;
        }
        
    }

    printf("[END OF SIMULATION]\n");
    fprintf(file_out, "[END OF SIMULATION]");
    fclose(file_in);
    fclose(file_out);
    free(memory);

    return 0;
}

void add(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);

    unsigned long long int reg_x_64 =  (unsigned long long int) reg[x];
    unsigned long long int reg_y_64 = (unsigned long long int) reg[y];

    unsigned long long int int_64 = reg_x_64 + reg_y_64;

    char tmp_ = (int_64 & 0xFFFFFFFF00000000) >> 32;
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (reg[35] & 0x10) >> 4;
    if(tmp_ == 1 && ov == 0)
        reg[35] |= 0x10;
    else if (tmp_ == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "add %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));

    printf("[0x%08X]\t%-20s\tFR=0x%08X,%s=%s+%s=0x%08X\n", reg[32] * 4, instruction, reg[35], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,%s=%s+%s=0x%08X\n", reg[32] * 4, instruction, reg[35], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;

}

void sub(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);

    unsigned long long int reg_x_64 =  (unsigned long long int) reg[x];
    unsigned long long int reg_y_64 = (unsigned long long int) reg[y];

    unsigned long long int int_64 = reg_x_64 - reg_y_64;

    char tmp_ = (int_64 & 0xFFFFFFFF00000000) >> 32;
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (reg[35] & 0x10) >> 4;
    if(tmp_ == 1 && ov == 0)
        reg[35] |= 0x10;
    else if (tmp_ == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "sub %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));

    printf("[0x%08X]\t%-20s\tFR=0x%08X,%s=%s-%s=0x%08X\n", reg[32] * 4, instruction, reg[35], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,%s=%s-%s=0x%08X\n", reg[32] * 4, instruction, reg[35], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void mul(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);

    unsigned long long int reg_x_64 =  (unsigned long long int) reg[x];
    unsigned long long int reg_y_64 = (unsigned long long int) reg[y];

    unsigned long long int int_64 = reg_x_64 * reg_y_64;

    reg[34] = (int_64 & 0xFFFFFFFF00000000) >> 32;
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (reg[35] & 0x10) >> 4;

    if(reg[34] > 0 && ov == 0)
        reg[35] |= 0x10;
    else if (reg[34] == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "mul %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\tFR=0x%08X,ER=0x%08X,%s=%s*%s=0x%08X\n", reg[32] * 4, instruction, reg[35], reg[34], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,ER=0x%08X,%s=%s*%s=0x%08X\n", reg[32] * 4, instruction, reg[35], reg[34], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void _div(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);

    unsigned int zd = (reg[35] & 0x08) >> 3;
    unsigned int ov = (reg[35] & 0x10) >> 4;
    unsigned long long int rx_64 = (unsigned long long int) reg[x];
    unsigned long long int ry_64 = (unsigned long long int) reg[y];
    unsigned long long int rz_64;

    if (reg[y] != 0)
    {
        if (z == 0)
            rz_64 = 0;
        else
            rz_64 = rx_64 / ry_64;
        reg[34] = reg[x] % reg[y];

        char tmp_ = (rz_64 & 0xFFFFFFFF00000000) >> 32;
        reg[z] = (rz_64 & 0xFFFFFFFF);

        if(tmp_ == 1 && ov == 0)
            reg[35] |= 0x10;
        else if (tmp_ == 0 && ov == 1)
            reg[35] &= 0x0F;
    }

    if (reg[y] == 0 && zd == 0)
        reg[35] |= 0x08;
    else if (reg[y] != 0 && zd == 1)
        reg[35] &= 0x07;


    sprintf(instruction, "div %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=0x%08X,%s=%s/%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=0x%08X,%s=%s/%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void cmp(unsigned int *reg, FILE *file)
{
    unsigned int x, y, cmp, tmp, tmp_x, tmp_y;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    y = (reg[33] & 0x1F);
    
    tmp = (reg[33] & 0x38000) >> 15;
    tmp_x = (tmp & 0x02) >> 1;
    tmp_y = (tmp & 0x01);
    
    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if ( tmp_y == 1)
        y |= (tmp_y << 5);
    
    
    cmp = (reg[35] & 0xFFFFFFF8);

    if (reg[x] == reg[y])
        cmp = (cmp | 0x00000001);
    else if (reg[x] < reg[y])
        cmp = (cmp | 0x00000002);
    else
        cmp = (cmp | 0x00000004);
    
    reg[35] = cmp;

    sprintf(instruction, "cmp %s,%s", indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35]);

    reg[32]++;
}

void shl(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);

    unsigned long long int shl_z_64, shl_x_64;

    unsigned long long int er_64 = (unsigned long long int) reg[34];
    unsigned long long int x_64 = (unsigned long long int) reg[x];

    shl_x_64 = (er_64 << 32) | x_64;
    shl_z_64 = (shl_x_64 << (y + 1));

    reg[34] = (shl_z_64 & 0xFFFFFFFF00000000) >> 32;
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = (shl_z_64 & 0xFFFFFFFF);

    sprintf(instruction, "shl %s,%s,%d", indexToName(z, 0), indexToName(x, 0), y);
    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=%s<<%u=0x%08X\n",reg[32] * 4, instruction, indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), y+1, reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=%s<<%d=0x%08X\n",reg[32] * 4, instruction, indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), y+1, reg[z]);

    reg[32]++;

}

void shr(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);

    unsigned long long int shr_z_64, shr_x_64;

    unsigned long long int er_64 = (unsigned long long int) reg[34];
    unsigned long long int x_64 = (unsigned long long int) reg[x];

    shr_x_64 = (er_64 << 32) | x_64;
    shr_z_64 = (shr_x_64 >> (y + 1));

    reg[34] = (shr_z_64 & 0xFFFFFFFF00000000) >> 32;
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = (shr_z_64 & 0xFFFFFFFF);

    sprintf(instruction, "shr %s,%s,%d", indexToName(z, 0), indexToName(x, 0), y);
    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=%s>>%u=0x%08X\n",reg[32] * 4, instruction, indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), y+1, reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=%s>>%d=0x%08X\n",reg[32] * 4, instruction, indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), y+1, reg[z]);

    reg[32]++;

}

void and(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);
    
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = reg[x] & reg[y];

    sprintf(instruction, "and %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=%s&%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s&%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void not(unsigned int *reg, FILE *file)
{
    unsigned int x, y, tmp, tmp_x, tmp_y;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);

    reg[x] = ~reg[y];

    sprintf(instruction, "not %s,%s", indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=~%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=~%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), reg[x]);

    reg[32]++;
}

void or(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);

     if (z == 0)
        reg[z] = 0;
    else
        reg[z] = reg[x] | reg[y];

    sprintf(instruction, "or %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=%s|%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s|%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void xor(unsigned int *reg, FILE *file)
{
    unsigned int x, y, z, tmp, tmp_x, tmp_y, tmp_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;
    tmp_z = (tmp & 0x04) >> 2;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);
    if (tmp_z == 1)
        z |= (tmp_z << 5);
    
     if (z == 0)
        reg[z] = 0;
    else
        reg[z] = reg[x] ^ reg[y];

    sprintf(instruction, "xor %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=%s^%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s^%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void push(unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int x, y, tmp, tmp_x, tmp_y;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);

    mem[reg[x]] = reg[y];

    unsigned long long int reg_x_64 =  (unsigned long long int) reg[x];
    unsigned long long int imd_64 = (unsigned long long int) 1;

    unsigned long long int int_64 = reg_x_64 - imd_64;

    char tmp_ = (int_64 & 0xFFFFFFFF00000000) >> 32;
     if (x == 0)
        reg[x] = 0;
    else
        reg[x] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (reg[35] & 0x10) >> 4;

    if(tmp_ == 1 && ov == 0)
        reg[35] = (reg[35] | 0x10);
    else if (tmp_ == 0 && ov == 1)
        reg[35] = (reg[35] & 0x0F);

    sprintf(instruction, "push %s,%s", indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\tMEM[%s->0x%08X]=%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), x, indexToName(y, 1), y);
    fprintf(file, "[0x%08X]\t%-20s\tMEM[%s->0x%08X]=%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), x, indexToName(y, 1), y);

    reg[32]++;
}

void pop(unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int x, y, tmp, tmp_x, tmp_y;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;

    tmp = (reg[33] & 0x38000) >> 15;
    tmp_y = (tmp & 0x01);
    tmp_x = (tmp & 0x02) >> 1;

    if (tmp_x == 1) 
        x |= (tmp_x << 5);
    if (tmp_y == 1)
        y |= (tmp_y << 5);

    unsigned long long int reg_y_64 =  (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) 1;

    unsigned long long int int_64 = reg_y_64 + imd_64;

    char tmp_ = (int_64 & 0xFFFFFFFF00000000) >> 32;
     if (y == 0)
        reg[y] = 0;
    else
        reg[y] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (reg[35] & 0x10) >> 4;

    if(tmp_ == 1 && ov == 0)
        reg[35] = (reg[35] | 0x10);
    else if (tmp_ == 0 && ov == 1)
        reg[35] = (reg[35] & 0x0F);

    if(x == 0)
        reg[x] = 0;
    else
        reg[x] = mem[reg[y]];

    sprintf(instruction, "pop %s,%s", indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=MEM[%s->0x%08X]=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), y, mem[reg[y]]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=MEM[%s->0x%08X]=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), y, mem[reg[y]]);

    reg[32]++;
}

void addi(unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    unsigned long long int reg_y_64 =  (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) imd;

    unsigned long long int int_64 = reg_y_64 + imd_64;

    char tmp_ = (int_64 & 0xFFFFFFFF00000000) >> 32;
     if (x == 0)
        reg[x] = 0;
    else
        reg[x] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (reg[35] & 0x10) >> 4;

    if(tmp_ == 1 && ov == 0)
        reg[35] = (reg[35] | 0x10);
    else if (tmp_ == 0 && ov == 1)
        reg[35] = (reg[35] & 0x0F);
    
    sprintf(instruction, "addi %s,%s,%d", indexToName(x, 0), indexToName(y, 0), imd);

    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=%s+0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=%s+0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void subi(unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    unsigned long long int reg_y_64 =  (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) imd;

    unsigned long long int int_64 = reg_y_64 - imd_64;

    char tmp_ = (int_64 & 0xFFFFFFFF00000000) >> 32;
     if (x == 0)
        reg[x] = 0;
    else
        reg[x] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (reg[35] & 0x10) >> 4;

    if(tmp_ == 1 && ov == 0)
        reg[35] = (reg[35] | 0x10);
    else if (tmp_ == 0 && ov == 1)
        reg[35] = (reg[35] & 0x0F);
    
    sprintf(instruction, "subi %s,%s,%d", indexToName(x, 0), indexToName(y, 0), imd);

    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=%s+0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=%s+0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void muli(unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    unsigned long long int reg_y_64 =  (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) imd;

    unsigned long long int int_64 = reg_y_64 * imd;

    reg[34] = (int_64 & 0xFFFFFFFF00000000) >> 32;
     if (x == 0)
        reg[x] = 0;
    else
        reg[x] = (int_64 & 0xFFFFFFFF);

    unsigned int ov = (reg[35] & 0x10) >> 4;

    if(reg[34] > 0 && ov == 0)
        reg[35] |= 0x10;
    else if (reg[34] == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "muli %s,%s,%d", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\tFR=0x%08X,ER=0x%08X,%s=%s*0x%04X=0x%08X\n", reg[32] * 4, instruction, reg[35], reg[34], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,ER=0x%08X,%s=%s*0x%04X=0x%08X\n", reg[32] * 4, instruction, reg[35], reg[34], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void divi(unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    unsigned int zd = (reg[35] & 0x08) >> 3;
    unsigned int ov = (reg[35] & 0x10) >> 4;
    unsigned long long int ry_64 = (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) imd;
    unsigned long long int rx_64;

    if (imd != 0)
    {
        if (x == 0)
            rx_64 = 0;
        else
            rx_64 = ry_64 / imd;
        reg[34] = ry_64 % imd;

        char tmp_ = (rx_64 & 0xFFFFFFFF00000000) >> 32;
        reg[x] = (rx_64 & 0xFFFFFFFF);

        if(tmp_ == 1 && ov == 0)
            reg[35] |= 0x10;
        else if (tmp_ == 0 && ov == 1)
            reg[35] &= 0x0F;
    }

    if (imd == 0 && zd == 0)
        reg[35] |= 0x08;
    else if (imd != 0 && zd == 1)
        reg[35] &= 0x07;


    sprintf(instruction, "divi %s,%s,%d", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=0x%08X,%s=%s/0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(34, 1), reg[34], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=0x%08X,%s=%s/0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(34, 1), reg[34], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
        
    reg[32]++;
}

void cmpi(unsigned int *reg, FILE *file)
{
    unsigned int x, imd, cmp;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;
    cmp = (reg[35] & 0xFFFFFFF8);

    if (reg[x] == imd)
        cmp = (cmp | 0x00000001);
    else if (reg[x] < imd)
        cmp = (cmp | 0x00000002);
    else
        cmp = (cmp | 0x00000004);
    
    reg[35] = cmp;

    sprintf(instruction, "cmpi %s,%d", indexToName(x, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35]);
    
    reg[32]++;
}

void andi(unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;
    
    if (x == 0)
        reg[x] = 0;
    else
        reg[x] = reg[y] & imd;

    sprintf(instruction, "andi %s,%s,%d", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=%s&0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s&0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void noti(unsigned int *reg, FILE *file)
{
    unsigned int x, imd;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    reg[x] = ~imd;

    sprintf(instruction, "noti %s,%d", indexToName(x, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=~0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=~0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, reg[x]);

    reg[32]++;
}

void ori(unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;
    
    if (x == 0)
        reg[x] = 0;
    else
        reg[x] = reg[y] | imd;

    sprintf(instruction, "ori %s,%s,%d", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\tt%s=%s|0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s|0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void xori(unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    if (x == 0)
        reg[x] = 0;
    else
        reg[x] = reg[y] ^ imd;

    sprintf(instruction, "xori %s,%s,%d", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=%s^0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s^0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void ldw(unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    sprintf(instruction, "ldw r%d,r%d,0x%04X", x, y, imd);

    if(x == 0)
        reg[x] = 0;
    else
        reg[x] = mem[reg[y] + imd];
    
    printf("[0x%08X]\t%-20s\tR%u=MEM[(R%u+0x%04X)<<2]=0x%08X\n", reg[32] * 4, instruction, x, y, imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\tR%u=MEM[(R%u+0x%04X)<<2]=0x%08X\n", reg[32] * 4, instruction, x, y, imd, reg[x]);
    
    reg[32]++;
}

void stw(unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    sprintf(instruction, "stw %s,0x%04X,%s", indexToName(x, 0), imd, indexToName(y, 0));

    mem[reg[x] + imd] = reg[y];
    
    printf("[0x%08X]\t%-20s\tMEM[(%s+0x%04X)<<2]=%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, indexToName(y, 1), mem[reg[x] + imd]);
    fprintf(file, "[0x%08X]\t%-20s\tMEM[(%s+0x%04X)<<2]=%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, indexToName(y, 1), mem[reg[x] + imd]);
    
    reg[32]++;
}

void ldb(unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int index, byte, x, y, imd, tmp;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    index = (reg[y] + imd) / 4;
    tmp = mem[index];
    byte = (reg[y] + imd) % 4;
    
    if (x == 0)
        reg[x] == 0;
    else
    {
        switch(byte)
        {
            case 0:
                reg[x] = (tmp & 0xFF000000) >> 24;
                break;
            case 1:
                reg[x] = (tmp & 0xFF0000) >> 16;
                break;
            case 2:
                reg[x] = (tmp & 0xFF00) >> 8;
                break;
            case 3:
                reg[x] = (tmp & 0xFF);
                break;
        }
    }
    
    sprintf(instruction, "ldb %s,%s,0x%04X", indexToName(x, 0), indexToName(y, 0), imd);

    printf("[0x%08X]\t%-20s\t%s=MEM[%s+0x%04X]=0x%02X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=MEM[%s+0x%04X]=0x%02X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void stb(unsigned int *mem, unsigned int *reg, FILE *file)
{
    unsigned int index, byte, x, y, imd, tmp;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    index = (reg[x] + imd) / 4;
    tmp = mem[index];
    byte = (reg[x] + imd) % 4;
    
    switch(byte)
    {
        case 0:
            mem[index] =  (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
            break;
        case 1:
            mem[index] =  (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
            break;
        case 2:
            mem[index] =  (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
            break;
        case 3:
            mem[index] =  (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
            break;
    }

    sprintf(instruction, "stb %s,0x%04X,%s", indexToName(x, 0), imd, indexToName(y, 0));

    printf("[0x%08X]\t%-20s\tMEM[%s+0x%04X]=%s=0x%02X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, indexToName(y, 1), reg[y]);
    fprintf(file, "[0x%08X]\t%-20s\tMEM[%s+0x%04X]=%s=0x%02X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, indexToName(y, 1), reg[y]);
    
    reg[32]++;
}

void bun(unsigned int *reg, FILE *file)
{
    unsigned int old = reg[32];
    reg[32] = (reg[33] & 0x3FFFFFF);
    char instruction[20];
    sprintf(instruction, "bun 0x%08X", reg[32]);
    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    
}

void beq(unsigned int *reg, FILE *file)
{
    unsigned int old = reg[32], cmp;
    char instruction[20];
    
     cmp = (reg[35] & 0x07);
     sprintf(instruction, "beq 0x%08X", (reg[33] & 0x3FFFFFF));

     if(cmp == 1)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);

}

void blt(unsigned int *reg, FILE *file)
{
    unsigned int old = reg[32], cmp;
    char instruction[20];
    
     cmp = (reg[35] & 0x07);
     sprintf(instruction, "blt 0x%08X", (reg[33] & 0x3FFFFFF));

     if(cmp == 2)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);

}

void bgt(unsigned int *reg, FILE *file)
{
    unsigned int old = reg[32], cmp;
    char instruction[20];
    
     cmp = (reg[35] & 0x07);
     sprintf(instruction, "bgt 0x%08X", (reg[33] & 0x3FFFFFF));

     if(cmp == 4)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);

}

void bne(unsigned int *reg, FILE *file)
{
    unsigned int old = reg[32], cmp;
    char instruction[20];
    
     cmp = (reg[35] & 0x07);
     sprintf(instruction, "bne 0x%08X", (reg[33] & 0x3FFFFFF));

     if(cmp != 1)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);

}

void ble(unsigned int *reg, FILE *file)
{
    unsigned int old = reg[32], cmp;
    char instruction[20];
    
     cmp = (reg[35] & 0x07);
     sprintf(instruction, "ble 0x%08X", (reg[33] & 0x3FFFFFF));

     if(cmp == 1 || cmp == 2)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);

}

void bge(unsigned int *reg, FILE *file)
{
    unsigned int old = reg[32], cmp;
    char instruction[20];
    
     cmp = (reg[35] & 0x07);
     sprintf(instruction, "bge 0x%08X", (reg[33] & 0x3FFFFFF));

     if(cmp == 1 || cmp == 4)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);

}

void call(unsigned int *reg, FILE *file)
{
    unsigned int x, y, imd, old;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;
    old = reg[32];

    if (x == 0)
        reg[x] = 0;
    else
        reg[x] = reg[32] + 1;

    reg[32] = reg[y] + imd;

    sprintf(instruction, "call %s,%s,0x%04X", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=(PC+4)>>2=0x%08X,PC=(%s+0x%04X)<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[x], indexToName(y, 1), imd, reg[32]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=(PC+4)>>2=0x%08X,PC=(%s+0x%04X)<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[x], indexToName(y, 1), imd, reg[32]);
}

void ret(unsigned int *reg, FILE *file)
{
    unsigned int x, old;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    old = reg[32];

    reg[32] = reg[x];

    sprintf(instruction, "ret %s", indexToName(x, 0));
    printf("[0x%08X]\t%-20s\tPC=%s<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[32]);
    fprintf(file, "[0x%08X]\t%-20s\tPC=%s<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[32]);
}

void _int(unsigned int *reg, FILE *file)
{
    unsigned int imd, old = reg[32];
    char instruction[20];    

    imd = (reg[33] & 0x3FFFFFF);
    reg[32] = 0;
    
    sprintf(instruction, "int %d", imd);
    printf("[0x%08X]\t%-20s\tCR=0x00000000,PC=0x%08X\n", old * 4, instruction, reg[32]);
    fprintf(file, "[0x%08X]\t%-20s\tCR=0x00000000,PC=0x%08X\n", old * 4, instruction, reg[32]);

}

void invalid(unsigned int pc, FILE *file)
{
    printf("[INVALID INSTRUCTION @ 0x%08X]\n", pc);
    fprintf(file, "[INVALID INSTRUCTION @ 0x%08X]\n", pc);
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

char * indexToName(int index, int upperCase)
{
    char *str = (char *) malloc(sizeof(char) * 4);

    if (index < 32)
    {
        sprintf(str, "r%d", index);
    }
    else
    {
        switch (index)
        {
            case 32:
                sprintf(str, "pc");
                break;
            case 33:
                sprintf(str, "ir");
                break;
            case 34:
                sprintf(str, "er");
                break;
            case 35:
                sprintf(str, "fr");
                break;
            default:
                break;
        }
    }

    if (upperCase == 1)
    {
        for(int i = 0; str[i] != '\0'; i++){
            str[i] = toupper(str[i]);
        }
    }
    
    return str;
}