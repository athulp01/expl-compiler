#include "codegen.h"
#include "node.h"
#include "register.h"

void test(tnode *root) {
    if(root == NULL) return;
    test(root->left);
    printf("%d", root->type);
    test(root->right);
}

reg_index parseExprTree(struct tnode *root, FILE *out) {
    if(root == NULL) return 0;
    if(!root->left && !root->right) {
        reg_index cur = getReg();
        if(root->type == NUM)
            fprintf(out, "MOV R%d, %d\n", cur, root->value.num);
        else if(root->type == VAR)
            fprintf(out, "MOV R%d, [%d]\n", cur, REG(root->value.name));
        return cur;
    }

    reg_index left = parseExprTree(root->left, out);
    reg_index right = parseExprTree(root->right, out);
    
    if(root->type == OP)
        switch(root->value.name) {
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

reg_index parseQ(struct tnode *root, FILE *out) {
    char *label = "L1";
    reg_index one = getReg();
    reg_index answer = getReg();
    reg_index tmp = getReg();
    reg_index input = getReg();
    fprintf(out, "MOV R%d, 1\nMOV R%d, -1\n", one, answer);
    fprintf(out, "MOV R%d, %d\n", input, root->left->value.num);
    fprintf(out, "%s:INR R%d\nDIV R%d, 2\n MOV R%d, R%d\n EQ R%d, R%d\n",label, answer, input, tmp, answer, tmp, one);
    fprintf(out, "JZ R%d, %s\n INR R%d\n",tmp, label, answer);
    return answer;
}

void parseSyntaxTree(struct tnode *root, FILE *out) {
    if(root == NULL) return;
    parseSyntaxTree(root->left, out);
    switch(root->type) {
        case READ:
            fprintf(out, "MOV R18, %d\n", REG(root->left->value.name));
            fprintf(out, "MOV R19, \"Read\"\nPUSH R19\nMOV R19, -1\nPUSH R19\nPUSH R18\nPUSH R19\nPUSH R19\n");
            fprintf(out, "CALL 0\nPOP R18\nPOP R19\nPOP R19\nPOP R19\nPOP R19\n");
            break;

        case WRITE:
            if(root->left->type == OP)
                fprintf(out, "MOV R18, R%d\n", parseExprTree(root->left, out));
            else if(root->left->type == VAR)
                fprintf(out, "MOV R18, [%d]\n", REG(root->left->value.name));
            else if(root->left->type == Q) 
                fprintf(out, "MOV R18, R%d\n", parseQ(root->left, out));
            else 
                fprintf(out, "MOV R18, %d\n", root->left->value.num);
            fprintf(out, "MOV R19, \"Write\"\nPUSH R19\nMOV R19, -2\nPUSH R19\nPUSH R18\nPUSH R19\nPUSH R19\n");
            fprintf(out, "CALL 0\nPOP R18\nPOP R19\nPOP R19\nPOP R19\nPOP R19\n");
            break;

        case ASSN:
            if(root->right->type == VAR) {
                fprintf(out, "MOV [%d], [%d]", REG(root->left->value.name), REG(root->right->value.name));
                return;
            }else if(root->right->type == OP) {
                fprintf(out, "MOV [%d], R%d\n",REG(root->left->value.name), parseExprTree(root->right, out));
                return;
            }else if(root->right->type == Q) {
                fprintf(out, "MOV [%d], R%d\n",REG(root->left->value.name), parseQ(root->right, out));
            }
            else {
                reg_index tmp = getReg();
                fprintf(out, "MOV R%d, %d\nMOV [%d], R%d\n",tmp,root->right->value.num, REG(root->left->value.name), tmp);
                freeReg();
                return;
            }
            break;


    }
    parseSyntaxTree(root->right, out);
    return;
}