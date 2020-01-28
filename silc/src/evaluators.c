#include "node.h"
#include "register.h"
#include "evaluators.h"
#include "string.h"

struct loop_llist *llist;

struct loop_llist* createLlistNode(int s, int e, struct loop_llist* root) {
    struct loop_llist *tmp = (struct loop_llist*)malloc(sizeof(struct loop_llist));
    tmp->start_label = s;
    tmp->end_label = e;
    tmp->next = root;
    if(root) root->prev = tmp;
    return tmp;
}

struct loop_llist* deleteLlistNode(struct loop_llist* root) {
    if(root->next) {
        root->next->prev = NULL;
        return root->next;
    }else return NULL;
}

void eval_tree(tnode* root, FILE *out) {
    if(root == NULL) return;
    eval_tree(root->left, out);
    switch(root->type) {
        case READ:
            eval_read(root, out);
            break;
        case WRITE:
            eval_write(root, out);
            break;
        case ASSN:
            eval_assgn(root, out);
            break;
        case IF:
            eval_if(root, out);
            return;
        case WHILE:
            eval_while(root, out);
            return;
        case BREAK:
            eval_break(out);
            break;
        case CONT:
            eval_cont(out);
            break;
    }
    eval_tree(root->right, out);
    return;
}

reg_index eval_expr(struct tnode *root, FILE *out) {
    if(root == NULL) return 0;
    
    if(!root->left && !root->right) {
        reg_index cur = getReg();
        if(root->type == NUM)
            fprintf(out, "MOV R%d, %d\n", cur, root->val);
        else if(root->type == VAR)
            fprintf(out, "MOV R%d, [%d]\n", cur, REG(root->varname));
        return cur;
    }

    
    reg_index left = eval_expr(root->left, out);
    reg_index right = eval_expr(root->right, out);
    
    if(root->type == OP) {
        if(root->right && root->left)
        if(!root->left->vartype == NUM || !root->right->vartype == NUM) {
            yyerror("Type mismatch in expression\n");
        }    
        if(!strcmp(root->varname,"+")) {
            fprintf(out, "ADD R%d, R%d\n",left, right);
            root->vartype = INT;
        }else if(!strcmp(root->varname,"-")) {
            fprintf(out, "SUB R%d, R%d\n",left, right);
            root->vartype = INT;
        } else if(!strcmp(root->varname,"*")) {
            fprintf(out, "MUL R%d, R%d\n",left, right);
            root->vartype = INT;
        }else if(!strcmp(root->varname,"/")) {
            fprintf(out, "DIV R%d, R%d\n",left, right);
            root->vartype = INT;
        }else if(!strcmp(root->varname,"<")) {
            fprintf(out, "LT R%d, R%d\n",left, right);
            root->vartype = INT;
        }else if(!strcmp(root->varname, ">")) {
            fprintf(out, "GT R%d, R%d\n", left, right);
            root->vartype = BOOL;
        }else if(!strcmp(root->varname, "<=")) {
            fprintf(out, "LE R%d, R%d\n", left, right);
            root->vartype = BOOL;
        }else if(!strcmp(root->varname, ">=")) {
            fprintf(out, "GE R%d, R%d\n", left, right);
            root->vartype = BOOL;
        }else if(!strcmp(root->varname, "==")) {
            fprintf(out, "EQ R%d, R%d\n", left, right);
            root->vartype = BOOL;
        }else if(!strcmp(root->varname, "!=")) {
            fprintf(out, "NE R%d, R%d\n", left, right);
            root->vartype = BOOL;
        }else if(!strcmp(root->varname, "Q")) {
            root->vartype = INT;
            return eval_qfunc(root, out);
        }
    }
    freeReg();
    return left;
}

void eval_write(tnode *root, FILE *out) {
    pushToStack(out);
    reg_index writereg = getReg(), tmp = getReg();
    if(root->left->type == OP) {
        fprintf(out, "MOV R%d, R%d\n",writereg, eval_expr(root->left, out));
        freeReg();
    }
    else if(root->left->type == VAR)
        fprintf(out, "MOV R%d, [%d]\n", writereg, REG(root->left->varname));
    else if(root->left->type == NUM)
        fprintf(out, "MOV R%d, %d\n", writereg, root->left->val);
    fprintf(out, "MOV R%d, \"Write\"\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\n", tmp, tmp, tmp, tmp);
    fprintf(out, "PUSH R%d\nPUSH R%d\nPUSH R%d\n", writereg, tmp, tmp);
    fprintf(out, "CALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", writereg, tmp, tmp, tmp, tmp);
    freeReg();
    freeReg();
    getFromStack(out);
}

void eval_read(tnode* root, FILE *out) {
    pushToStack(out);
    reg_index mem = getReg();
    reg_index comm = getReg();
    fprintf(out, "MOV R%d, %d\n",mem, REG(root->left->varname));
    fprintf(out, "MOV R%d, \"Read\"\nPUSH R%d\nMOV R%d, -1\n", comm, comm,comm);
    fprintf(out, "PUSH R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\n", comm, mem, comm, comm);
    fprintf(out, "CALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", mem, comm, comm, comm, comm);
    freeReg();
    freeReg();
    getFromStack(out);
}

void eval_assgn(tnode *root, FILE *out) {
    if(root->right->type == VAR) {
        fprintf(out, "MOV [%d], [%d]\n", REG(root->left->varname), REG(root->right->varname));
        return;
    }else if(root->right->type == OP) {
        fprintf(out, "MOV [%d], R%d\n",REG(root->left->varname), eval_expr(root->right, out));
        freeReg();
        return;
    }else {
        reg_index tmp = getReg();
        fprintf(out, "MOV R%d, %d\nMOV [%d], R%d\n",tmp,root->right->val, REG(root->left->varname), tmp);
        freeReg();
        return;
    }
}
void eval_if(tnode *root, FILE *out) {
    reg_index res = eval_expr(root->left, out);
    if(root->right->right) {
        int start = getLabel(), end = getLabel();
        fprintf(out, "JZ R%d, L%d\n", res, start);
        eval_tree(root->right->left, out);
        fprintf(out, "JMP L%d\n", end);
        fprintf(out, "L%d:\n", start);
        eval_tree(root->right->right, out);
        fprintf(out, "L%d:\n", end);
    }else {
        int end = getLabel();
        fprintf(out, "JZ R%d, L%d\n", res, end);
        eval_tree(root->right->left, out);
        fprintf(out, "L%d:\n", end);
    }

}

void eval_while(tnode *root,FILE *out) {
    int loop_start = getLabel(), loop_end = getLabel();
    llist = createLlistNode(loop_start, loop_end, llist);
    fprintf(out, "L%d:\n", loop_start);
    reg_index res = eval_expr(root->left, out);
    fprintf(out,"JZ R%d, L%d\n", res, loop_end);
    eval_tree(root->right, out);
    fprintf(out, "JMP L%d\n", loop_start);
    fprintf(out, "L%d:\n", loop_end);
    llist = deleteLlistNode(llist);
    freeReg();
}

reg_index eval_qfunc(struct tnode *root, FILE *out) {
    char *label = "L1";
    reg_index answer = getReg();
    reg_index one = getReg();
    reg_index tmp = getReg();
    reg_index input = getReg();
    fprintf(out, "MOV R%d, 1\nMOV R%d, -1\n", one, answer);
    fprintf(out, "MOV R%d, %d\n", input, root->left->val);
    fprintf(out, "%s:\nINR R%d\nDIV R%d, 2\nMOV R%d, R%d\nEQ R%d, R%d\n",label, answer, input, tmp, input, tmp, one);
    fprintf(out, "JZ R%d, %s\nINR R%d\n",tmp, label, answer);
    freeReg();
    freeReg();
    freeReg();
    return answer;
}

void eval_break(FILE *out) {
    if(llist) {
        fprintf(out, "JMP L%d\n", llist->end_label);
        deleteLlistNode(llist);
    }
}

void eval_cont(FILE *out) {
    if(llist) {
        fprintf(out, "JMP L%d\n", llist->start_label);
        deleteLlistNode(llist);
    }
}

