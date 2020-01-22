#pragma once

enum TYPE{NUM, READ, WRITE, CONN, VAR, OP, ASSN};

typedef union val {
    int num;
    char name;
}val;

typedef struct tnode { 
    enum TYPE type;
    val value;
	struct tnode *left,*right;	//left and right branches   
}tnode;

/*Create a node tnode*/
struct tnode* createNode(enum TYPE type, char s, int n, 
            struct tnode *l, struct tnode *r);

struct tnode* connect(tnode* first, tnode* second);
