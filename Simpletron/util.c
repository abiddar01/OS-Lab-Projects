#include <stdio.h>
#include "simple.h"

void initializeMemory(int memory[])
{
    for (int i = 0; i < INDEXES; i++)
    {
        memory[i] = 0;
    }
}

void displayMemory(int memory[])
{
    printf("\n    ");
    for (int j = 0; j < 10; j++)
    {
        printf(" %4d", j);
    }
    printf("\n    -------------------------------------------");

    for (int i = 0; i < INDEXES; i++)
    {
        if (i % 10 == 0)
        {
            printf("\n%2d |", i/10);
        }
        printf(" %04d", memory[i]);
    }
    printf("\n");
}

void loadSMLProgram(int memory[], const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    int value;
    for (int i = 0; i < INDEXES && fscanf(file, "%4d", &value) == 1; i++)
    {
        memory[i] = value;
    }
    
    fclose(file);
}
