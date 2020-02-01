#pragma once
#include <stdio.h>

enum STATUS { FREE, IN_USE, RESV };

typedef short reg_index;

extern enum STATUS registers[20];
extern int label;

int getLabel();

// Get a register
reg_index getReg();

// free the last used register
void freeReg();

// Push all the registers which are in use to stack
void pushToStack(FILE*);

// Get back all the registers from the stack
void getFromStack(FILE*);
