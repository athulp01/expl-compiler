#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "register.h"

extern int loop_start, loop_end;
extern struct symbolList* globalSym;

struct labelList {
  int start_label;
  int end_label;
  struct labelList *next, *prev;
};

struct symbolList {
  char* name;
  enum VARTYPE type;
  int size;
  int binding;
  struct symbolList* next;
};

struct varList {
  char* name;
  int size;
  struct varList* next;
};

struct symbolList* createSlistNode(char*, enum VARTYPE, int,
                                   struct symbolList*);
struct symbolList* searchSlist(char*, struct symbolList*);

struct labelList* createLlistNode(int, int, struct labelList*);
struct labelList* deleteLlistNode(struct labelList*);

struct varList* createVlistNode(char*, int, struct varList*);

void eval_tree(tnode*, FILE*);
reg_index eval_expr(tnode*, FILE*);
void eval_read(tnode*, FILE*);
void eval_write(tnode*, FILE*);
void eval_assgn(tnode*, FILE*);
void eval_if(tnode*, FILE*);
void eval_while(tnode*, FILE*);
reg_index eval_qfunc(tnode*, FILE*);
void eval_break(FILE*);
void eval_cont(FILE*);
