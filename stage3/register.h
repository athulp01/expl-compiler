#pragma once
#include<stdio.h>

#define REG(x) ((int)x + 4096 - (int)'a')

enum STATUS{IN_USE, FREE, RESV};

typedef short reg_index;

extern enum STATUS registers[20];

//Get a register
reg_index getReg();

//free the last used register
void freeReg();

//Push all the registers which are in use to stack
void pushToStack(FILE*);

//Get back all the registers from the stack
void getFromStack(FILE*);
