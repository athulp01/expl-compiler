#pragma once
#include<stdio.h>
#include "exprtree.h"

typedef short reg_index;

extern reg_index registers[20];

reg_index getReg();

void freeReg();

reg_index codeGen(struct tnode*, FILE*);
