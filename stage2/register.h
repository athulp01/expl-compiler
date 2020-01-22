#pragma once
#include<stdio.h>
#define REG(x) ((int)x + 4096 - (int)'a')

typedef short reg_index;

extern reg_index registers[20];

reg_index getReg();

void freeReg();
