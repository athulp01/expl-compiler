%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "evaluators.h"
    #include <string.h>
    #include "datastructures.h"
	int yylex(void);
    int yyerror(char*);
    FILE *out, *yyin;
    LinkedList *GSymList, *LSymList, *LVarList, *GVarList, *TypeList;
    int curMemory;

    void checkArg(tnode* sym, tnode* arg) {
        if(sym == NULL) {
            if(arg == NULL) return;
            else {
                yyerror("Argument mismatch");
            }
        }
        if(arg == NULL) yyerror("Argument mismatch");
        if(arg->vartype != sym->vartype) yyerror("Argument mismatch");
        if(arg->type == OP || arg->type == FUNC) return;
        checkArg(sym->left, arg->left);
        checkArg(sym->right, arg->right);
    }

    GSymbol* getSymbol(char *name) {

    }
%}

%locations

%union {
	tnode *no;
    char *name;
    char* type;
    LinkedList* list;
    Field* field;
	
}

%type <no> expr _NUM _END read write stmt stmtList assgn ifstmt whilestmt break cont _TEXT mainblock
%type <no> param paramlist fdef return arg args funccall funcstmt init alloc
%type <name> _ID type field
%type <list> gvarlist lvarlist ldecl gdeclblock gdecl ldecllist ldeclblock fieldlst
%type <field> fielddef

%token _DECL _ENDDECL _INT _STR _TEXT _MAIN _RET _TYPE _ENDTYPE _NULL
%token _IF _WHILE _THEN _ELSE _ENDIF _ENDWHILE _DO _BREAK _CONT _AND _INIT _ALLOC
%token _LT _GT _EQ _NE _LE _GE
%token _PLUS _MINUS _MUL _DIV _END _BEGIN _READ _WRITE _SEMI _EQUALS _Q _COMMA _MOD 
%token _ID _NUM

%nonassoc _LT _GT _EQ _NE _LE _GE _ID _AND

%left _MOD
%left _PLUS _MINUS
%left _MUL _DIV



%%

/*----------------------------------------------------------Program-----------------------------------------------------------*/

program : typedefblock gdeclblock fdefblock mainblock
        | typedefblock gdeclblock mainblock
        | gdeclblock fdefblock mainblock
        | typedefblock mainblock
        | gdeclblock mainblock
        | mainblock
        ;

 /*-----------------------------------------------------------------------------------------------------------------------------*/


 /*--------------------------------------------------------Main-----------------------------------------------------------------*/

 mainblock : _ID _MAIN '(' ')' '{' ldeclblock _BEGIN stmtList return  _END '}'      {tnode *node = createNode(FUNC, "main", -1, connect($8,$9), NULL); $$->vartype = 0;
                                                             GSymbol* tmp = malloc(sizeof(GSymbol));
                                                             Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                             frame->Lvars = $6;
                                                             *tmp = (GSymbol){.name="main", .params=NULL, .frame=frame};
                                                             GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                             fprintf(out ,"main:\nBRKP\n");
                                                             eval_func(node, out);
                                                            } 

 /*-----------------------------------------------------------------------------------------------------------------------------*/

 /*--------------------------------------------------------Type defintion-----------------------------------------------------------------*/

 typedefblock : typedefblock typedef                   {}
                | typedef                                {}
                ;

fielddef : type _ID _SEMI                                 { Field *field = (Field*)malloc(sizeof(Field));
                                                            *field = (Field){.name=$2, .type=searchType($1, TypeList)};
                                                            $$ = field;
                                                          }
      ;

fieldlst : fieldlst fielddef                               { $2->idx = ((Field*)$1->data)->idx + 1;
                                                             $$ = addNode($2, sizeof(Field), $1);}
         | fielddef                                        { $1->idx = 1; $$ = addNode($1, sizeof(Field), NULL); }
         ;

typedef : _TYPE _ID '{' fieldlst '}' _ENDTYPE             { Type *type = (Type*)malloc(sizeof(Type));
                                                            *type = (Type){.name=$2, .fields=$4};
                                                            LinkedList *field = type->fields;
                                                            int size = 0;
                                                            while(field) {
                                                                size++;
                                                                field = field->next;
                                                            }
                                                            type->size = size;
                                                             TypeList = addNode(type, sizeof(Type), TypeList);
                                                           }
     ;

field : _ID '.' _ID                                         {char *tmp = (char*)malloc(strlen($1) + strlen($3) + 1);
                                                            strcpy(tmp, $1); strcat(tmp, ".");strcat(tmp, $3); $$ = tmp;}
      | field '.' _ID                                       {char *tmp = (char*)malloc(strlen($1) + strlen($3) + 1);
                                                            strcpy(tmp, $1); strcat(tmp, ".");strcat(tmp, $3); $$ = tmp;}
      ;
 /*-----------------------------------------------------------------------------------------------------------------------------*/

 /*------------------------------------------------------Body-------------------------------------------------------------------*/

expr : expr _PLUS expr		                            {$$ = createNode(OP, "+", -1, $1, $3); 
                                                        $$->vartype = searchType("int", TypeList);}
                                                        
     | expr _MINUS expr  	                            {$$ = createNode(OP, "-", -1, $1, $3); 
                                                        $$->vartype = searchType("int", TypeList);}

	 | expr _MUL expr	                                {$$ = createNode(OP, "*", -1, $1, $3); 
                                                        $$->vartype = searchType("int", TypeList);}

	 | expr _DIV expr	                                {$$ = createNode(OP, "/", -1, $1, $3); 
                                                        $$->vartype = searchType("int", TypeList);}

     | expr _LE expr                                    {$$ = createNode(OP, "<=", -1, $1, $3); 
                                                        $$->vartype = searchType("bool", TypeList);}

     | expr _GE expr                                    {$$ = createNode(OP, ">=", -1, $1, $3);
                                                        $$->vartype = searchType("bool", TypeList);}

     | expr _EQ expr                                    {$$ = createNode(OP, "==", -1, $1, $3);
                                                        $$->vartype = searchType("bool", TypeList);}

     | expr _LT expr                                    {$$ = createNode(OP, "<", -1, $1, $3);
                                                        $$->vartype = searchType("bool", TypeList);}

     | expr _GT expr                                    {$$ = createNode(OP, ">", -1, $1, $3);
                                                        $$->vartype = searchType("bool", TypeList);}

     | expr _NE expr                                    {$$ = createNode(OP, "!=", -1, $1, $3);
                                                        $$->vartype = searchType("bool", TypeList);}

     | expr _AND expr                                   {$$ = createNode(OP, "AND", -1, $1, $3);
                                                        $$->vartype = searchType("bool", TypeList);}

     | expr _MOD expr                                   {$$ = createNode(OP, "%", -1, $1, $3);
                                                        $$->vartype = searchType("int", TypeList);}

	 | '(' expr ')'		                                {$$ = $2;}
	 | _NUM			                                    {$$ = $1; $$->vartype = searchType("int", TypeList);}
     | _ID                                              {$$ = createNode(VAR, $1, -1, NULL, NULL);}
     | _ID '[' expr ']'                                 {$$ = createNode(VAR, $1, -1, $3, NULL);}
     | _TEXT                                            {$$ = $1; $$->vartype = searchType("str", TypeList);}
     | funccall                                         {$$ = $1;}
     | field                                            {$$ = createNode(VAR, $1, -1, NULL, NULL);}
     | _NULL                                            {$$ = createNode(CONST, "\0", 0, NULL, NULL); $$->vartype = searchType("int", TypeList);} 
	 ;

return : _RET expr _SEMI                                {$$ = createNode(RET, "\0", -1, $2, NULL);}
        ;


read : _READ '(' _ID ')' _SEMI                          {$$ = createNode(READ, "", -1, createNode(VAR, $3, -1, NULL, NULL), NULL);}
     | _READ '(' _ID '[' expr ']' ')' _SEMI             {$$ = createNode(READ, "", -1, createNode(VAR, $3, -1, $5, NULL), NULL);}
     | _READ '(' field ')' _SEMI                          {$$ = createNode(READ, "", -1, createNode(VAR, $3, -1, NULL, NULL), NULL);}
     ;
     
init : _INIT '(' ')'                                      {$$ = createNode(INIT, "", -1, NULL, NULL);}
     ;

alloc : _ALLOC '(' ')'                                 {$$ = createNode(ALLOC, "", -1, NULL, NULL);}
      ;

write : _WRITE '(' expr ')' _SEMI                       {$$ = createNode(WRITE, "", -1, $3, NULL);}
      ;

assgn : _ID _EQUALS expr _SEMI                          { 
                                                         $$ = createNode(ASSN, "", -1, createNode(VAR, $1, -1, NULL, NULL), $3);}
      | _ID '[' expr ']' _EQUALS expr _SEMI             {
                                                         $$ = createNode(ASSN, "", -1, createNode(VAR, $1, -1, $3, NULL), $6);}
      | field _EQUALS expr _SEMI                        {$$ = createNode(ASSN, "", -1, createNode(VAR, $1, -1, NULL, NULL), $3);}
      ;

ifstmt : _IF '(' expr ')' _THEN stmtList _ELSE
         stmtList _ENDIF _SEMI                          {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, $6, $8);
                                                            $$ = createNode(IF, "", -1, $3, tmp);
                                                            printf("%s %s\n", $3->right->varname, $3->left->varname);
                                                        }
       | _IF '(' expr ')' _THEN stmtList _ENDIF _SEMI   {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, $6, NULL);
                                                            $$ = createNode(IF, "", -1, $3, tmp);
                                                            printf("%s %s\n", $3->right->varname, $3->left->varname);
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
     | funcstmt                                         {$$ = $1;}
     ;

stmtList : stmtList stmt                                {$$ = connect($1, $2);}
         | stmt                                         {$$ = $1;}
         ;


arg : expr                                              {$$ = $1;}
    |                                                   {$$ = NULL;}
    ;

args : args _COMMA arg                                  {$$ = connect($3, $1);}
     | arg                                              {$$ = $1;}
     ;
funccall : _ID '(' args ')'                      {  GSymbol* sym = (GSymbol*)searchSymbol($1, GSymList);
                                                    if(sym== NULL) yyerror("Function is not declared");
                                                    $$ = createNode(FUNC, $1, -1, $3, NULL);
                                                    
                                                    $$->vartype = sym->type;}
         | alloc                                        {$$ = $1;}
         | init                                         {$$ = $1;}
         ;
funcstmt : funccall _SEMI                               { $$ = $1;}
         ;
        
 /*-------------$3->next = $1; $$ = $3----------------------------------------------------------------------------------------------------------------*/


 /*-----------------------------------------------------Types-------------------------------------------------------------------*/

type : _ID                                             {$$ = $1;}
 /*-----------------------------------------------------------------------------------------------------------------------------*/


 /*----------------------------------------------------------Local Declaration---------------------------------------------------*/


ldeclblock : _DECL ldecllist  _ENDDECL                  { $$ = $2;}
            | _DECL _ENDDECL                            { $$ = NULL;}
            ;

 ldecllist : ldecllist ldecl                            { $$ = connectList($1, $2, sizeof(LSymbol));}
         | ldecl                                        {$$ = $1;}
         ;

        
ldecl : type lvarlist _SEMI                             {
                                                            LinkedList* gvars = $2;
                                                            $$ = NULL;
                                                            while(gvars) {
                                                                char *var = (char*)(gvars->data);
                                                                LSymbol* tmp = (LSymbol*)malloc(sizeof(LSymbol));
                                                                Type *type = (Type*)searchType($1, TypeList);
                                                                if(type==NULL) yyerror("Undefined type");
                                                                *tmp = (LSymbol){.name=var, .type=type, .size=1};
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

gdeclblock : _DECL gdecllist _ENDDECL                   {}
            | _DECL _ENDDECL                            {}
            ;

 gdecllist : gdecllist gdecl                            {}
         | gdecl                                        {}
         ;
 
        
gdecl : type gvarlist _SEMI                             {
                                                            LinkedList* gvars = $2;
                                                            while(gvars) {
                                                                GVariable *var = (GVariable*)(gvars->data);
                                                                if(searchSymbol(var->name, GSymList)) {
                                                                    yyerror("Variable is already declared");
                                                                }
                                                                GSymbol* tmp = (GSymbol*)malloc(sizeof(GSymbol));
                                                                Type *type = (Type*)searchType($1, TypeList);
                                                                if(type==NULL) yyerror("Undefined type");
                                                                *tmp = (GSymbol){.name=var->name, .type=type, .size=var->size, .params=var->params, .binding=curMemory};
                                                                GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                                gvars = gvars->next;
                                                                curMemory+=var->size;
                                                            }
                                                        }
     ;
gvarlist : gvarlist _COMMA _ID                          {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$3, .size=1};
                                                         $$ = addNode(tmp, sizeof(GVariable), $1);
                                                        }

        | gvarlist _COMMA _ID '[' _NUM ']'              {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$3, .size=$5->val};
                                                         $$ = addNode(tmp, sizeof(GVariable), $1);
                                                        }

        | gvarlist _COMMA _ID '(' paramlist ')'         {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$3, .params=$5, .size=0};
                                                         $$ = addNode(tmp, sizeof(GVariable), $1);
                                                        }

        | _ID '[' _NUM ']'                              {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$1, .size=$3->val};
                                                         $$ = addNode(tmp, sizeof(GVariable), NULL);
                                                        }

        | _ID                                           {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$1, .size=1};
                                                         $$ = addNode(tmp, sizeof(GVariable), NULL);
                                                        }

        | _ID '(' paramlist ')'                         {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=$1, .params=$3, .size=0};
                                                         $$ = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
        ;

 /*-----------------------------------------------------------------------------------------------------------------------------*/

 /*----------------------------------------------------------Function Definition-------------------------------------------------*/

fdefblock : fdefblock fdef                              {
                                                         fprintf(out, "%s:\n", $2->varname);
                                                         eval_func($2, out);}

          | fdef                                        {
                                                         fprintf(out, "%s:\n", $1->varname);
                                                         eval_func($1, out);}                           
          ;

fdef : type _ID '(' paramlist ')' '{'ldeclblock  _BEGIN stmtList return  _END'}'   {
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = $7;    
                                                                    addArgSymbol($4, frame, -3, out);
                                                                    if($10->left->type == VAR) {  
                                                                        LSymbol *sym = (LSymbol*)searchSymbol($10->left->varname, $7);
                                                                        if(sym && strcmp($1, sym->type->name)) yyerror("Return type is not correct");
                                                                        if(!sym) {
                                                                            LSymbol *sym = (LSymbol*)searchSymbol($10->left->varname,GSymList);
                                                                            if(sym && strcmp($1, sym->type->name)) yyerror("Return type is not correct");
                                                                        }
                                                                    } else if(strcmp($1, $10->left->vartype->name)) yyerror("Return type is not correct");
                                                                    $$ = createNode(FUNC, $2, -1, connect($9, $10), $4); $$->vartype = searchType($1, TypeList);
                                                                    GSymbol* tmp = searchSymbol($2, GSymList);
                                                                    if(tmp == NULL) yyerror("Function is not declared");
                                                                    tmp->frame = frame;
                                                                    tmp->type = $$->vartype;
                                                                    }
                                                                    
            ;

paramlist : paramlist _COMMA param                      { $$ = connect($3, $1);}
          | param                                       { $$ = $1;}
          |                                             { $$ = NULL;}
          ;

param : type _ID                                        {$$ = createNode(VAR, $2, -1, NULL, NULL); $$->vartype = searchType($1, TypeList);}
      ;

 /*-----------------------------------------------------------------------------------------------------------------------------*/

%%




int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Usage: ./silc <source_file>\n");
        exit(1);
    }
    curMemory = 4096;
    GSymList = NULL;
    LSymList = NULL;
    LVarList = NULL;
    GVarList = NULL;
    Type *type = (Type*)malloc(sizeof(Type));
    *type = (Type){.name="int", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);
    *type = (Type){.name="str", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);
    *type = (Type){.name="bool", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);     
    *type = (Type){.name="void", .size=0, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);       
    yyin = fopen(argv[1], "r");
    if(yyin == NULL) yyerror("Input file not found");
	out = fopen("./out.xsm", "w");
    fprintf(out, "0\n2056\n0\n0\n0\n0\n0\n0\nMOV SP, 4300\nCALL main\n");
    fprintf(out, "MOV R19, \"Exit\"\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nCALL 0\n");
    if(out == NULL) yyerror("file");
	yyparse();
	return 0;
}
