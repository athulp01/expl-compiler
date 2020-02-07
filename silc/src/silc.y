%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "evaluators.h"
    #include <string.h>
    #include "datastructures.h"
	int yylex(void);
    FILE *out, *yyin;
    LinkedList *GSymList, *LSymList, *LVarList, *GVarList;
%}

%locations
%union{
	tnode *no;
    char *name;
    int type;
    LinkedList* list;
	
}
%type <no> expr _NUM _END read write stmt stmtList assgn ifstmt whilestmt break cont _TEXT body mainblock
%type <no> param paramlist fdef return arg args funccall funcstmt
%type <name> _ID 
%type <type> type
%type <list> gvarlist lvarlist ldeclblock ldecl gdeclblock gdecl

%token _DECL _ENDDECL _INT _STR _TEXT _MAIN _RET
%token _IF _WHILE _THEN _ELSE _ENDIF _ENDWHILE _DO _BREAK _CONT
%token _LT _GT _EQ _NE _LE _GE 
%token _PLUS _MINUS _MUL _DIV _END _BEGIN _READ _WRITE _SEMI _EQUALS _Q _COMMA _MOD
%token _ID _NUM

%nonassoc _LT _GT _EQ _NE _LE _GE _ID

%left _MOD
%left _PLUS _MINUS
%left _MUL _DIV



%%


 /*----------------------------------------------------------Program-----------------------------------------------------------*/

program : gdeclblock fdefblock mainblock
        | gdeclblock mainblock
        | mainblock
        ;

 /*-----------------------------------------------------------------------------------------------------------------------------*/


 /*--------------------------------------------------------Main-----------------------------------------------------------------*/

 mainblock : _INT _MAIN '(' ')' '{' ldeclblock body '}'      {tnode *node = createNode(FUNC, "main", -1, $7, NULL); $$->vartype = 0;
                                                             GSymbol* tmp = malloc(sizeof(GSymbol));
                                                             Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                             frame->Lvars = $6;
                                                             *tmp = (GSymbol){.name="main", .params=NULL, .frame=frame};
                                                             GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                             fprintf(out ,"main:\nMOV SP, 4300\nBRKP\n");
                                                             eval_func(node, out);
                                                            } 

 /*-----------------------------------------------------------------------------------------------------------------------------*/


 /*------------------------------------------------------Body-------------------------------------------------------------------*/

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
	 | _NUM			                                    {$$ = $1; $$->vartype = INT;}
     | _ID                                              {$$ = createNode(VAR, $1, -1, NULL, NULL);}
     | _ID '[' expr ']'                                 {$$ = createNode(VAR, $1, -1, $3, NULL);}
     | _TEXT                                            {$$ = $1; $$->vartype = STRING;}
     | funccall                                         {$$ = $1;}
	 ;

return : _RET expr _SEMI                                {$$ = createNode(RET, "\0", -1, $2, NULL);}
        ;


read : _READ '(' _ID ')' _SEMI                          {$$ = createNode(READ, "", -1, createNode(VAR, $3, -1, NULL, NULL), NULL);}
     | _READ '(' _ID '[' expr ']' ')' _SEMI             {$$ = createNode(READ, "", -1, createNode(VAR, $3, -1, $5, NULL), NULL);}
     ;
     

write : _WRITE '(' expr ')' _SEMI                       {$$ = createNode(WRITE, "", -1, $3, NULL);}
      ;

assgn : _ID _EQUALS expr _SEMI                          {$$ = createNode(ASSN, "", -1, createNode(VAR, $1, -1, NULL, NULL), $3);}
      | _ID '[' expr ']' _EQUALS expr _SEMI             {$$ = createNode(ASSN, "", -1, createNode(VAR, $1, -1, $3, NULL), $6);}
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
     | ifstmt                                           {$$ = $1;}
     | whilestmt                                        {$$ = $1;}
     | break                                            {$$ = $1;}
     | cont                                             {$$ = $1;}
     | return                                           {$$ = $1;}
     | funcstmt                                        {$$ = $1;}
     ;

stmtList : stmtList stmt                                {$$ = connect($1, $2);}
         | stmt                                         {$$ = $1;}
         ;

body : _BEGIN stmtList _END                             {$$ = $2;}
     ;

arg : expr                                              {$$ = $1;}
    |                                                   {$$ = NULL;}
    ;

args : args _COMMA arg                                  {$$ = connect($3, $1);}
     | arg                                              {$$ = $1;}
     ;
funccall : _ID '(' args ')'                      {$$ = createNode(FUNC, $1, -1, $3, NULL);}
         ;
funcstmt : funccall _SEMI                               { $$ = $1;}
         ;
        
 /*-------------$3->next = $1; $$ = $3----------------------------------------------------------------------------------------------------------------*/


 /*-----------------------------------------------------Types-------------------------------------------------------------------*/

type : _INT                                             {$$ = 0;}
     | _STR                                             {$$ = 2;}
     ;
 /*-----------------------------------------------------------------------------------------------------------------------------*/


 /*----------------------------------------------------------Local Declaration---------------------------------------------------*/


ldeclblock : _DECL ldecl  _ENDDECL                      {$$ = $2;}
            | _DECL _ENDDECL                            {$$ = NULL;}
            ;

 /*ldecllist : ldecllist ldecl                             { $2->next = }
         | ldecl                                        { $$ = $1;}
         ;
 */
        
ldecl : type lvarlist _SEMI                             {
                                                            LinkedList* gvars = $2;
                                                            $$ = NULL;
                                                            while(gvars) {
                                                                char *var = (char*)(gvars->data);
                                                                LSymbol* tmp = (LSymbol*)malloc(sizeof(LSymbol));
                                                                *tmp = (LSymbol){.name=var, .type=$1, .size=1};
                                                                $$ = addNode(tmp, sizeof(LSymbol), $$);
                                                                gvars = gvars->next;
                                                            }
                                                        }
     ;
lvarlist : lvarlist _COMMA _ID                          {$$ = addNode(strdup($3), sizeof($3), $1);}
        | _ID                                           {$$ = addNode(strdup($1), sizeof($1), NULL);}
        ;

 /*-----------------------------------------------------------------------------------------------------------------------------*/


 /*----------------------------------------------------------Global Declaration--------------------------------------------------*/

gdeclblock : _DECL gdecl _ENDDECL                       { $$ = $2;}
            | _DECL _ENDDECL                            { $$ = NULL;}
            ;

 /*gdecllist : gdecllist gdecl                             {}
         | gdecl                                        {}
         ;
 */
        
gdecl : type gvarlist _SEMI                             {
                                                            LinkedList* gvars = $2;
                                                            while(gvars) {
                                                                GVariable *var = (GVariable*)(gvars->data);
                                                                if(searchSymbol(var->name, GSymList)) {
                                                                    yyerror("Variable is already declared");
                                                                }
                                                                GSymbol* tmp = (GSymbol*)malloc(sizeof(GSymbol));
                                                                *tmp = (GSymbol){.name=var->name, .type=$1, .size=var->size, .params=var->params};
                                                                GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                                gvars = gvars->next;
                                                            }
                                                        }
     ;
gvarlist : gvarlist _COMMA _ID                          {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$3};
                                                         $$ = addNode(tmp, sizeof(GVariable), $1);
                                                        }

        | gvarlist _COMMA _ID '[' _NUM ']'              {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$3, .size=$5->val};
                                                         $$ = addNode(tmp, sizeof(GVariable), $1);
                                                        }

        | gvarlist _COMMA _ID '(' paramlist ')'         {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$3, .params=$5};
                                                         $$ = addNode(tmp, sizeof(GVariable), $1);
                                                        }

        | _ID '[' _NUM ']'                              {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$1, .size=$3->val};
                                                         $$ = addNode(tmp, sizeof(GVariable), NULL);
                                                        }

        | _ID                                           {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$1};
                                                         $$ = addNode(tmp, sizeof(GVariable), NULL);
                                                        }

        | _ID '(' paramlist ')'                         {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$1, .params=$3};
                                                         $$ = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
        ;

 /*-----------------------------------------------------------------------------------------------------------------------------*/

 /*----------------------------------------------------------Function Definition-------------------------------------------------*/

fdefblock : fdefblock fdef                              {fprintf(out, "%s:\n", $2->varname);
                                                         eval_func($2, out);}
          | fdef                                        {fprintf(out, "%s:\n", $1->varname);
                                                         eval_func($1, out);}                           
          ;

fdef : type _ID '(' paramlist ')' '{' ldeclblock body '}'   {$$ = createNode(FUNC, $2, -1, $8, $4); $$->vartype = $1;
                                                             GSymbol* tmp = malloc(sizeof(GSymbol));
                                                             Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                             frame->Lvars = $7;
                                                             *tmp = (GSymbol){.name=$2, .params=$4, .frame=frame};
                                                             GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                            }
     ;

paramlist : paramlist _COMMA param                      { $$ = connect($3, $1);}
          | param                                       { $$ = $1;}
          |                                             {$$ = NULL;}
          ;

param : type _ID                                        {$$ = createNode(VAR, $2, -1, NULL, NULL); $$->vartype = $1;}
      ;

 /*-----------------------------------------------------------------------------------------------------------------------------*/

%%




int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Usage: ./silc <source_file>\n");
        exit(1);
    }
    GSymList = NULL;
    LSymList = NULL;
    LVarList = NULL;
    GVarList = NULL;
    yyin = fopen(argv[1], "r");
    if(yyin == NULL) yyerror("Input file not found");
	out = fopen("./out.xsm", "w");
    fprintf(out, "0\nSTART\n0\n0\n0\n0\n0\n0\n");
    if(out == NULL) yyerror("file");
	yyparse();
    fprintf(out, "MOV R19, \"Exit\"\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nCALL 0");
	return 0;
}
