%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "evaluators.h"
    #include "node.h"
	int yylex(void);
    FILE *out, *yyin;
%}

%union{
	struct tnode *no;
	
}
%type <no> expr _NUM _END read write stmt stmtList assgn _ID func ifstmt whilestmt break cont
%token _IF _WHILE _THEN _ELSE _ENDIF _ENDWHILE _DO _BREAK _CONT
%token _LT _GT _EQ _NE _LE _GE 
%token _PLUS _MINUS _MUL _DIV _END _BEGIN _READ _WRITE _SEMI _EQUALS _Q 
%token _ID _NUM
%left _PLUS _MINUS
%left _MUL _DIV
%nonassoc _LT _GT _EQ _NE _LE _GE

%%
program: _BEGIN stmtList _END _SEMI {eval_tree($2, out);}
       ;

expr : expr _PLUS expr		{$$ = createNode(OP, "+", -1, $1, $3);}
     | expr _MINUS expr  	{$$ = createNode(OP, "-", -1, $1, $3);}
	 | expr _MUL expr	    {$$ = createNode(OP, "*", -1, $1, $3);}
	 | expr _DIV expr	    {$$ = createNode(OP, "/", -1, $1, $3);}
     | expr _LE expr        {$$ = createNode(OP, "<=", -1, $1, $3);}
     | expr _GE expr        {$$ = createNode(OP, ">=", -1, $1, $3);}
     | expr _EQ expr        {$$ = createNode(OP, "==", -1, $1, $3);}
     | expr _LT expr        {$$ = createNode(OP, "<", -1, $1, $3);}
     | expr _GT expr        {$$ = createNode(OP, ">", -1, $1, $3);}
     | expr _NE expr        {$$ = createNode(OP, "!=", -1, $1, $3);}
	 | '(' expr ')'		    {$$ = $2;}
	 | _NUM			        {$$ = $1;}
     | _ID                  {$$ = $1;}
     | func                 {$$ = $1;}
	 ;

func : _Q '(' expr ')'       {$$ = createNode(Q, "", -1, $3, NULL);}  
     ;

read : _READ '(' _ID ')' _SEMI      {$$ = createNode(READ, "", -1, $3, NULL);}
     ;

write : _WRITE '(' expr ')' _SEMI   {$$ = createNode(WRITE, "", -1, $3, NULL);}
      ;

assgn : _ID _EQUALS expr _SEMI     {$$ = createNode(ASSN, "", -1, $1, $3);}
      ;

ifstmt : _IF '(' expr ')' _THEN stmtList _ELSE stmtList _ENDIF _SEMI  {tnode *tmp = createNode(IF_BODY, "", -1, $6, $8);
                                                                 $$ = createNode(IF, "", -1, $3, tmp);}
       | _IF '(' expr ')' _THEN stmtList _ENDIF _SEMI                {tnode *tmp = createNode(IF_BODY, "", -1, $6, NULL);
                                                                 $$ = createNode(IF, "", -1, $3, tmp);}
       ;
break : _BREAK _SEMI             {$$ = createNode(BREAK, "", -1, NULL, NULL);}
      ;

cont :  _CONT _SEMI             {$$ = createNode(CONT, "", -1, NULL, NULL);}
     ;   

whilestmt : _WHILE '(' expr ')' _DO stmtList _ENDWHILE _SEMI         {$$ = createNode(WHILE, "", -1, $3, $6);}
          ;

stmt : read                     {$$ = $1;}
     | write                    {$$ = $1;}
     | assgn                    {$$ = $1;}
     | func                     {$$ = $1;}
     | ifstmt                   {$$ = $1;}
     | whilestmt                {$$ = $1;}
     | break                    {$$ = $1;}
     | cont                     {$$ = $1;}
     ;
stmtList : stmtList stmt        {$$ = connect($1, $2);}
         | stmt                 {$$ = $1;}
         ;

%%

int yyerror(char const *s)
{
    printf("yyerror %s",s);
    exit(0);
}


int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Usage: ./silc <source_file>\n");
        exit(1);
    }
    yyin = fopen(argv[1], "r");
    if(yyin == NULL) yyerror("Input file not found");
	out = fopen("./out.xsm", "w");
    fprintf(out, "0\n2056\n0\n0\n0\n0\n0\n0\nMOV SP, 4200\n");
    if(out == NULL) yyerror("file");
	yyparse();
    fprintf(out, "MOV R19, \"Exit\"\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nCALL 0");
	return 0;
}
