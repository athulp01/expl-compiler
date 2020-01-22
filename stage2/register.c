#include "register.h"

reg_index registers[20];

reg_index getReg() {
    for(reg_index i=0; i<20; i++)
        if(!registers[i]) {
            registers[i] = 1;
            return i;
        }
}

void freeReg() {
    for(reg_index i=19; i>=0; i--) 
        if(registers[i]) {
            registers[i] = 0;
            return;
        }
}
