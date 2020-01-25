#pragma once
#include "node.h"
#include "register.h"

void parseSyntaxTree(struct tnode*, FILE*);
reg_index parseExprTree(struct tnode*, FILE*);
void test(tnode*);
    


