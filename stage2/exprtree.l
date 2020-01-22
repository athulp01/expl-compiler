%{ 
    #include <stdlib.h>
	#include <stdio.h>
	#include "y.tab.h"
	#include  "node.h"
	int number;
	char c;
%}

%%
[0-9]+	{number = atoi(yytext); yylval.no = createNode(NUM, '\0', number, NULL, NULL); return _NUM;}
"+"	{return _PLUS;}
"-"	{return _MINUS;}
"*"	{return _MUL;}
"/"	{return _DIV;}
"=" {return _EQUALS;}

[\n \t]	{}
[()]	{return *yytext;}
"end" {return _END;}
[a-z] {c = yytext[0]; yylval.no = createNode(VAR, c, -1, NULL, NULL); return _ID;}

";" {return _SEMI;}
"begin" {return _BEGIN;}
"read" {return _READ;}
"write" {return _WRITE;}

%%

int yywrap(void) {
	return 1;
}

