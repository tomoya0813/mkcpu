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
} cpu;

typedef enum
{
    IMM,
    MOV,
    ADD,
    SUB,
    MUL,
    DIV
} opcode;

typedef struct Instruction
{
    opcode opcode;
    reg_id dest;
    reg_id src1;
    reg_id src2;
    int value;
} inst;
void results_display(cpu *my_cpu, int reg_length)
{
    printf("[結果]\n");
    for (int i = 0; i < reg_length; i++)
    {
        printf("%s:%d\n", reg_names[i], my_cpu->reg[i]);
    }
}
void instruction_execution(cpu *my_cpu, inst my_inst[], int inst_length, int reg_length)
{
    while (my_cpu->pc < inst_length)
    {
        inst current = my_inst[my_cpu->pc];
        printf("<命令%d>\n", my_cpu->pc + 1);
        switch (current.opcode)
        {
        case IMM:
            my_cpu->reg[current.dest] = current.value;
            printf("IMM %s,%d\n", reg_names[current.dest], current.value);
            break;
        case MOV:
            my_cpu->reg[current.dest] = my_cpu->reg[current.src1];
            printf("MOV %s,%s\n", reg_names[current.dest], reg_names[current.src1]);
            break;
        case ADD:
            my_cpu->reg[current.dest] = my_cpu->reg[current.src1] + my_cpu->reg[current.src2];
            printf("ADD %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        case SUB:
            my_cpu->reg[current.dest] = my_cpu->reg[current.src1] - my_cpu->reg[current.src2];
            printf("SUB %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        case MUL:
            my_cpu->reg[current.dest] = my_cpu->reg[current.src1] * my_cpu->reg[current.src2];
            printf("MUL %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        case DIV:
            if (my_cpu->reg[current.src2] == 0)
            {
                printf("<命令%d>がエラーです\n", my_cpu->pc + 1);
                break;
            }
            my_cpu->reg[current.dest] = my_cpu->reg[current.src1] / my_cpu->reg[current.src2];
            printf("DIV %s,%s,%s\n", reg_names[current.dest], reg_names[current.src1], reg_names[current.src2]);
            break;
        }
        results_display(my_cpu, reg_length);
        my_cpu->pc++;
    }
}

int main(void)
{
    cpu my_cpu = {0};
    cpu *ptr_my_cpu = &my_cpu;

    inst my_inst[3] = {
        {.opcode = IMM, .dest = eax, .value = 100},
        {.opcode = IMM, .dest = ebx, .value = 200},
        {.opcode = ADD, .dest = ecx, .src1 = eax, .src2 = ebx}};

    int inst_length = sizeof(my_inst) / sizeof(my_inst[0]);
    int reg_length = sizeof(reg_names) / sizeof(reg_names[0]);

    instruction_execution(ptr_my_cpu, my_inst, inst_length, reg_length);
}

// reg_lengthの数を一括管理,jmp系命令実装予定.論理演算もできたら