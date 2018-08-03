#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

typedef struct node
{
    char character;
    struct node *next;
} Node;

typedef struct list
{
    int size;
    Node* head;
} List;

typedef struct set
{
    uint32_t age;
     uint32_t block[4];
    uint32_t id;
    uint32_t valid;
} Set;

typedef struct cache
{
    Set set[2];
} Cache;

int getNumberLines(FILE *file);
char * indexToName(uint32_t index, int upperCase);
char * validToString(uint32_t valid);
char * blockToString(uint32_t *block);
List* createList();
void insert(List *list, char c);

void add(uint32_t *reg, FILE *file);                                // Implemented
void sub(uint32_t *reg, FILE *file);                                // Implemented
void mul(uint32_t *reg, FILE *file);                                // Implemented
void _div(uint32_t *reg, FILE *file);                               // Implemented
void cmp(uint32_t *reg, FILE *file);                                // Implemented
void shl(uint32_t *reg, FILE *file);                                // Implemented
void shr(uint32_t *reg, FILE *file);                                // Implemented
void and(uint32_t *reg, FILE *file);                                // Implemented
void not(uint32_t *reg, FILE *file);                                // Implemented
void or(uint32_t *reg, FILE *file);                                 // Implemented
void xor(uint32_t *reg, FILE *file);                                // Implemented
void addi(uint32_t *reg, FILE *file);                               // Implemented
void subi(uint32_t *reg, FILE *file);                               // Implemented
void muli(uint32_t *reg, FILE *file);                               // Implemented
void divi(uint32_t *reg, FILE *file);                               // Implemented
void cmpi(uint32_t *reg, FILE *file);                               // Implemented
void andi(uint32_t *reg, FILE *file);                               // Implemented
void noti(uint32_t *reg, FILE *file);                               // Implemented
void ori(uint32_t *reg, FILE *file);                                // Implemented
void xori(uint32_t *reg, FILE *file);                               // Implemented
void ldw(uint32_t *mem, uint32_t *reg, FILE *file);                 // Implemented
void stw(uint32_t *mem, uint32_t *reg, FILE *file);                 // Implemented
void ldb(uint32_t *mem, uint32_t *reg, FILE *file);                 // Implemented
void stb(uint32_t *mem, uint32_t *reg, FILE *file, List *terminal); // Implemented
void push(uint32_t *mem, uint32_t *reg, FILE *file);                // Implemented
void pop(uint32_t *mem, uint32_t *reg, FILE *file);                 // Implemented
void bun(uint32_t *reg, FILE *file);                                // Implemented
void bgt(uint32_t *reg, FILE *file);                                // Implemented
void beq(uint32_t *reg, FILE *file);                                // Implemented
void blt(uint32_t *reg, FILE *file);                                // Implemented
void bne(uint32_t *reg, FILE *file);                                // Implemented
void ble(uint32_t *reg, FILE *file);                                // Implemented
void bge(uint32_t *reg, FILE *file);                                // Implemented
void bzd(uint32_t *reg, FILE *file);                                // Implemented
void bnz(uint32_t *reg, FILE *file);                                // Implemented
void biv(uint32_t *reg, FILE *file);                                // Implemented
void bni(uint32_t *reg, FILE *file);                                // Implemented
void call(uint32_t *reg, FILE *file);                               // Implemented
void ret(uint32_t *reg, FILE *file);                                // Implemented
void isr(uint32_t *reg, FILE *file);                                // Implemented
void reti(uint32_t *reg, FILE *file);                               // Implemented
void _int(uint32_t *reg, FILE *file);                               // Implemented
void invalid(uint32_t *reg, FILE *file);                            // Implemented
void watchdog(uint32_t *reg, FILE *file);                           // Implemented
void fpu(uint32_t *reg, FILE *file);                                // Implemented
void fpu_add(float fx, float fy, float fz);                         // Implemented
void fpu_sub(float fx, float fy, float fz);                         // Implemented
void fpu_mul(float fx, float fy, float fz);                         // Implemented
void fpu_div(float fx, float fy, float fz);                         // Implemented
void fpu_assign_x(float fz);                                        // Implemented
void fpu_assign_y(float fz);                                        // Implemented
void fpu_ceil(float fz);                                            // Implemented
void fpu_floor(float fz);                                           // Implemented
void fpu_round(float fz);                                           // Implemented
void imprime(FILE *file, List *LISTA);                              // Implemented
uint32_t getInstruction(uint32_t  index, uint32_t *mem, uint32_t length, FILE *file);
void cacheStats(FILE *file);

uint32_t fpu_x, fpu_y, fpu_z, fpu_control, fpu_int, fpu_fez_op;
uint32_t wdg, terminal_in, terminal_out;
uint32_t int_ctrl = 4;
uint32_t fpux_is_ieee = 0;
uint32_t fpuy_is_ieee = 0;
uint32_t fpuz_is_ieee = 0;
uint32_t fpu_counter = 0xFFFFFFFF;
uint32_t cache_i_miss_counter = 0;
uint32_t cache_i_hit_counter = 0;
uint32_t cache_d_miss_counter = 0;
uint32_t cache_d_hit_counter = 0;

Cache cache_d[8];
Cache cache_i[8];

int main(int argc, char *argv[])
{
    List* terminalOut = createList();
    if (argc < 3)
    {
        printf("Usage: <filename>.hex <filename>.out\n");
        return -1;
    }

    FILE *file_in = fopen(argv[1], "r");
    FILE *file_out = fopen(argv[2], "w");

    if (file_in == NULL)
    {
        printf("Error loading file.\n");
        return -1;
    }

    uint32_t lines = getNumberLines(file_in) + 1;
    uint32_t *memory = (uint32_t *) malloc(sizeof(uint32_t) * lines);
    uint32_t line;
    int i = 0;
    fseek(file_in, 0, SEEK_SET);

    while (!feof(file_in))
    {
        fscanf(file_in, "%X", &line);
        memory[i] = line;
        i++;
    }

    printf("[START OF SIMULATION]\n");
    fprintf(file_out, "[START OF SIMULATION]\n");

    uint32_t opcode;
    uint32_t reg[64];
    for (i = 0; i < 64; i++) reg[i] = 0;
    int exit_ = 0;

    int_ctrl = 0;

    while(exit_ == 0)
    {
        reg[33] = getInstruction(reg[32], memory, lines, file_out);//memory[reg[32]];
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
                pop(memory, reg, file_out);
                break;
            // addi
            case 0x10:
                addi(reg, file_out);
                break;
            // subi
            case 0x11:
                subi(reg, file_out);
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
                stb(memory, reg, file_out, terminalOut);
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
            // bzd
            case 0x27:
                bzd(reg, file_out);
                break;
            // bnz
            case 0x28:
                bnz(reg, file_out);
                break;
            // biv
            case 0x29:
                biv(reg, file_out);
                break;
            // bni
            case 0x2A:
                bni(reg, file_out);
                break;
            // call
            case 0x30:
                call(reg, file_out);
                break;
            // ret
            case 0x31:
                ret(reg, file_out);
                break;
            // isr
            case 0x32:
                isr(reg, file_out);
                break;
            // reti
            case 0x33:
                reti(reg, file_out);
                break;
            // int
            case 0x3F:
                _int(reg, file_out);
                if (reg[32] == 0) exit_ = 1;
                break;
            default:
                invalid(reg, file_out);
                break;
        }

        int is_enable = (wdg >> 31);
        if (is_enable == 1)
            watchdog(reg, file_out);
        
        fpu(reg, file_out);

    }
    
    imprime(file_out, terminalOut);
    printf("[END OF SIMULATION]\n");
    fprintf(file_out, "[END OF SIMULATION]\n");
    cacheStats(file_out);
    fclose(file_in);
    fclose(file_out);
    free(memory);

    return 0;
}

void imprime(FILE *file, List *list)
{
    if (list->size > 0)
    {
        printf("[TERMINAL]\n");
        fprintf(file, "[TERMINAL]\n");
        
        Node* aux = list->head;
        while(aux->next != NULL)
        {
            printf("%c", aux->character);
            fprintf(file, "%c", aux->character);
            aux = aux->next;
        }
        
        printf("\n");
        fprintf(file, "\n");
    }
}

void watchdog(uint32_t *reg, FILE *file)
{
    uint32_t is_enable = (wdg >> 31);
    uint32_t counter = (wdg & 0x7FFFFFFF);
    if (counter == 0)
    {
        uint32_t ie = (reg[35] & 0x40) >> 6;
        if (ie == 1 && (int_ctrl == 0 || int_ctrl == 4))
        {
            is_enable = 0;
            int_ctrl |= 0x02;
            reg[37] = reg[32];
            reg[32] = 1;
            reg[36] = 0xE1AC04DA;

            printf("[HARDWARE INTERRUPTION %u]\n", reg[32]);
            fprintf(file, "[HARDWARE INTERRUPTION %u]\n", reg[32]);
        }
    }
    else
        counter --;

    wdg = counter;
    wdg |= (is_enable << 31);
}

void fpu(uint32_t *reg, FILE *file)
{
    float f_x, f_y, f_z;
    uint32_t op;

    op = fpu_control & 0x1F;

    if (fpu_counter <= 0)
    {
        fpu_control &= 0x20;
        uint32_t ie = (reg[35] & 0x40) >> 6;
        if (ie == 1 && int_ctrl == 0 && fpu_int == 0)
        {
            int_ctrl|= 0x04;
            fpu_fez_op = 0;
            fpu_int = 1;
            reg[37] = reg[32];
            reg[32] = 2;
            reg[36] = 0x01EEE754;

            printf("[HARDWARE INTERRUPTION %u]\n", reg[32]);
            fprintf(file, "[HARDWARE INTERRUPTION %u]\n", reg[32]);
        }
    }
    
    if (op != 0 && fpu_fez_op == 0)
    {
        fpu_int = 0;

        if (fpux_is_ieee == 0)
        f_x = (float) fpu_x;
        else
        {
            float *x = (float*) &fpu_x;
            f_x = *x;
        }
        if(fpuy_is_ieee == 0)
            f_y = (float) fpu_y;
        else
        {
            float *y = (float*) &fpu_y;
            f_z = *y;
        }
        if(fpuz_is_ieee == 0)
            f_z = (float) fpu_z;
        else
        {
            float *z = (float*) &fpu_z;
            f_z = *z;
        }

        switch (op)
        {
            case 0x01:
                fpu_add(f_x, f_y, f_z);
                fpu_fez_op = 1;
                break;
            case 0x02:
                fpu_sub(f_x, f_y, f_z);
                fpu_fez_op = 1;
                break;
            case 0x03:
                fpu_mul(f_x, f_y, f_z);
                fpu_fez_op = 1;
                break;
            case 0x04:
                fpu_div(f_x, f_y, f_z);
                fpu_fez_op = 1;
                break;
            case 0x05:
                fpu_assign_x(f_z);
                fpu_fez_op = 1;
                break;
            case 0x06:
                fpu_assign_y(f_z);
                fpu_fez_op = 1;
                break;
            case 0x07:
                fpu_ceil(f_z);
                fpu_fez_op = 1;
                break;
            case 0x08:
                fpu_floor(f_z);
                fpu_fez_op = 1;
                break;
            case 0x09:
                fpu_round(f_z);
                fpu_fez_op = 1;
                break;
            default:
                fpu_control = 0x20;
                fpu_counter = 1;
                break;
        }

    }
    if (fpu_counter > 0)
            fpu_counter--;
    
}

void fpu_add(float fx, float fy, float fz)
{
    uint32_t *pfx = (uint32_t *) &fx;
    uint32_t *pfy = (uint32_t *) &fy;

    uint32_t exp_x, exp_y;

    exp_x = (*pfx & 0x7F800000) >> 23;
    exp_y = (*pfy & 0x7F800000) >> 23;

    fpu_counter = (exp_x - exp_y) + 1;
    
    fz = fx + fy;
    
    uint32_t *pfz = (uint32_t *) &fz;
    fpu_z = *pfz;
    
    fpuz_is_ieee = 1;
}

void fpu_sub(float fx, float fy, float fz)
{
    uint32_t *pfx = (uint32_t *) &fx;
    uint32_t *pfy = (uint32_t *) &fy;

    uint32_t exp_x, exp_y;

    exp_x = (*pfx & 0x7F800000) >> 23;
    exp_y = (*pfy & 0x7F800000) >> 23;

    fpu_counter = (exp_x - exp_y) + 1;
    
    
    fz = fx - fy;
    
    uint32_t *pfz = (uint32_t *) &fz;
    fpu_z = *pfz;
    
    fpuz_is_ieee = 1;
}

void fpu_mul(float fx, float fy, float fz)
{
    uint32_t *pfx = (uint32_t *) &fx;
    uint32_t *pfy = (uint32_t *) &fy;

    uint32_t exp_x, exp_y;

    exp_x = (*pfx & 0x7F800000) >> 23;
    exp_y = (*pfy & 0x7F800000) >> 23;

    fpu_counter = (exp_x - exp_y) + 1;
    
    fz = fx * fy;
    
    uint32_t *pfz = (uint32_t *) &fz;
    fpu_z = *pfz;
    

    fpuz_is_ieee = 1;
}

void fpu_div(float fx, float fy, float fz)
{
    if (fy == 0)
    {
        fpu_control = 0x20;
        fpu_counter = 1;
    }
    else
    {
        uint32_t *pfx = (uint32_t *) &fx;
        uint32_t *pfy = (uint32_t *) &fy;

        uint32_t exp_x, exp_y;

        exp_x = (*pfx & 0x7F800000) >> 23;
        exp_y = (*pfy & 0x7F800000) >> 23;

        fpu_counter = (exp_x - exp_y) + 1;
        
        fz = fx / fy;
        
        uint32_t *pfz = (uint32_t *) &fz;
        fpu_z = *pfz;
        
        fpuz_is_ieee = 1;
    }
}

void fpu_assign_x(float fz)
{
    uint32_t *pfz = (uint32_t *) &fz;
    fpu_x = *pfz;
    fpu_counter = 1;
    fpux_is_ieee = 1;
}

void fpu_assign_y(float fz)
{
    uint32_t *pfz = (uint32_t *) &fz;
    fpu_y = *pfz;
    fpu_counter = 1;
    fpuy_is_ieee = 1;
}

void fpu_ceil(float fz)
{
    fpu_z = (uint32_t) ceilf(fz);
    fpu_counter = 1;
    fpuz_is_ieee = 0;
}

void fpu_floor(float fz)
{
    fpu_z = (uint32_t) floorf(fz);
    fpu_counter = 1;
    fpuz_is_ieee = 0;
}

void fpu_round(float fz)
{
    fpu_z = (uint32_t) roundf(fz);
    fpu_counter = 1;
    fpuz_is_ieee = 0;
}

void add(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

    unsigned long long int reg_x_64 =  (unsigned long long int) reg[x];
    unsigned long long int reg_y_64 = (unsigned long long int) reg[y];

    unsigned long long int int_64 = reg_x_64 + reg_y_64;

    char tmp = (int_64 & 0xFFFFFFFF00000000) >> 32;
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = (int_64 & 0xFFFFFFFF);

    uint32_t ov = (reg[35] & 0x10) >> 4;
    if(tmp == 1 && ov == 0)
        reg[35] |= 0x10;
    else if (tmp == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "add %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));

    printf("[0x%08X]\t%-20s\tFR=0x%08X,%s=%s+%s=0x%08X\n", reg[32] * 4, instruction, reg[35], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,%s=%s+%s=0x%08X\n", reg[32] * 4, instruction, reg[35], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void sub(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

    unsigned long long int reg_x_64 =  (unsigned long long int) reg[x];
    unsigned long long int reg_y_64 = (unsigned long long int) reg[y];

    unsigned long long int int_64 = reg_x_64 - reg_y_64;

    char tmp = (int_64 & 0xFFFFFFFF00000000) >> 32;
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = (int_64 & 0xFFFFFFFF);

    uint32_t ov = (reg[35] & 0x10) >> 4;
    if(tmp == 1 && ov == 0)
        reg[35] |= 0x10;
    else if (tmp == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "sub %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));

    printf("[0x%08X]\t%-20s\tFR=0x%08X,%s=%s-%s=0x%08X\n", reg[32] * 4, instruction, reg[35], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,%s=%s-%s=0x%08X\n", reg[32] * 4, instruction, reg[35], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void mul(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

    unsigned long long int reg_x_64 =  (unsigned long long int) reg[x];
    unsigned long long int reg_y_64 = (unsigned long long int) reg[y];

    unsigned long long int int_64 = reg_x_64 * reg_y_64;

    reg[34] = (int_64 & 0xFFFFFFFF00000000) >> 32;
    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = (int_64 & 0xFFFFFFFF);

    uint32_t ov = (reg[35] & 0x10) >> 4;

    if(reg[34] > 0 && ov == 0)
        reg[35] |= 0x10;
    else if (reg[34] == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "mul %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\tFR=0x%08X,ER=0x%08X,%s=%s*%s=0x%08X\n", reg[32] * 4, instruction, reg[35], reg[34], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,ER=0x%08X,%s=%s*%s=0x%08X\n", reg[32] * 4, instruction, reg[35], reg[34], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void _div(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

    uint32_t zd = (reg[35] & 0x08) >> 3;
    uint32_t ov = (reg[35] & 0x10) >> 4;
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

        char tmp = (rz_64 & 0xFFFFFFFF00000000) >> 32;
        reg[z] = (rz_64 & 0xFFFFFFFF);

        if(tmp == 1 && ov == 0)
            reg[35] |= 0x10;
        else if (tmp == 0 && ov == 1)
            reg[35] &= 0x0F;
    }

    if (reg[y] == 0 && zd == 0)
        reg[35] |= 0x08;
    else if (reg[y] != 0 && zd == 1)
        reg[35] &= 0x07;


    sprintf(instruction, "div %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=0x%08X,%s=%s/%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=0x%08X,%s=%s/%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    uint32_t ie = (reg[35] & 0x40) >> 6;
    if (reg[y] == 0 && ie == 1)
    {
        reg[37] = reg[32] + 1;
        reg[32] = 3;
        reg[36] = 1;

        printf("[SOFTWARE INTERRUPTION]\n");
        fprintf(file, "[SOFTWARE INTERRUPTION]\n");
    }
    else
        reg[32]++;
}

void cmp(uint32_t *reg, FILE *file)
{
    uint32_t x, y, cmp, ext, ext_x, ext_y;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    y = (reg[33] & 0x1F);

    ext = (reg[33] & 0x38000) >> 15;
    ext_x = (ext & 0x02) >> 1;
    ext_y = (ext & 0x01);

    if (ext_x == 1)
        x |= (ext_x << 5);
    if ( ext_y == 1)
        y |= (ext_y << 5);


    cmp = (reg[35] & 0xFFFFFFF8);

    if (reg[x] == reg[y])
        cmp |= 0x00000001;
    else if (reg[x] < reg[y])
        cmp |= 0x00000002;
    else
        cmp |= 0x00000004;

    reg[35] = cmp;

    sprintf(instruction, "cmp %s,%s", indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35]);

    reg[32]++;
}

void shl(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

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

void shr(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

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

    sprintf(instruction, "shr %s,%s,%u", indexToName(z, 0), indexToName(x, 0), y);
    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=%s>>%u=0x%08X\n",reg[32] * 4, instruction, indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), y+1, reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=%s>>%u=0x%08X\n",reg[32] * 4, instruction, indexToName(34, 1), reg[34], indexToName(z, 1), indexToName(x, 1), y+1, reg[z]);

    reg[32]++;
}

void and(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

    if (z == 0)
        reg[z] = 0;
    else
        reg[z] = reg[x] & reg[y];

    sprintf(instruction, "and %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=%s&%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s&%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void not(uint32_t *reg, FILE *file)
{
    uint32_t x, y, ext, ext_x, ext_y;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);

    reg[x] = ~reg[y];

    sprintf(instruction, "not %s,%s", indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=~%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=~%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), reg[x]);

    reg[32]++;
}

void or(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

     if (z == 0)
        reg[z] = 0;
    else
        reg[z] = reg[x] | reg[y];

    sprintf(instruction, "or %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=%s|%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s|%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void xor(uint32_t *reg, FILE *file)
{
    uint32_t x, y, z, ext, ext_x, ext_y, ext_z;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    z = (reg[33] & 0x7C00) >> 10;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;
    ext_z = (ext & 0x04) >> 2;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);
    if (ext_z == 1)
        z |= (ext_z << 5);

     if (z == 0)
        reg[z] = 0;
    else
        reg[z] = reg[x] ^ reg[y];

    sprintf(instruction, "xor %s,%s,%s", indexToName(z, 0), indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=%s^%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s^%s=0x%08X\n", reg[32] * 4, instruction, indexToName(z, 1), indexToName(x, 1), indexToName(y, 1), reg[z]);

    reg[32]++;
}

void push(uint32_t *mem, uint32_t *reg, FILE *file)
{
    uint32_t x, y, ext, ext_x, ext_y;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);

    mem[reg[x]] = reg[y]; // stw rx, ry

    if (x == 0)
        reg[x] = 0;
    else
        reg[x]--;


    sprintf(instruction, "push %s,%s", indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\tMEM[%s->0x%08X]=%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), (reg[x] + 1) * 4, indexToName(y, 1), mem[reg[x] + 1]);
    fprintf(file, "[0x%08X]\t%-20s\tMEM[%s->0x%08X]=%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), (reg[x] + 1) * 4, indexToName(y, 1), mem[reg[x] + 1]);

    reg[32]++;
}

void pop(uint32_t *mem, uint32_t *reg, FILE *file)
{
    uint32_t x, y, ext, ext_x, ext_y;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;

    ext = (reg[33] & 0x38000) >> 15;
    ext_y = (ext & 0x01);
    ext_x = (ext & 0x02) >> 1;

    if (ext_x == 1)
        x |= (ext_x << 5);
    if (ext_y == 1)
        y |= (ext_y << 5);

    if (y == 0)
        reg[y] = 0;
    else
        reg[y]++;

    if(x == 0)
        reg[x] = 0;
    else
        reg[x] = mem[reg[y]];

    sprintf(instruction, "pop %s,%s", indexToName(x, 0), indexToName(y, 0));
    printf("[0x%08X]\t%-20s\t%s=MEM[%s->0x%08X]=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), reg[y] * 4, mem[reg[y]]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=MEM[%s->0x%08X]=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), reg[y] * 4, mem[reg[y]]);

    reg[32]++;
}

void addi(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    unsigned long long int reg_y_64 =  (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) imd;

    unsigned long long int int_64 = reg_y_64 + imd_64;

    char tmp = (int_64 & 0xFFFFFFFF00000000) >> 32;
     if (x == 0)
        reg[x] = 0;
    else
        reg[x] = (int_64 & 0xFFFFFFFF);

    uint32_t ov = (reg[35] & 0x10) >> 4;

    if(tmp == 1 && ov == 0)
        reg[35] = (reg[35] | 0x10);
    else if (tmp == 0 && ov == 1)
        reg[35] = (reg[35] & 0x0F);

    sprintf(instruction, "addi %s,%s,%u", indexToName(x, 0), indexToName(y, 0), imd);

    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=%s+0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=%s+0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void subi(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    unsigned long long int reg_y_64 =  (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) imd;

    unsigned long long int int_64 = reg_y_64 - imd_64;

    uint32_t tmp = (int_64 & 0xFFFFFFFF00000000) >> 32;
     if (x == 0)
        reg[x] = 0;
    else
        reg[x] = (int_64 & 0xFFFFFFFF);

    uint32_t ov = (reg[35] & 0x10) >> 4;

    if(tmp > 0 && ov == 0)
        reg[35] |= 0x10;
    else if (tmp == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "subi %s,%s,%u", indexToName(x, 0), indexToName(y, 0), imd);

    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=%s-0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=%s-0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void muli(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    unsigned long long int reg_y_64 =  (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) imd;

    unsigned long long int int_64 = reg_y_64 * imd_64;

    reg[34] = (int_64 & 0xFFFFFFFF00000000) >> 32;
     if (x == 0)
        reg[x] = 0;
    else
        reg[x] = (int_64 & 0xFFFFFFFF);

    uint32_t ov = (reg[35] & 0x10) >> 4;

    if(reg[34] > 0 && ov == 0)
        reg[35] |= 0x10;
    else if (reg[34] == 0 && ov == 1)
        reg[35] &= 0x0F;

    sprintf(instruction, "muli %s,%s,%u", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\tFR=0x%08X,ER=0x%08X,%s=%s*0x%04X=0x%08X\n", reg[32] * 4, instruction, reg[35], reg[34], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\tFR=0x%08X,ER=0x%08X,%s=%s*0x%04X=0x%08X\n", reg[32] * 4, instruction, reg[35], reg[34], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void divi(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    uint32_t zd = (reg[35] & 0x08) >> 3;
    uint32_t ov = (reg[35] & 0x10) >> 4;
    unsigned long long int ry_64 = (unsigned long long int) reg[y];
    unsigned long long int imd_64 = (unsigned long long int) imd;
    unsigned long long int rx_64;

    if (imd != 0)
    {
        if (x == 0)
            rx_64 = 0;
        else
            rx_64 = ry_64 / imd_64;
        reg[34] = ry_64 % imd_64;

        char tmp = (rx_64 & 0xFFFFFFFF00000000) >> 32;
        reg[x] = (rx_64 & 0xFFFFFFFF);

        if(tmp > 0 && ov == 0)
            reg[35] |= 0x10;
        else if (tmp == 0 && ov == 1)
            reg[35] &= 0x0F;
    }

    if (imd == 0 && zd == 0)
        reg[35] |= 0x08;
    else if (imd != 0 && zd == 1)
        reg[35] &= 0x07;


    sprintf(instruction, "divi %s,%s,%u", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=0x%08X,%s=0x%08X,%s=%s/0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(34, 1), reg[34], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X,%s=0x%08X,%s=%s/0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35], indexToName(34, 1), reg[34], indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    uint32_t ie = (reg[35] & 0x40) >> 6;
    if (imd == 0 && ie == 1)
    {
        reg[37] = reg[32] + 1;
        reg[32] = 3;
        reg[36] = 1;

        printf("[SOFTWARE INTERRUPTION]\n");
        fprintf(file, "[SOFTWARE INTERRUPTION]\n");
    }
    else
        reg[32]++;
}

void cmpi(uint32_t *reg, FILE *file)
{
    uint32_t x, imd, cmp;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;
    cmp = (reg[35] & 0xFFFFFFF8);

    if (reg[x] == imd)
        cmp |= 0x00000001;
    else if (reg[x] < imd)
        cmp |= 0x00000002;
    else
        cmp |= 0x00000004;

    reg[35] = cmp;

    sprintf(instruction, "cmpi %s,%u", indexToName(x, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=0x%08X\n", reg[32] * 4, instruction, indexToName(35, 1), reg[35]);

    reg[32]++;
}

void andi(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    if (x == 0)
        reg[x] = 0;
    else
        reg[x] = reg[y] & imd;

    sprintf(instruction, "andi %s,%s,%u", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=%s&0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s&0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void noti(uint32_t *reg, FILE *file)
{
    uint32_t x, imd;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    reg[x] = ~imd;

    sprintf(instruction, "noti %s,%u", indexToName(x, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=~0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=~0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, reg[x]);

    reg[32]++;
}

void ori(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    if (x == 0)
        reg[x] = 0;
    else
        reg[x] = reg[y] | imd;

    sprintf(instruction, "ori %s,%s,%u", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=%s|0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s|0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void xori(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    if (x == 0)
        reg[x] = 0;
    else
        reg[x] = reg[y] ^ imd;

    sprintf(instruction, "xori %s,%s,%u", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=%s^0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=%s^0x%04X=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void ldw(uint32_t *mem, uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    sprintf(instruction, "ldw %s,%s,0x%04X", indexToName(x, 0), indexToName(y, 0), imd);

    if(x == 0)
        reg[x] = 0;
    else
    {
        switch(reg[y] + imd)
        {
            case 0x00002020:
                reg[x] = wdg;
                break;
            case 0x0000888A:
                reg[x] = terminal_in;
                break;
            case 0x0000888B:
                reg[x] = terminal_out;
                break;
	        case 0x00002200:
                reg[x] = fpu_x;
                break;
	        case 0x00002201:
                reg[x] = fpu_y;
                break;
	        case 0x00002202:
                reg[x] = fpu_z;
                break;
	        case 0x00002203:
                reg[x] = fpu_control;
                break;
            default:
                reg[x] = mem[reg[y] + imd];
                break;
        }
    }

    printf("[0x%08X]\t%-20s\tR%u=MEM[(R%u+0x%04X)<<2]=0x%08X\n", reg[32] * 4, instruction, x, y, imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\tR%u=MEM[(R%u+0x%04X)<<2]=0x%08X\n", reg[32] * 4, instruction, x, y, imd, reg[x]);

    reg[32]++;
}

void stw(uint32_t *mem, uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    sprintf(instruction, "stw %s,0x%04X,%s", indexToName(x, 0), imd, indexToName(y, 0));

    switch(reg[x] + imd)
    {
        case 0x00002020:
            wdg = reg[y];
            break;
        case 0x0000888A:
            terminal_in = reg[y];
            break;
        case 0x0000888B:
            terminal_out = reg[y];
            break;
        case 0x00002200:
            fpu_x = reg[y];
            break;
        case 0x00002201:
            fpu_y = reg[y];
            break;
        case 0x00002202:
            fpu_z = reg[y];
            break;
        case 0x00002203:
            fpu_control = reg[y];
            break;
        default:
            mem[reg[x] + imd] = reg[y];
            break;
    }

    printf("[0x%08X]\t%-20s\tMEM[(%s+0x%04X)<<2]=%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, indexToName(y, 1), reg[y]);
    fprintf(file, "[0x%08X]\t%-20s\tMEM[(%s+0x%04X)<<2]=%s=0x%08X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, indexToName(y, 1), reg[y]);

    reg[32]++;
}

void ldb(uint32_t *mem, uint32_t *reg, FILE *file)
{
    uint32_t index, byte, x, y, imd, tmp;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    index = (reg[y] + imd) / 4;
    tmp = mem[index];
    byte = (reg[y] + imd) % 4;

    if (x == 0)
        reg[x] = 0;
    else
    {
        switch(byte)
        {
            case 0:
                switch(reg[y] + imd)
                {
                    case 0x00002020:
                        reg[x] = (wdg & 0xFF000000) >> 24;
                        break;
                    case 0x0000888A:
                        reg[x] = (terminal_in & 0xFF000000) >> 24;
                        break;
                    case 0x0000888B:
                        reg[x] = (terminal_out & 0xFF000000) >> 24;
                        break;
                    case 0x00002200:
                        reg[x] = (fpu_x & 0xFF000000) >> 24;
                        break;
                    case 0x00002201:
                        reg[x] = (fpu_y & 0xFF000000) >> 24;
                        break;
                    case 0x00002202:
                        reg[x] = (fpu_z & 0xFF000000) >> 24;
                        break;
                    case 0x00002203:
                        reg[x] = (fpu_control & 0xFF000000) >> 24;
                        break;
                    default:
                        reg[x] = (tmp & 0xFF000000) >> 24;
                        break;
                }
                break;
            case 1:
                switch(reg[y] + imd)
                {
                    case 0x00002020:
                        reg[x] = (wdg & 0xFF0000) >> 16;
                        break;
                    case 0x0000888A:
                        reg[x] = (terminal_in & 0xFF0000) >> 16;
                        break;
                    case 0x0000888B:
                        reg[x] = (terminal_out & 0xFF0000) >> 16;
                        break;
                    case 0x00002200:
                        reg[x] = (fpu_x & 0xFF0000) >> 16;
                        break;
                    case 0x00002201:
                        reg[x] = (fpu_y & 0xFF0000) >> 16;
                        break;
                    case 0x00002202:
                        reg[x] = (fpu_z & 0xFF000) >> 16;
                        break;
                    case 0x00002203:
                        reg[x] = (fpu_control & 0xFF000) >> 16;
                        break;
                    default:
                        reg[x] = (tmp & 0xFF0000) >> 16;
                        break;
                }
                break;
            case 2:
                switch(reg[y] + imd)
                {
                    case 0x00002020:
                        reg[x] = (wdg & 0xFF00) >> 8;
                        break;
                    case 0x0000888A:
                        reg[x] = (terminal_in & 0xFF00) >> 8;
                        break;
                    case 0x0000888B:
                        reg[x] = (terminal_out & 0xFF00) >> 8;
                        break;
                    case 0x00002200:
                        reg[x] = (fpu_x & 0xFF00) >> 8;
                        break;
                    case 0x00002201:
                        reg[x] = (fpu_y & 0xFF00) >> 8;
                        break;
                    case 0x00002202:
                        reg[x] = (fpu_z & 0xFF00) >> 8;
                        break;
                    case 0x00002203:
                        reg[x] = (fpu_control & 0xFF00) >> 8;
                        break;
                    default:
                        reg[x] = (tmp & 0xFF00) >> 8;
                        break;
                }
                break;
            case 3:
                switch(reg[y] + imd)
                {
                    case 0x00002020:
                        reg[x] = (wdg & 0xFF);
                        break;
                    case 0x0000888A:
                        reg[x] = (terminal_in & 0xFF);
                        break;
                    case 0x0000888B:
                        reg[x] = (terminal_out & 0xFF);
                        break;
                    case 0x00002200:
                        reg[x] = (fpu_x & 0xFF);
                        break;
                    case 0x00002201:
                        reg[x] = (fpu_y & 0xFF);
                        break;
                    case 0x00002202:
                        reg[x] = (fpu_z & 0xFF);
                        break;
                    case 0x00002203:
                        reg[x] = (fpu_control & 0xFF);
                        break;
                    default:
                        reg[x] = (tmp & 0xFF);
                        break;
                }
                break;
        }
    }

    sprintf(instruction, "ldb %s,%s,0x%04X", indexToName(x, 0), indexToName(y, 0), imd);

    printf("[0x%08X]\t%-20s\t%s=MEM[%s+0x%04X]=0x%02X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);
    fprintf(file, "[0x%08X]\t%-20s\t%s=MEM[%s+0x%04X]=0x%02X\n", reg[32] * 4, instruction, indexToName(x, 1), indexToName(y, 1), imd, reg[x]);

    reg[32]++;
}

void stb(uint32_t *mem, uint32_t *reg, FILE *file, List *terminal)
{
    uint32_t index, byte, x, y, imd, tmp;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;

    index = (reg[x] + imd) / 4;

    switch(reg[x] + imd)
    {
        case 0x00002020:
            tmp = wdg;
            break;
        case 0x0000888A:
            tmp = terminal_in;
        case 0x0000888B:
            tmp = terminal_out;
            break;
        case 0x00002200:
            tmp = fpu_x;
            break;
        case 0x00002201:
            tmp = fpu_y;
            break;
        case 0x00002202:
            tmp = fpu_z;
            break;
        case 0x00002203:
            tmp = fpu_control;
            break;
        default:
            tmp = mem[index];
            break;
    }

    byte = (reg[x] + imd) % 4;
    uint32_t out;

    if (byte == 0)
    {
        switch (reg[x] + imd)
        {
            case 0x00002020:
                wdg = (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x0000888A:
                terminal_in = (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x0000888B:
                terminal_out = (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                insert(terminal, (char)terminal_out);
                break;
            case 0x00002200:
                fpu_x = (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002201:
                fpu_y = (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002202:
                fpu_z = (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002203:
                fpu_control = (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            default:
                mem[index] =  (reg[y] << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
        }
    }
    else if (byte == 1)
    {
        switch (reg[x] + imd)
        {
            case 0x00002020:
                wdg = (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x0000888A:
                terminal_in = (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x0000888B:
                terminal_out = (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                insert(terminal, (char)terminal_out);
                break;
            case 0x00002200:
                fpu_x = (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002201:
                fpu_y = (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002202:
                fpu_z = (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002203:
                fpu_control =  (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            default:
                mem[index] = (((tmp & 0xFF000000) >> 24) << 24) | (reg[y] << 16) | (((tmp & 0xFF00) >> 8) << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
        }

    }
    else if (byte == 2)
    {
        switch (reg[x] + imd)
        {
            case 0x00002020:
                wdg = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x0000888A:
                terminal_in = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x0000888B:
                terminal_out = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
               insert(terminal, (char)terminal_out);
                break;
            case 0x00002200:
                fpu_x = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002201:
                fpu_y = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002202:
                fpu_z = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002203:
                fpu_control = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
            default:
                mem[index] = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (reg[y] << 8) | (tmp & 0xFF);
                out = (reg[y] << 24) >> 24;
                break;
        }
    }
    else if (byte == 3)
    {
        switch (reg[x] + imd)
        {
            case 0x00002020:
                wdg = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
                out = (reg[y] << 24) >> 24;
                break;
            case 0x0000888A:
                terminal_in = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
                out = (reg[y] << 24) >> 24;
                break;
            case 0x0000888B:
                terminal_out = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
                out = (reg[y] << 24) >> 24;
                insert(terminal, (char)terminal_out);
                break;
            case 0x00002200:
                fpu_x = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002201:
                fpu_y = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002202:
                fpu_z = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
                out = (reg[y] << 24) >> 24;
                break;
            case 0x00002203:
                fpu_control = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
                out = (reg[y] << 24) >> 24;
                break;
            default:
                mem[index] = (((tmp & 0xFF000000) >> 24) << 24) | (((tmp & 0xFF0000) >> 16) << 16) | (((tmp & 0xFF00) >> 8) << 8) | reg[y];
                out = (reg[y] << 24) >> 24;
                break;
        }
    }

    sprintf(instruction, "stb %s,0x%04X,%s", indexToName(x, 0), imd, indexToName(y, 0));

    printf("[0x%08X]\t%-20s\tMEM[%s+0x%04X]=%s=0x%02X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, indexToName(y, 1), out);
    fprintf(file, "[0x%08X]\t%-20s\tMEM[%s+0x%04X]=%s=0x%02X\n", reg[32] * 4, instruction, indexToName(x, 1), imd, indexToName(y, 1), out);

    reg[32]++;
}

void bun(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32];
    reg[32] = (reg[33] & 0x3FFFFFF);
    char instruction[20];
    sprintf(instruction, "bun 0x%08X", reg[32]);
    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
}

void beq(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
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

void blt(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
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

void bgt(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
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

void bne(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
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

void ble(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
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

void bge(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
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

void bzd(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
    char instruction[20];

    cmp = (reg[35] & 0x08) >> 3;
    sprintf(instruction, "bzd 0x%08X", (reg[33] & 0x3FFFFFF));

    if(cmp == 1)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
}

void bnz(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
    char instruction[20];

    cmp = (reg[35] & 0x08) >> 3;
    sprintf(instruction, "bnz 0x%08X", (reg[33] & 0x3FFFFFF));

    if(cmp == 0)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
}

void biv(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
    char instruction[20];

    cmp = (reg[35] & 0x20) >> 5;
    sprintf(instruction, "biv 0x%08X", (reg[33] & 0x3FFFFFF));

    if(cmp == 1)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
}

void bni(uint32_t *reg, FILE *file)
{
    uint32_t old = reg[32], cmp;
    char instruction[20];

    cmp = (reg[35] & 0x20) >> 4;
    sprintf(instruction, "bni 0x%08X", (reg[33] & 0x3FFFFFF));

    if(cmp == 0)
        reg[32] = (reg[33] & 0x3FFFFFF);
    else
        reg[32]++;

    printf("[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=0x%08X\n", old * 4, instruction, reg[32] * 4);
}

void call(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd, old;
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
    printf("[0x%08X]\t%-20s\t%s=(PC+4)>>2=0x%08X,PC=(%s+0x%04X)<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[x], indexToName(y, 1), imd, reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\t%s=(PC+4)>>2=0x%08X,PC=(%s+0x%04X)<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[x], indexToName(y, 1), imd, reg[32] * 4);
}

void ret(uint32_t *reg, FILE *file)
{
    uint32_t x, old;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    old = reg[32];

    reg[32] = reg[x];

    sprintf(instruction, "ret %s", indexToName(x, 0));
    printf("[0x%08X]\t%-20s\tPC=%s<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=%s<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[32] * 4);
}

void isr(uint32_t *reg, FILE *file)
{
    uint32_t x, y, imd, old;
    char instruction[20];

    y = (reg[33] & 0x1F);
    x = (reg[33] & 0x3E0) >> 5;
    imd = (reg[33] & 0x3FFFC00) >> 10;
    old = reg[32];

    if (x == 0)
        reg[x] = 0;
    else
        reg[x] = reg[37];
    
    if (y == 0)
        reg[y] = 0;
    else
        reg[y] = reg[36];

    reg[32] = imd;

    sprintf(instruction, "isr %s,%s,0x%04X", indexToName(x, 0), indexToName(y, 0), imd);
    printf("[0x%08X]\t%-20s\t%s=IPC>>2=0x%08X,%s=CR=0x%08X,PC=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[x], indexToName(y, 1), reg[y], imd * 4);
    fprintf(file, "[0x%08X]\t%-20s\t%s=IPC>>2=0x%08X,%s=CR=0x%08X,PC=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[x], indexToName(y, 1), reg[y], imd * 4);
}

void reti(uint32_t *reg, FILE *file)
{
    uint32_t x, old;
    char instruction[20];

    x = (reg[33] & 0x3E0) >> 5;
    old = reg[32];

    reg[32] = reg[x];
    
    if ((int_ctrl & 0x01) == 1)
        int_ctrl &= 0xFFFFFFFE;
    else if ((int_ctrl & 0x02) >> 1 == 1)
        int_ctrl &= 0xFFFFFFFD;
    else if ((int_ctrl & 0x04) >> 2 == 1)
        int_ctrl &= 0xFFFFFFFB;

    sprintf(instruction, "reti %s", indexToName(x, 0));
    printf("[0x%08X]\t%-20s\tPC=%s<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tPC=%s<<2=0x%08X\n", old * 4, instruction, indexToName(x, 1), reg[32] * 4);
}

void _int(uint32_t *reg, FILE *file)
{
    uint32_t imd, old = reg[32];
    char instruction[20];

    imd = (reg[33] & 0x3FFFFFF);
    
    if (imd == 0)
    {
        reg[32] = imd;
        reg[36] = 0;
    }
    else
    {
        reg[37] = reg[32] + 1;
        reg[32] = 3;
        reg[36] = imd;
    }

    sprintf(instruction, "int %u", imd);
    printf("[0x%08X]\t%-20s\tCR=0x%08X,PC=0x%08X\n", old * 4, instruction, reg[36], reg[32] * 4);
    fprintf(file, "[0x%08X]\t%-20s\tCR=0x%08X,PC=0x%08X\n", old * 4, instruction, reg[36], reg[32] * 4);

    if (imd != 0)
    {
        printf("[SOFTWARE INTERRUPTION]\n");
        fprintf(file, "[SOFTWARE INTERRUPTION]\n");
    }

}

void invalid(uint32_t *reg, FILE *file)
{
    reg[35] |= 0x20;
    reg[36] = reg[32];
    printf("[INVALID INSTRUCTION @ 0x%08X]\n", reg[32] * 4);
    fprintf(file, "[INVALID INSTRUCTION @ 0x%08X]\n", reg[32] * 4);

    reg[37] = reg[32] + 1;
    reg[32] = 3;

    printf("[SOFTWARE INTERRUPTION]\n");
    fprintf(file, "[SOFTWARE INTERRUPTION]\n");
}

uint32_t getInstruction(uint32_t index, uint32_t *mem, uint32_t length, FILE *file)
{
    index *= 4;
    uint32_t alignment = (index & 0x03);
    uint32_t word = (index & 0x0C) >> 2;
    uint32_t line = (index & 0x70) >> 4;
    uint32_t id = (index & 0xFFFFFF80);

    char instruction[20], str_set0[80], str_set1[80];
    
    for (int i = 0; i <  8; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (cache_i[i].set[j].valid == 1)
                cache_i[i].set[j].age ++;
        }
    }
    
    int old = 0;
    
    for (int i = 0; i < 2; i++)
    {
        if (cache_i[line].set[i].valid == 1 )
        {
            if (cache_i[line].set[i].id == id)
            {
                cache_i[line].set[i].age = 0;
                sprintf(instruction, "read_hit I->%u", line);
                sprintf(str_set0, "SET=0:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_i[line].set[0].valid), cache_i[line].set[0].age, blockToString(cache_i[line].set[0].block));
                sprintf(str_set1, "SET=1:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_i[line].set[1].valid), cache_i[line].set[1].age, blockToString(cache_i[line].set[1].block));
                printf("[0x%08X]\t%-20s\t%s\n\t\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
                fprintf(file, "[0x%08X]\t%-20s\t%s\n\t\t\t\t\t\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
                cache_i_hit_counter++;
                return cache_i[line].set[i].block[word];
            }
            else if (cache_i[line].set[i].id != id)
            {
                if (old < cache_i[line].set[i].age)
                    old = cache_i[line].set[i].age;

                if (i == 1)
                {
                    sprintf(instruction, "read_miss I->%u", line);
                    sprintf(str_set0, "SET=0:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_i[line].set[0].valid), cache_i[line].set[0].age, blockToString(cache_i[line].set[0].block));
                    sprintf(str_set1, "SET=1:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_i[line].set[1].valid), cache_i[line].set[1].age, blockToString(cache_i[line].set[1].block));
                    printf("[0x%08X]\t%-20s\t%s\n\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
                    fprintf(file, "[0x%08X]\t%-20s\t%s\n\t\t\t\t\t\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
                    if (old == cache_i[line].set[0].age)
                    {
                        int k = index >> 2;
                        if (length % 4 != 0)
                        {
                            int diff = length - (index / 4);
                            if (diff < 4)
                            {
                                switch(diff)
                                {
                                    case 3:
                                        for (int j = 0; j < 3; j++)
                                        {
                                            cache_i[line].set[0].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_i[line].set[0].block[3] = 0;
                                        break;
                                    case 2:
                                        for (int j = 0; j < 2; j++)
                                        {
                                            cache_i[line].set[0].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_i[line].set[0].block[2] = 0;
                                        cache_i[line].set[0].block[3] = 0;
                                        break;
                                    case 1:
                                        for (int j = 0; j < 1; j++)
                                        {
                                            cache_i[line].set[0].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_i[line].set[0].block[1] = 0;
                                        cache_i[line].set[0].block[2] = 0;
                                        cache_i[line].set[0].block[3] = 0;
                                        break;
                                }
                            }
                            else
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    cache_i[line].set[0].block[j] = mem[k];
                                    k++;
                                }
                            }
                        }
                        else
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                cache_i[line].set[0].block[j] = mem[k];
                                k++;
                            }
                        }
                        cache_i[line].set[0].age = 0;
                        cache_i[line].set[0].id = id;
                        cache_i_miss_counter++;
                        return mem[index / 4];
                    }
                    else
                    {
                        int k = index >> 2;
                        if (length % 4 != 0)
                        {
                            int diff = length - (index / 4);
                            if (diff < 4)
                            {
                                switch(diff)
                                {
                                    case 3:
                                        for (int j = 0; j < 3; j++)
                                        {
                                            cache_i[line].set[1].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_i[line].set[1].block[3] = 0;
                                        break;
                                    case 2:
                                        for (int j = 0; j < 2; j++)
                                        {
                                            cache_i[line].set[1].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_i[line].set[1].block[2] = 0;
                                        cache_i[line].set[1].block[3] = 0;
                                        break;
                                    case 1:
                                        for (int j = 0; j < 1; j++)
                                        {
                                            cache_i[line].set[1].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_i[line].set[1].block[1] = 0;
                                        cache_i[line].set[1].block[2] = 0;
                                        cache_i[line].set[1].block[3] = 0;
                                        break;
                                }
                            }
                            else
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    cache_i[line].set[1].block[j] = mem[k];
                                    k++;
                                }
                            }
                        }
                        else
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                cache_i[line].set[1].block[j] = mem[k];
                                k++;
                            }
                        }
                        cache_i[line].set[1].age = 0;
                        cache_i[line].set[1].id = id;
                        cache_i_miss_counter++;
                        return mem[index / 4];
                    }
                }
            }
        }
        else
        {
            sprintf(instruction, "read_miss I->%u", line);
            sprintf(str_set0, "SET=0:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_i[line].set[0].valid), cache_i[line].set[0].age, blockToString(cache_i[line].set[0].block));
            sprintf(str_set1, "SET=1:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_i[line].set[1].valid), cache_i[line].set[1].age, blockToString(cache_i[line].set[1].block));
            printf("[0x%08X]\t%-20s\t%s\n\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
            fprintf(file, "[0x%08X]\t%-20s\t%s\n\t\t\t\t\t\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
            int k = index >> 2;
            if (length % 4 != 0)
            {
                int diff = length - (index / 4);
                if (diff < 4)
                {
                    switch(diff)
                    {
                        case 3:
                            for (int j = 0; j < 3; j++)
                            {
                                cache_i[line].set[i].block[j] = mem[k];
                                k++;
                            }
                            cache_i[line].set[i].block[3] = 0;
                            break;
                        case 2:
                            for (int j = 0; j < 2; j++)
                            {
                                cache_i[line].set[i].block[j] = mem[k];
                                k++;
                            }
                            cache_i[line].set[i].block[2] = 0;
                            cache_i[line].set[i].block[3] = 0;
                            break;
                        case 1:
                            for (int j = 0; j < 1; j++)
                            {
                                cache_i[line].set[i].block[j] = mem[k];
                                k++;
                            }
                            cache_i[line].set[i].block[1] = 0;
                            cache_i[line].set[i].block[2] = 0;
                            cache_i[line].set[i].block[3] = 0;
                            break;
                    }
                }
                else
                {
                    for (int j = 0; j < 4; j++)
                    {
                        cache_i[line].set[i].block[j] = mem[k];
                        k++;
                    }
                }
            }
            else
            {
                for (int j = 0; j < 4; j++)
                {
                    cache_i[line].set[i].block[j] = mem[k];
                    k++;
                }
            }
            cache_i[line].set[i].valid = 1;
            cache_i[line].set[i].id = id;
            cache_i_miss_counter++;
            return mem[index / 4];
        }
    }
}

uint32_t getData(uint32_t index, uint32_t *mem, uint32_t length, FILE *file)
{
    index *= 4;
    uint32_t alignment = (index & 0x03);
    uint32_t word = (index & 0x0C) >> 2;
    uint32_t line = (index & 0x70) >> 4;
    uint32_t id = (index & 0xFFFFFF80);

    char instruction[20], str_set0[80], str_set1[80];
    
    for (int i = 0; i <  8; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (cache_d[i].set[j].valid == 1)
                cache_d[i].set[j].age ++;
        }
    }
    
    int old = 0;
    
    for (int i = 0; i < 2; i++)
    {
        if (cache_d[line].set[i].valid == 1 )
        {
            if (cache_d[line].set[i].id == id)
            {
                cache_d[line].set[i].age = 0;
                sprintf(instruction, "read_hit D->%u", line);
                sprintf(str_set0, "SET=0:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_d[line].set[0].valid), cache_d[line].set[0].age, blockToString(cache_d[line].set[0].block));
                sprintf(str_set1, "SET=1:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_d[line].set[1].valid), cache_d[line].set[1].age, blockToString(cache_d[line].set[1].block));
                printf("[0x%08X]\t%-20s\t%s\n\t\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
                fprintf(file, "[0x%08X]\t%-20s\t%s\n\t\t\t\t\t\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
                cache_d_hit_counter++;
                return cache_d[line].set[i].block[word];
            }
            else if (cache_d[line].set[i].id != id)
            {
                if (old < cache_d[line].set[i].age)
                    old = cache_d[line].set[i].age;

                if (i == 1)
                {
                    sprintf(instruction, "read_miss D->%u", line);
                    sprintf(str_set0, "SET=0:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_d[line].set[0].valid), cache_d[line].set[0].age, blockToString(cache_d[line].set[0].block));
                    sprintf(str_set1, "SET=1:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_d[line].set[1].valid), cache_d[line].set[1].age, blockToString(cache_d[line].set[1].block));
                    printf("[0x%08X]\t%-20s\t%s\n\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
                    fprintf(file, "[0x%08X]\t%-20s\t%s\n\t\t\t\t\t\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
                    if (old == cache_i[line].set[0].age)
                    {
                        int k = index >> 2;
                        if (length % 4 != 0)
                        {
                            int diff = length - (index / 4);
                            if (diff < 4)
                            {
                                switch(diff)
                                {
                                    case 3:
                                        for (int j = 0; j < 3; j++)
                                        {
                                            cache_d[line].set[0].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_d[line].set[0].block[3] = 0;
                                        break;
                                    case 2:
                                        for (int j = 0; j < 2; j++)
                                        {
                                            cache_d[line].set[0].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_d[line].set[0].block[2] = 0;
                                        cache_d[line].set[0].block[3] = 0;
                                        break;
                                    case 1:
                                        for (int j = 0; j < 1; j++)
                                        {
                                            cache_d[line].set[0].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_d[line].set[0].block[1] = 0;
                                        cache_d[line].set[0].block[2] = 0;
                                        cache_d[line].set[0].block[3] = 0;
                                        break;
                                }
                            }
                            else
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    cache_d[line].set[0].block[j] = mem[k];
                                    k++;
                                }
                            }
                        }
                        else
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                cache_d[line].set[0].block[j] = mem[k];
                                k++;
                            }
                        }
                        cache_d[line].set[0].age = 0;
                        cache_d[line].set[0].id = id;
                        cache_d_miss_counter++;
                        return mem[index / 4];
                    }
                    else
                    {
                        int k = index >> 2;
                        if (length % 4 != 0)
                        {
                            int diff = length - (index / 4);
                            if (diff < 4)
                            {
                                switch(diff)
                                {
                                    case 3:
                                        for (int j = 0; j < 3; j++)
                                        {
                                            cache_d[line].set[1].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_d[line].set[1].block[3] = 0;
                                        break;
                                    case 2:
                                        for (int j = 0; j < 2; j++)
                                        {
                                            cache_d[line].set[1].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_d[line].set[1].block[2] = 0;
                                        cache_d[line].set[1].block[3] = 0;
                                        break;
                                    case 1:
                                        for (int j = 0; j < 1; j++)
                                        {
                                            cache_d[line].set[1].block[j] = mem[k];
                                            k++;
                                        }
                                        cache_d[line].set[1].block[1] = 0;
                                        cache_d[line].set[1].block[2] = 0;
                                        cache_d[line].set[1].block[3] = 0;
                                        break;
                                }
                            }
                            else
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    cache_d[line].set[1].block[j] = mem[k];
                                    k++;
                                }
                            }
                        }
                        else
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                cache_d[line].set[1].block[j] = mem[k];
                                k++;
                            }
                        }
                        cache_d[line].set[1].age = 0;
                        cache_d[line].set[1].id = id;
                        cache_d_miss_counter++;
                        return mem[index / 4];
                    }
                }
            }
        }
        else
        {
            sprintf(instruction, "read_miss D->%u", line);
            sprintf(str_set0, "SET=0:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_d[line].set[0].valid), cache_d[line].set[0].age, blockToString(cache_d[line].set[0].block));
            sprintf(str_set1, "SET=1:STATUS=%s,AGE=%u,DATA=%s", validToString(cache_d[line].set[1].valid), cache_d[line].set[1].age, blockToString(cache_d[line].set[1].block));
            printf("[0x%08X]\t%-20s\t%s\n\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
            fprintf(file, "[0x%08X]\t%-20s\t%s\n\t\t\t\t\t\t\t\t\t\t%s\n", index, instruction, str_set0, str_set1);
            int k = index >> 2;
            if (length % 4 != 0)
            {
                int diff = length - (index / 4);
                if (diff < 4)
                {
                    switch(diff)
                    {
                        case 3:
                            for (int j = 0; j < 3; j++)
                            {
                                cache_d[line].set[i].block[j] = mem[k];
                                k++;
                            }
                            cache_d[line].set[i].block[3] = 0;
                            break;
                        case 2:
                            for (int j = 0; j < 2; j++)
                            {
                                cache_d[line].set[i].block[j] = mem[k];
                                k++;
                            }
                            cache_d[line].set[i].block[2] = 0;
                            cache_d[line].set[i].block[3] = 0;
                            break;
                        case 1:
                            for (int j = 0; j < 1; j++)
                            {
                                cache_d[line].set[i].block[j] = mem[k];
                                k++;
                            }
                            cache_d[line].set[i].block[1] = 0;
                            cache_d[line].set[i].block[2] = 0;
                            cache_d[line].set[i].block[3] = 0;
                            break;
                    }
                }
                else
                {
                    for (int j = 0; j < 4; j++)
                    {
                        cache_d[line].set[i].block[j] = mem[k];
                        k++;
                    }
                }
            }
            else
            {
                for (int j = 0; j < 4; j++)
                {
                    cache_d[line].set[i].block[j] = mem[k];
                    k++;
                }
            }
            cache_d[line].set[i].valid = 1;
            cache_d[line].set[i].id = id;
            cache_d_miss_counter++;
            return mem[index / 4];
        }
    }
}

void cacheStats(FILE *file)
{
    float total_i = (float)(cache_i_hit_counter + cache_i_miss_counter);
    float ta_i_f = (float)(cache_i_hit_counter / total_i) * 100;
    float tf_i_f = (float)(cache_i_miss_counter / total_i) * 100;
    uint32_t ta_i =  (uint32_t) roundf(ta_i_f);
    uint32_t tf_i = (uint32_t) roundf(tf_i_f);
    float total_d = (float)(cache_d_hit_counter + cache_d_miss_counter);
    float ta_d_f = (float)(cache_i_hit_counter / total_d) * 100;
    float tf_d_f = (float)(cache_i_miss_counter / total_d) * 100;
    uint32_t ta_d =  (uint32_t) roundf(ta_d_f);
    uint32_t tf_d = (uint32_t) roundf(tf_d_f);
    printf("[CACHE D STATISTICS] #Hit = %u (%u%%), #Miss = %u (%u%%)\n", cache_d_hit_counter, ta_d, cache_d_miss_counter, tf_d);
    fprintf(file, "[CACHE D STATISTICS] #Hit = %u (%u%%), #Miss = %u (%u%%)\n", cache_d_hit_counter, ta_d, cache_d_miss_counter, tf_d);
    printf("[CACHE I STATISTICS] #Hit = %u (%u%%), #Miss = %u (%u%%)\n", cache_i_hit_counter, ta_i, cache_i_miss_counter, tf_i);
    fprintf(file, "[CACHE I STATISTICS] #Hit = %u (%u%%), #Miss = %u (%u%%)\n", cache_i_hit_counter, ta_i, cache_i_miss_counter, tf_i);
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

char * indexToName(uint32_t index, int upperCase)
{
    char *str = (char *) malloc(sizeof(char) * 4);

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
        case 36:
            sprintf(str, "cr");
            break;
        default:
            sprintf(str, "r%u", index);
            break;
    }

    if (upperCase == 1)
    {
        for(int i = 0; str[i] != '\0'; i++){
            str[i] = toupper(str[i]);
        }
    }
    return str;
}
char * validToString(uint32_t valid)
{
    char *str = (char *) malloc(sizeof(char) * 7);

    if (valid == 0)
        sprintf(str, "INVALID");
    else
        sprintf(str, "VALID");
    return str;
}

char * blockToString(uint32_t *block)
{
    char *str = (char *) malloc(sizeof(char) * 43);
    sprintf(str, "0x%08X|0x%08X|0x%08X|0x%08X", block[0], block[1], block[2], block[3]);
    return str;
}

List* createList()
{
    List* list = (List*) malloc(sizeof(List));
    
    list->head = NULL;
    list->size = 0;
    
    return list;
}

void insert(List *list, char c)
{
    Node *node = (Node*) malloc(sizeof(Node));
    node->character = c;
    node->next = NULL;
    if (list->head == NULL && list->size == 0)
        list->head = node;
    else
    {
        Node* aux = list->head;
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = node;
    }
    list->size++;
}