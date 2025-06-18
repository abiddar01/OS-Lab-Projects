#include <stdio.h>
#include <stdbool.h>
#include "simple.h"

void initializeMemory(int memory[]);

void displayMemory(int memory[]);

void loadSMLProgram(int memory[], const char *fileName) ;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <SML program file>\n", argv[0]);
        return 1;
    }
    
    int memory[INDEXES];
    initializeMemory(memory);
    loadSMLProgram(memory, argv[1]);
    
    int counter = 0;
    int accumulator = 0;
    int instruction = 0;
    int opcode = 0;
    int operand = 0; 
    
    while (true)
    {
	   
    
        printf("\n---- InstructionCounter: %d | Accumulator: %d ----\n", counter, accumulator);
        printf("\n---- InstructionRegister: %d | OpCode: %d | Operand: %d----\n", instruction, opcode, operand);
        displayMemory(memory);
        
        instruction = memory[counter];
        opcode = instruction / 100;
        operand = instruction % 100; 
        
        switch (opcode)
        {
            case READ:
                printf("\nEnter a number: ");
                scanf("%d", &memory[operand]);
                break;
            case WRITE:
                printf("\nMemory[%d] = %d\n", operand, memory[operand]);
                break;
            case LOAD:
                accumulator = memory[operand];
                break;
            case STORE:
                memory[operand] = accumulator;
                break;
            case ADD:
                accumulator += memory[operand];
                break;
            case SUBTRACT:
                accumulator -= memory[operand];
                break;
            case MULTIPLY:
                accumulator *= memory[operand];
                break;
            case DIVIDE:
                if (memory[operand] != 0)
                    accumulator /= memory[operand];
                else
                    printf("\nError: Division by zero.\n");
                break;
            case BRANCH:
                counter = operand;
                continue;
            case BRANCHNEG:
                if (accumulator < 0)
                    counter = operand;
                continue;
            case BRANCHZERO:
                if (accumulator == 0)
                    counter = operand;
                continue;
            case HALT:
                printf("\nHALT! Program execution completed. \n");
                return 0;
            default:
                printf("\nInvalid operation code: %d\n", opcode);
                break;
        }
        
        counter++;
        if (counter >= INDEXES)
            break;
    }
    return 0;
}
