#pragma once
enum TYPE{NUM, READ, WRITE, CONN};
typedef union _val {
    int num;
    char *name;
}val;

typedef struct _node { 
    enum TYPE type;
    val value;
	struct tnode *left,*right;	//left and right branches   
}node;

/*Create a node tnode*/
struct node* createNode(enum TYPE type, val value, 
                struct node *l = NULL, struct node *r = NULL);

struct node* connect(node* first, node* second);
