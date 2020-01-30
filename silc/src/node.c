#include "../include/node.h"
#include "../include/evaluators.h"
#include <stdlib.h>

int yyerror(char *);

tnode* createNode(enum TYPE type, char *s, int n,  struct tnode *l, struct tnode *r) {
    struct tnode *tmp = (struct tnode*)malloc(sizeof(struct tnode));
    tmp->type = type;
    tmp->varname = s;
    tmp->val = n;
    tmp->left = l;
    tmp->right = r;
    return tmp;
}


tnode* connect(tnode* first, tnode* second) {
    struct tnode* tmp = (struct tnode*)malloc(sizeof(struct tnode));
    tmp->type = CONN;
    tmp->left = first;
    tmp->right = second;
    return tmp;
}

//TODO: implement NULL checking
tnode* createVarNode(char *name, int s) {
    struct tnode* tmp = (struct tnode*)malloc(sizeof(struct tnode));
    tmp->type = VAR;
    tmp->val = s;
    tmp->varname = name;
    tmp->symbol = searchSlist(name, globalSym);
    if(tmp->symbol == NULL) {
        yyerror("Variable is not defined\n");
    }
    return tmp;
}


