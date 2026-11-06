#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// マクロここから =============================================================================
#define TO_STRING(def) #def,
#define TO_ENUM(def) def,
#define TO_STR_AND_ENUM(def) {#def, def},

#define MNEMONIC(X)            \
    /*頻出命令*/               \
    X(IMM)                     \
    X(MOV)                     \
    /*四則演算*/               \
    X(ADD)                     \
    X(SUB)                     \
    X(MUL)                     \
    X(DIV)                     \
    /*ジャンプ*/               \
    X(JMP)                     \
    /*比較、比較結果ジャンプ*/ \
    X(CMP)                     \
    X(JG)                      \
    X(JE)                      \
    X(JNE)                     \
    X(JL)                      \
    X(JGE)                     \
    X(JLE)                     \
    /*論理演算*/               \
    X(AND)                     \
    X(OR)                      \
    X(XOR)                     \
    X(NOT)

#define REGISTER(R) \
    R(eax)          \
    R(ebx)          \
    R(ecx)          \
    R(edx)

#define FLAG(F) \
    F(ZF)       \
    F(SF)       \
    /* OF,CFはあとで作れそうなら*/
// マクロここまで ==========================================================================================

typedef enum
{
    // eax,ebx,,,,,,
    REGISTER(TO_ENUM)
    REG_COUNT
} Reg_id;

const char *reg_names[] = {
    // "eax","ebx",,,,,
    REGISTER(TO_STRING)};

typedef enum
{
    // ZF,SF,,,,,,
    FLAG(TO_ENUM)
    FLAG_COUNT
} Flag_id;

const char *flag_names[] = {
    // "ZF","SF",,,,,,
    FLAG(TO_STRING)};

typedef struct
{
    int reg[REG_COUNT];
    int pc;
    int flag[FLAG_COUNT]; // OF,CFは今回除外
} Cpu;

typedef enum
{
    // IMM,MOV,,,,,,
    MNEMONIC(TO_ENUM)
} Opcode_id;

typedef struct
{
    const char *mnemonic;
    Opcode_id code;
} Opcode_info;

Opcode_info opcode_table[] = {
    // {"IMM", IMM}, {"MOV", MOV},,,,,,
    MNEMONIC(TO_STR_AND_ENUM)};

typedef struct
{
    Opcode_id opcode;
    Reg_id dest;
    Reg_id src1;
    Reg_id src2;
    int value;
} Inst;

void results_display(Cpu *my_cpu)
{
    printf("[結果]\n");
    for (int i = 0; i < REG_COUNT; i++)
    {
        printf("%s:%d\n", reg_names[i], my_cpu->reg[i]);
    }
    for (int i = 0; i < FLAG_COUNT; i++)
    {
        printf("%s:%d\n", flag_names[i], my_cpu->flag[i]);
    }
}
void instruction_execution(Cpu *ptr_my_cpu, Inst my_inst[], int my_inst_length)
{
    while (ptr_my_cpu->pc < my_inst_length)
    {
        Inst current = my_inst[ptr_my_cpu->pc];
        printf("<命令%d>\n", ptr_my_cpu->pc + 1);
        switch (current.opcode)
        {
        case IMM:
        {
            ptr_my_cpu->reg[current.dest] = current.value;
            printf("IMM %s,%d\n", reg_names[current.dest], current.value);
            break;
        }
        case MOV:
        {
            ptr_my_cpu->reg[current.dest] = ptr_my_cpu->reg[current.src1];
            printf("MOV %s,%s\n", reg_names[current.dest], reg_names[current.src1]);
            break;
        }
        case ADD:
        {
            ptr_my_cpu->reg[current.dest] = ptr_my_cpu->reg[current.src1] + ptr_my_cpu->reg[current.src2];
            printf("ADD %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        case SUB:
            ptr_my_cpu->reg[current.dest] = ptr_my_cpu->reg[current.src1] - ptr_my_cpu->reg[current.src2];
            printf("SUB %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        case MUL:
        {
            ptr_my_cpu->reg[current.dest] = ptr_my_cpu->reg[current.src1] * ptr_my_cpu->reg[current.src2];
            printf("MUL %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        case DIV:
        {
            if (ptr_my_cpu->reg[current.src2] == 0)
            {
                printf("<命令%d>がエラーです\n", ptr_my_cpu->pc + 1);
                break;
            }
            ptr_my_cpu->reg[current.dest] = ptr_my_cpu->reg[current.src1] / ptr_my_cpu->reg[current.src2];
            printf("DIV %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        case JMP:
        {
            if (current.value < 1 || current.value > my_inst_length)
            {
                printf("<命令%d>がエラーです\n", ptr_my_cpu->pc + 1);
                break;
            }

            ptr_my_cpu->pc = current.value - 2;
            printf("JMP %d\n", current.value);
            break;
        }
        case CMP:
        {
            for (int i = 0; i < FLAG_COUNT; i++)
            {
                ptr_my_cpu->flag[i] = 0;
            }
            int cmp_result = ptr_my_cpu->reg[current.src1] - ptr_my_cpu->reg[current.src2];
            if (cmp_result == 0)
            {
                ptr_my_cpu->flag[ZF] = 1;
            }
            if (cmp_result < 0)
            {
                ptr_my_cpu->flag[SF] = 1;
            }
            printf("CMP %s,%s\n", reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        case JE:
        {
            if (current.value < 1 || current.value > my_inst_length)
            {
                printf("<命令%d>がエラーです\n", ptr_my_cpu->pc + 1);
                break;
            }
            if (ptr_my_cpu->flag[ZF] == 1)
            {
                ptr_my_cpu->pc = current.value - 2;
                printf("JE %d\n", current.value);
            }
            break;
        }
        case JNE:
        {
            if (current.value < 1 || current.value > my_inst_length)
            {
                printf("<命令%d>がエラーです\n", ptr_my_cpu->pc + 1);
                break;
            }
            if (ptr_my_cpu->flag[ZF] == 0)
            {
                ptr_my_cpu->pc = current.value - 2;
                printf("JNE %d\n", current.value);
            }
            break;
        }
        case JG:
        {
            if (current.value < 1 || current.value > my_inst_length)
            {
                printf("<命令%d>がエラーです\n", ptr_my_cpu->pc + 1);
                break;
            }
            if (ptr_my_cpu->flag[SF] == 0 && ptr_my_cpu->flag[ZF] == 0)
            {
                ptr_my_cpu->pc = current.value - 2;
                printf("JG %d\n", current.value);
            }
            break;
        }
        case JL:
        {
            if (current.value < 1 || current.value > my_inst_length)
            {
                printf("<命令%d>がエラーです\n", ptr_my_cpu->pc + 1);
                break;
            }
            if (ptr_my_cpu->flag[SF] == 1)
            {
                ptr_my_cpu->pc = current.value - 2;
                printf("JL %d\n", current.value);
            }
            break;
        }
        case JGE:
        {
            if (current.value < 1 || current.value > my_inst_length)
            {
                printf("<命令%d>がエラーです\n", ptr_my_cpu->pc + 1);
                break;
            }
            if (ptr_my_cpu->flag[SF] == 0)
            {
                ptr_my_cpu->pc = current.value - 2;
                printf("JGE %d\n", current.value);
            }
            break;
        }
        case JLE:
        {
            if (current.value < 1 || current.value > my_inst_length)
            {
                printf("<命令%d>がエラーです\n", ptr_my_cpu->pc + 1);
                break;
            }
            if (ptr_my_cpu->flag[SF] == 1 || ptr_my_cpu->flag[ZF] == 1)
            {
                ptr_my_cpu->pc = current.value - 2;
                printf("JLE %d\n", current.value);
            }
            break;
        }
        case AND:
        {
            ptr_my_cpu->reg[current.dest] = ptr_my_cpu->reg[current.src1] & ptr_my_cpu->reg[current.src2];
            printf("AND %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        case OR:
        {
            ptr_my_cpu->reg[current.dest] = ptr_my_cpu->reg[current.src1] | ptr_my_cpu->reg[current.src2];
            printf("OR %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        case XOR:
        {
            ptr_my_cpu->reg[current.dest] = ptr_my_cpu->reg[current.src1] ^ ptr_my_cpu->reg[current.src2];
            printf("XOR %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        case NOT:
        {
            ptr_my_cpu->reg[current.dest] = ~ptr_my_cpu->reg[current.src1];
            printf("NOT %s,%s\n", reg_names[current.dest], reg_names[current.src1]);
            break;
        }
        }
        results_display(ptr_my_cpu);
        ptr_my_cpu->pc++;
    }
}

void draw_cpu(Cpu *before_my_cpu, Cpu *after_my_cpu)
{
    printf("\n");
    printf("      before          after      \n");
    printf("+---------------+---------------+\n");
    printf("|      CPU      |      CPU      |\n");
    printf("+---------------+---------------+\n");
    for (int i = 0; i < REG_COUNT; i++)
    {
        // printf("|               |               |\n");
        printf("|  %-5s:%5d  |  %-5s:%5d  |\n", reg_names[i], before_my_cpu->reg[i], reg_names[i], after_my_cpu->reg[i]);
    }
    printf("+---------------+---------------+\n");
    printf("|     FLAGS     |     FLAGS     |\n");
    for (int i = 0; i < FLAG_COUNT; i++)
    {
        printf("|  %-5s:%5d  |  %-5s:%5d  |\n", flag_names[i], before_my_cpu->flag[i], flag_names[i], after_my_cpu->flag[i]);
    }

    printf("+---------------+---------------+\n");
    printf("\n");
}
void memory_cpu(Cpu *ptr_my_cpu, Cpu *ptr_memo_cpu)
{
    *ptr_memo_cpu = *ptr_my_cpu;
}

int main(void)
{
    Cpu my_cpu = {0};
    Cpu *ptr_my_cpu = &my_cpu;

    Cpu memo_cpu = {0};
    Cpu *ptr_memo_cpu = &memo_cpu;

    const char *file_name = "program.asm";

    FILE *fp = fopen(file_name, "r");
    if (!fp)
    {
        printf("ファイルを開けません\n");
        return 1;
    }
    else
    {
        printf("開けてる");
        // // ファイル行数取得
        // int whole_line_count = 0;
        // int checker;
        // while ((checker = fgetc(fp)) != EOF)
        // {
        //     if (checker == '\n')
        //     {
        //         whole_line_count++;
        //     }
        // }
        // rewind(fp);

        // Inst my_inst[whole_line_count];
        // char line[100];
        // char mnemonic[10];
        // int line_count = 0;

        // while (fgets(line, sizeof(line), fp) != NULL)
        // {
        //     char *ptr_space = strchr(line, ' ');    // 空白のアドレス
        //     int mnemonic_length = ptr_space - line; //

        //     for (int i = 0; i < mnemonic_length; i++)
        //     {
        //         mnemonic[i] = line[i];
        //     }
        //     mnemonic[mnemonic_length] = '\0';
        //     // // 文字列比較のためにopcodeに文字が必要。{"IMM",IMM}で管理する
        //     // for (int i = 0; i < sizeof(Opcode_id) / sizeof(Opcode_id[0]); i++)
        //     // {
        //     //     if (strcmp(mnemonic, Opcode_id[i].name){
        //     //         my_inst[line_count].opcode = IMM;
        //     // }
        //     // }

        //     line_count++;
        // }

        // fclose(fp);

        // int my_inst_length = sizeof(my_inst) / sizeof(my_inst[0]);
        // memory_cpu(ptr_my_cpu, ptr_memo_cpu);
        // instruction_execution(ptr_my_cpu, my_inst, my_inst_length);
        // draw_cpu(ptr_memo_cpu, ptr_my_cpu);
    }
}