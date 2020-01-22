#pragma once
#define reg(x) int(x) + 4096

typedef short reg_index;

extern reg_index registers[20];

reg_index getReg();

void freeReg();

