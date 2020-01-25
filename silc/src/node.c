#include "node.h"
#include <stdlib.h>

struct tnode* createNode(enum TYPE type, char s, int n, struct tnode *l, struct tnode *r) {
    struct tnode *tmp = (struct tnode*)malloc(sizeof(struct tnode));
    tmp->type = type;
    if(s == '\0') {
        tmp->value.num = n;
    }else 
        tmp->value.name = s;
    tmp->left = l;
    tmp->right = r;
    return tmp;
}


struct tnode* connect(tnode* first, tnode* second) {
    struct tnode* tmp = (struct tnode*)malloc(sizeof(struct tnode));
    tmp->type = CONN;
    tmp->left = first;
    tmp->right = second;
    return tmp;
}
