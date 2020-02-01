#include "register.h"

#include <stdlib.h>

enum STATUS registers[20];
int label = 0;

int getLabel() { return label++; }

reg_index getReg() {
  for (reg_index i = 0; i < 20; i++) {
    if (registers[i] == FREE) {
      registers[i] = IN_USE;
      return i;
    }
  }
  printf("All the registers are occupied\n");
  exit(1);
  return 1;
}

void freeReg(FILE *out) {
  for (reg_index i = 19; i >= 0; i--)
    if (registers[i] == IN_USE) {
      registers[i] = FREE;
      return;
    }
}

void pushToStack(FILE *out) {
  for (reg_index i = 0; i < 20; i++) {
    if (registers[i] == IN_USE) {
      fprintf(out, "PUSH R%d\n", i);
      registers[i] = RESV;
    }
  }
}

void getFromStack(FILE *out) {
  for (reg_index i = 19; i >= 0; i--) {
    if (registers[i] == RESV) {
      fprintf(out, "POP R%d\n", i);
      registers[i] = IN_USE;
    }
  }
}