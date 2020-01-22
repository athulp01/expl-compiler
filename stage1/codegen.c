#include "codegen.h"

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

reg_index codeGen(struct tnode *root, FILE *out) {
    if(root == NULL) return 0;
    if(!root->left && !root->right) {
        reg_index cur = getReg();
        fprintf(out, "MOV R%d, %d\n", cur, root->val);
        return cur;
    }

    reg_index left = codeGen(root->left, out);
    reg_index right = codeGen(root->right, out);
    
    switch(*root->op) {
        case '+': fprintf(out, "ADD R%d, R%d\n",left, right);
                  break;
        
        case '-': fprintf(out, "SUB R%d, R%d\n",left, right);
                  break;

        case '*': fprintf(out, "MUL R%d, R%d\n",left, right);
                  break;
        case '/': fprintf(out, "DIV R%d, R%d\n",left, right);
                  break;
    }
    freeReg();
    return left;
}
