#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "datastructures.h"


extern int loop_start, loop_end;
extern struct GSymbol* globalSym;



void eval_tree(tnode*, Frame*, FILE*);
reg_index eval_expr(tnode*, Frame*, FILE*);
void eval_read(tnode*, Frame*, FILE*);
void eval_write(tnode*,Frame*, FILE*);
void eval_assgn(tnode*,Frame*, FILE*);
void eval_if(tnode*,Frame*, FILE*);
void eval_while(tnode*,Frame*, FILE*);
void eval_break(FILE*);
void eval_cont(FILE*);
void eval_func(tnode* root, FILE* out);
reg_index eval_alloc(int, Frame*, FILE*);
void eval_init(Frame*, FILE*);
reg_index call_func(tnode* root, Frame *frame, FILE *out);
void popArgFromStack(tnode *root, Frame*, FILE *out) ;
void eval_return(tnode *root, Frame *frame, FILE *out);
void pushArgToStack(tnode* root, Frame* frame, FILE* out);
int addArgSymbol(tnode* root, Frame* frame, int mem, FILE* out);
reg_index getAddress(tnode*, Frame *frame, FILE *out);
void eval_free(tnode *root, Frame *frame, FILE *out);
