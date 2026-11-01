#include <stdio.h>
typedef enum
{
    eax,
    ebx,
    ecx,
    edx,
} reg_id;

const char *reg_names[] = {
    "eax", "ebx", "ecx", "edx"};

typedef struct CPU
{
    int reg[4];
    int pc;
    int flag[2]; // OF,CFは今回除外
} cpu;

typedef enum
{
    // 四則演算
    IMM,
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    // ジャンプ
    JMP,
    // 比較、比較結果ジャンプ
    CMP,
    JG,
    JE,
    JNE,
    JL,
    JGE,
    JLE,
    // 論理演算
    AND,
    OR,
    XOR,
    NOT
} opcode;
typedef enum
{
    ZF,
    SF
    // OF,CFは今回除外
} flag;

const char *flag_names[] = {
    "ZF", "SF"};

typedef struct Instruction
{
    opcode opcode;
    reg_id dest;
    reg_id src1;
    reg_id src2;
    int value;
} inst;
void results_display(cpu *my_cpu, int reg_length, int flag_length)
{
    printf("[結果]\n");
    for (int i = 0; i < reg_length; i++)
    {
        printf("%s:%d\n", reg_names[i], my_cpu->reg[i]);
    }
    for (int i = 0; i < flag_length; i++)
    {
        printf("%s:%d\n", flag_names[i], my_cpu->flag[i]);
    }
}
void instruction_execution(cpu *ptr_my_cpu, inst my_inst[], int inst_length, int reg_length, int flag_length)
{
    while (ptr_my_cpu->pc < inst_length)
    {
        inst current = my_inst[ptr_my_cpu->pc];
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
            if (current.value < 1 || current.value > inst_length)
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
            for (int i = 0; i < flag_length; i++)
            {
                ptr_my_cpu->flag[i] = 0;
            }
            int CMP_result = ptr_my_cpu->reg[current.src1] - ptr_my_cpu->reg[current.src2];
            if (CMP_result == 0)
            {
                ptr_my_cpu->flag[ZF] = 1;
            }
            if (CMP_result < 0)
            {
                ptr_my_cpu->flag[SF] = 1;
            }
            printf("CMP %s,%s\n", reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        case JE:
        {
            if (current.value < 1 || current.value > inst_length)
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
            if (current.value < 1 || current.value > inst_length)
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
            if (current.value < 1 || current.value > inst_length)
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
            if (current.value < 1 || current.value > inst_length)
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
            if (current.value < 1 || current.value > inst_length)
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
            if (current.value < 1 || current.value > inst_length)
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
        results_display(ptr_my_cpu, reg_length, flag_length);
        ptr_my_cpu->pc++;
    }
}

void draw_cpu(cpu *before_my_cpu, cpu *after_my_cpu, int reg_length, int flag_length)
{
    printf("\n");
    printf("      before          after      \n");
    printf("+---------------+---------------+\n");
    printf("|      CPU      |      CPU      |\n");
    printf("+---------------+---------------+\n");
    for (int i = 0; i < reg_length; i++)
    {
        // printf("|               |               |\n");
        printf("|  %-5s:%5d  |  %-5s:%5d  |\n", reg_names[i], before_my_cpu->reg[i], reg_names[i], after_my_cpu->reg[i]);
    }
    printf("+---------------+---------------+\n");
    printf("|     FLAGS     |     FLAGS     |\n");
    for (int i = 0; i < flag_length; i++)
    {
        printf("|  %-5s:%5d  |  %-5s:%5d  |\n", flag_names[i], before_my_cpu->flag[i], flag_names[i], after_my_cpu->flag[i]);
    }

    printf("+---------------+---------------+\n");
    printf("\n");
}
void memory_cpu(cpu *ptr_my_cpu, cpu *ptr_memo_cpu)
{
    *ptr_memo_cpu = *ptr_my_cpu;
}

int main(void)
{
    cpu my_cpu = {0};
    cpu *ptr_my_cpu = &my_cpu;

    cpu memo_cpu = {0};
    cpu *ptr_memo_cpu = &memo_cpu;

    inst my_inst[] = {
        {.opcode = IMM, .dest = eax, .value = 5},               // 1
        {.opcode = IMM, .dest = ebx, .value = 1},               // 2
        {.opcode = SUB, .dest = eax, .src1 = eax, .src2 = ebx}, // 3
        {.opcode = CMP, .src1 = eax, .src2 = ebx},              // 4
        {.opcode = JG, .value = 3},                             // 5
    };

    int inst_length = sizeof(my_inst) / sizeof(my_inst[0]);
    int reg_length = sizeof(reg_names) / sizeof(reg_names[0]);
    int flag_length = sizeof(my_cpu.flag) / sizeof(my_cpu.flag[0]);
    memory_cpu(ptr_my_cpu, ptr_memo_cpu);
    instruction_execution(ptr_my_cpu, my_inst, inst_length, reg_length, flag_length);
    draw_cpu(ptr_memo_cpu, ptr_my_cpu, reg_length, flag_length);
}

// reg_lengthの数を一括管理,論理演算途中,そろそろasmファイルの読み取り