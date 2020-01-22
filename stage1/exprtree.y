%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "codegen.h"
	int yylex(void);
    FILE *out;
%}

%union{
	struct node *no = NULL;
	
}
%type <no> expr NUM program END
%token NUM PLUS MINUS MUL DIV END BEGIN READ WRITE ID SEMI EQUALS
%left PLUS MINUS
%left MUL DIV

%%

program : BEGIN stmt END{
                        $$ = $2;
                        reg_index res = codeGen($1, out);
                        fprintf(out, "MOV SP, 4097\n MOV [4096], R%d\n", res);
                        fprintf(out, "MOV R1, -2\nPUSH R1\n");
                        fprintf(out, "MOV R1, [4096]\nPUSH R1\n");
                        fprintf(out, "PUSH R5\nPUSH R5\nINT 7\nINT 10\n");
                        exit(1);
			}
		;

expr : expr PLUS expr		{$$ = createNode(OP, '+', -1, $1, $3);}
     | expr MINUS expr  	{$$ = createNode(OP, '-', -1, $1, $3);}
	 | expr MUL expr	    {$$ = createNode(OP, '*', -1, $1, $3);}
	 | expr DIV expr	    {$$ = createNode(OP, '/', -1, $1, $3);}
	 | '(' expr ')'		    {$$ = $2;}
	 | NUM			        {$$ = $1;}
	 ;

read : READ '(' ID ')' SEMI {$$ = createNode(READ, $3, -1, NULL, NULL);}
     ;

write : WRITE '(' expr ')' SEMI {$$ = createNode(WRITE, NULL, -1, $3, NULL);}
      | WRITE '(' ID ')' SEMI   {$$ = createNode(WRITE, $3, -1, NULL, NULL);}
      ;

assgn : ID EQUALS expr SEMI     {$$ = createNode(ASSN, $1, -1, $3, NULL);}
      ;

stmt : read                     {$$ = connect($$, $1);}
     | write                    {$$ = connect($$, $1);}
     | assgn                    {$$ = connect($$, $1);}
     ;

%%

yyerror(char const *s)
{
    printf("yyerror %s",s);
}


int main(void) {
	out = fopen("out.xsm", "w");
    fprintf(out, "0\n2056\n0\n0\n0\n0\n0\n0\n");
    if(out == NULL) yyerror("file");
	yyparse();
	return 0;
}
