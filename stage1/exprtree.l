%{ 
	#include <stdlib.h>
	#include <stdio.h>
	#include "y.tab.h"

	int number;
%}

%%
[0-9]+	{number = atoi(yytext); yylval.no = createNode(NUM, NULL, number, NULL, NULL); return NUM;}
"+"	{return PLUS;}
"-"	{return MINUS;}
"*"	{return MUL;}
"/"	{return DIV;}
"=" {return EQUALS}

[ \t]	{}
[()]	{return *yytext;}
"end" {return END;}
.	{yyerror("unknown character\n");exit(1);}
[a-z] {return ID;}

";" {return SEMI;}
"begin" {return BEGIN;}
"read" {return READ;}
"write" {return WRITE;}
"end" {return END;}

%%

int yywrap(void) {
	return 1;
}

