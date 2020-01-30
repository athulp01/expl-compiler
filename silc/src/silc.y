%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "evaluators.h"
    #include "node.h"
	int yylex(void);
    FILE *out, *yyin;
    struct symbolList *globalSym;
%}

%union{
	struct tnode *no;
    char *name;
    int type;
    struct varList *var;
	
}
%type <no> expr _NUM _END read write stmt stmtList assgn func ifstmt whilestmt break cont
%type <name> _ID 
%type <type> type
%type <var> varlist

%token _DECL _ENDDECL _INT _STR
%token _IF _WHILE _THEN _ELSE _ENDIF _ENDWHILE _DO _BREAK _CONT
%token _LT _GT _EQ _NE _LE _GE 
%token _PLUS _MINUS _MUL _DIV _END _BEGIN _READ _WRITE _SEMI _EQUALS _Q _COMMA _MOD
%token _ID _NUM

%nonassoc _LT _GT _EQ _NE _LE _GE _ID

%left _MOD
%left _PLUS _MINUS
%left _MUL _DIV



%%
program: _BEGIN stmtList _END _SEMI {eval_tree($2, out);}
       ;

expr : expr _PLUS expr		                            {$$ = createNode(OP, "+", -1, $1, $3);}
     | expr _MINUS expr  	                            {$$ = createNode(OP, "-", -1, $1, $3);}
	 | expr _MUL expr	                                {$$ = createNode(OP, "*", -1, $1, $3);}
	 | expr _DIV expr	                                {$$ = createNode(OP, "/", -1, $1, $3);}
     | expr _LE expr                                    {$$ = createNode(OP, "<=", -1, $1, $3);}
     | expr _GE expr                                    {$$ = createNode(OP, ">=", -1, $1, $3);}
     | expr _EQ expr                                    {$$ = createNode(OP, "==", -1, $1, $3);}
     | expr _LT expr                                    {$$ = createNode(OP, "<", -1, $1, $3);}
     | expr _GT expr                                    {$$ = createNode(OP, ">", -1, $1, $3);}
     | expr _NE expr                                    {$$ = createNode(OP, "!=", -1, $1, $3);}
     | expr _MOD expr                                   {$$ = createNode(OP, "%", -1, $1, $3);}
	 | '(' expr ')'		                                {$$ = $2;}
	 | _NUM			                                    {$$ = $1;}
     | _ID                                              {$$ = createVarNode($1, 0);}
     | func                                             {$$ = $1;}
     | _ID '[' _NUM ']'                                 {$$ = createVarNode($1, $3->val);}
	 ;

func : _Q '(' expr ')'                                  {$$ = createNode(OP, "Q", -1, $3, NULL);}  
     ;

read : _READ '(' _ID ')' _SEMI                          {$$ = createNode(READ, "", -1, createVarNode($3, 0), NULL);}
     | _READ '(' _ID '[' _NUM ']' ')' _SEMI             {$$ = createNode(READ, "", -1, createVarNode($3, $5->val), NULL);}
     

write : _WRITE '(' expr ')' _SEMI                       {$$ = createNode(WRITE, "", -1, $3, NULL);}
      ;

assgn : _ID _EQUALS expr _SEMI                          {$$ = createNode(ASSN, "", -1, createVarNode($1, 1), $3);}
      ;

ifstmt : _IF '(' expr ')' _THEN stmtList _ELSE
         stmtList _ENDIF _SEMI                          {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, $6, $8);
                                                            $$ = createNode(IF, "", -1, $3, tmp);
                                                        }
       | _IF '(' expr ')' _THEN stmtList _ENDIF _SEMI   {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, $6, NULL);
                                                            $$ = createNode(IF, "", -1, $3, tmp);
                                                        }
       ;

break : _BREAK _SEMI                                    {$$ = createNode(BREAK, "", -1, NULL, NULL);}
      ;

cont :  _CONT _SEMI                                     {$$ = createNode(CONT, "", -1, NULL, NULL);}
     ;   

whilestmt : _WHILE '(' expr ')' _DO stmtList
             _ENDWHILE _SEMI                            {$$ = createNode(WHILE, "", -1, $3, $6);}
          ;

stmt : read                                             {$$ = $1;}
     | write                                            {$$ = $1;}
     | assgn                                            {$$ = $1;}
     | func                                             {$$ = $1;}
     | ifstmt                                           {$$ = $1;}
     | whilestmt                                        {$$ = $1;}
     | break                                            {$$ = $1;}
     | cont                                             {$$ = $1;}
     | declaration                                      {}
     ;

stmtList : stmtList stmt                                {$$ = connect($1, $2);}
         | stmt                                         {$$ = $1;}
         ;

declaration : _DECL decllist _ENDDECL                   {}
            | _DECL _ENDDECL                            {}
            ;

decllist : decllist decl                                {}
         | decl                                         {}
         ;
        
decl : type varlist _SEMI                               {
                                                            struct varList* tmp = $2;
                                                            while(tmp) {
                                                                globalSym = createSlistNode(tmp->name, tmp->size, $1, globalSym);
                                                                tmp = tmp->next;
                                                            }
                                                        }
     ;

type : _INT                                             {$$ = 0;}
     | _STR                                             {$$ = 2;}
     ;

varlist : varlist _COMMA _ID                            {$$ = createVlistNode($3,1, $1);}
        | varlist _COMMA _ID '[' _NUM ']'               {$$ = createVlistNode($3, $5->val, $1);}
        | _ID '[' _NUM ']'                              {$$ = createVlistNode($1, $3->val, NULL);}
        | _ID                                           {$$ = createVlistNode($1,1, NULL);}
        ;
%%

int yyerror(char const *s)
{
    printf("Syntax Error: %s",s);
    exit(1);
}


int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Usage: ./silc <source_file>\n");
        exit(1);
    }
    globalSym = NULL;
    yyin = fopen(argv[1], "r");
    if(yyin == NULL) yyerror("Input file not found");
	out = fopen("./out.xsm", "w");
    fprintf(out, "0\n2056\n0\n0\n0\n0\n0\n0\nMOV SP, 4200\n");
    if(out == NULL) yyerror("file");
	yyparse();
    fprintf(out, "MOV R19, \"Exit\"\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nCALL 0");
	return 0;
}
