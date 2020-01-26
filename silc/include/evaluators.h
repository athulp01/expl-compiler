#pragma once
#include <stdio.h>
#include "node.h"
#include "register.h"

void eval_tree(tnode*, FILE*);
reg_index eval_expr(tnode*, FILE*);
void eval_read(tnode*, FILE*);
void eval_write(tnode*, FILE*);
void eval_assgn(tnode*, FILE*);
void eval_if(tnode*, FILE*);
void eval_while(tnode*, FILE*);
reg_index eval_func(tnode*, FILE*);
