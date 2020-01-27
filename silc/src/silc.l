%{ 
    #include <stdlib.h>
	#include <stdio.h>
	#include "silc.h"
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
"<" {return _LT;}
">"	{return _GT;}
"<=" {return _LE;}
">=" {return _GE;}
"==" {return _EQ;}
"!=" {return _NE;}

[\n \t]	{}
[()]	{return *yytext;}
"end" {return _END;}
[a-z] {char *s = strdup(yytext); yylval.no = createNode(VAR, s, -1, NULL, NULL); return _ID;}

";" {return _SEMI;}
"begin" {return _BEGIN;}
"read" {return _READ;}
"write" {return _WRITE;}
"func"	{return _Q;}
"if"	{return _IF;}
"then"	{return _THEN;}
"do"	{return _DO;};
"endif"	{return _ENDIF;}
"while" {return _WHILE;}
"endwhile" {return _ENDWHILE;}
"else"	{return _ELSE;}
"break"	{return _BREAK;}
"continue"	{return _CONT;}

%%

int yywrap(void) {
	return 1;
}
