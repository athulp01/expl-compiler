#pragma once

enum TYPE{NUM, READ, WRITE, CONN, VAR, OP, ASSN, Q, IF, WHILE, IF_BODY};
enum VARTYPE{INT, BOOL};

typedef struct tnode { 
    enum TYPE type;
    enum VARTYPE vartype;
    char *varname;
    int val;
	struct tnode *left,*right;	//left and right branches   
}tnode;

/*Create a node tnode*/
tnode* createNode(enum TYPE type, char *s, int n, 
            struct tnode *l, struct tnode *r);

tnode* connect(tnode* first, tnode* second);
