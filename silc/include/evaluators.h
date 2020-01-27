#pragma once
#include <stdio.h>
#include "node.h"
#include "register.h"
#include <stdlib.h>

extern int loop_start, loop_end;

struct loop_llist{
    int start_label;
    int end_label;
    struct loop_llist *next, *prev;
};

struct loop_llist* createLlistNode(int, int, struct loop_llist*);

struct loop_llist* deleteLlistNode(struct loop_llist*);

void eval_tree(tnode*, FILE*);
reg_index eval_expr(tnode*, FILE*);
void eval_read(tnode*, FILE*);
void eval_write(tnode*, FILE*);
void eval_assgn(tnode*, FILE*);
void eval_if(tnode*, FILE*);
void eval_while(tnode*, FILE*);
reg_index eval_func(tnode*, FILE*);
void eval_break(FILE*);
void eval_cont(FILE*);
