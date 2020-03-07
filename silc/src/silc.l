%option yylineno
%{ 
    #include <stdlib.h>
	#include <stdio.h>
	#include "datastructures.h"
	#include "silc.h"
	
	int number;
	char c;

%}

%%
[0-9]+	{number = atoi(yytext); yylval.no = createNode(CONST, "\0", number, NULL, NULL); return _NUM;}
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
"%"	{return _MOD;}

[ \t]	{}
[\n] 	{}
[()]	{return *yytext;}
[{}]	{return *yytext;}
[.]		{return *yytext;}
"[" {return *yytext;}
"]"	{return *yytext;}
"end" {return _END;}
";" {return _SEMI;}
"," {return _COMMA;}
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
"decl"	{return _DECL;}
"enddecl"	{return _ENDDECL;}
"main" {return _MAIN;}
"return" {return _RET;}
"AND"	{return _AND;}
"type" 	{return _TYPE;}
"endtype" {return _ENDTYPE;}
"initialize" {return _INIT;}
"alloc" {return _ALLOC;}
"null" {return _NULL;}
"free" {return _FREE;}
[a-zA-Z0-9]+ {yylval.name = strdup(yytext); return _ID;}
\"(\\.|[^"\\])*\" {yylval.no = createNode(CONST, strdup(yytext), -1, NULL, NULL); return _TEXT;}

%%

int yywrap(void) {
	return 1;
}

int yyerror(char const *s)
{
    printf("Line %d: | Syntax Error: %s \n",yylineno, s);
    exit(1);
}
