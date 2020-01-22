#include "code.h"

struct node* createTree(enum TYPE type, val value, struct tnode *l, struct tnode *r) {
    struct tnode *tmp = (struct tnode*)malloc(sizeof(struct tnode));
    *tmp = {type, val, l, r};
    return tmp;
}


struct node* connect(node* first, node* second) {
    if(first == NULL) {
        struct node* tmp = (struct node*)malloc(sizeof(struct node));
        tmp->left = second;
        tmp->type = CONN;
        return tmp;
    }else if(!first->left && !first->right) {
        struct node* tmp = (struct node*)malloc(sizeof(struct node));
        tmp->left = first;
        tmp->right = second;
        tmp->type = CONN;
        return tmp;
    }else {
        first->right = second;
        return first;
    }
}
