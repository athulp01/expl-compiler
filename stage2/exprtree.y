%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "codegen.h"
	int yylex(void);
    FILE *out;
%}

%union{
	struct tnode *no;
	
}
%type <no> expr _NUM _END read write stmt stmtList assgn _ID 
%token _NUM _PLUS _MINUS _MUL _DIV _END _BEGIN _READ _WRITE _SEMI _EQUALS
%token _ID
%left _PLUS _MINUS
%left _MUL _DIV

%%
program: _BEGIN stmtList _END _SEMI {parseSyntaxTree($2, out);}
       ;

expr : expr _PLUS expr		{$$ = createNode(OP, '+', -1, $1, $3);}
     | expr _MINUS expr  	{$$ = createNode(OP, '-', -1, $1, $3);}
	 | expr _MUL expr	    {$$ = createNode(OP, '*', -1, $1, $3);}
	 | expr _DIV expr	    {$$ = createNode(OP, '/', -1, $1, $3);}
	 | '(' expr ')'		    {$$ = $2;}
	 | _NUM			        {$$ = $1;}
     | _ID                   {$$ = $1;}
	 ;

read : _READ '(' _ID ')' _SEMI {$$ = createNode(READ, '\0', -1, $3, NULL);}
     ;

write : _WRITE '(' expr ')' _SEMI {$$ = createNode(WRITE, '\0', -1, $3, NULL);}
      ;

assgn : _ID _EQUALS expr _SEMI     {$$ = createNode(ASSN, '\0', -1, $1, $3);}
      ;

stmt : read                     {$$ = $1;}
     | write                    {$$ = $1;}
     | assgn                    {$$ = $1;}
     ;
stmtList : stmtList stmt        {$$ = connect($1, $2);}
         | stmt                 {$$ = $1;}
         ;

%%

int yyerror(char const *s)
{
    printf("yyerror %s",s);
}


int main(void) {
	out = fopen("out.xsm", "w");
    fprintf(out, "0\n2056\n0\n0\n0\n0\n0\n0\nMOV SP, 4200\n");
    if(out == NULL) yyerror("file");
	yyparse();
    fprintf(out, "MOV R19, \"Exit\"\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nCALL 0");
	return 0;
}
