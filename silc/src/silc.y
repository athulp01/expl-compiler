%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "evaluators.h"
    #include <string.h>
    #include "datastructures.h"

	int yylex(void);
  char virtual[1000];
    int yyerror(char*);
    int classno = 0, mid =0;
    FILE *out, *yyin;
    LinkedList *GSymList, *LSymList, *LVarList, *GVarList, *TypeList, *curLvar, *ClassList, *curClassField, *curClassMethod;
    Frame *curFrame;
    char *curClassName;
    int curMemory, curClassFieldMem;

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

    Type* getTypeOfSymbol(char *oname) {
        char *sname = strdup(oname);
        char *name = strtok(sname, ".");
        GSymbol* sym = searchSymbol(name, curLvar);
        if (sym) {  // if it is a local variable
            Field* field = NULL;
            if(!strcmp(name, "self")) { // search in the fields of current class if begin with "self"
                char *tok;
                LinkedList *lst = curClassField;
                while((tok = strtok(NULL, "."))) {
                    LOG("field", tok)
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                } 
            }else {
                char *tok;
                LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                    
                }
            }
            return field?field->type:sym->type;            
        } else {
            sym = searchSymbol(name, GSymList);
            if (sym) {  // global variable
                Field* field = NULL;
                char *tok;
                LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                }
                return field?field->type:sym->type; 
            } else {
                printf("%s", sname);
                yyerror("Undefined variable");
                return NULL;
            }
        } 
    }

    ClassDef* getClassOfSymbol(char *oname) {
        char *sname = strdup(oname);
        char *name = strtok(sname, ".");
        GSymbol* sym = searchSymbol(name, curLvar);
        if (sym) {  // local variable
            Field* field = NULL;
            if(!strcmp(name, "self")) {
                char *tok;
                LinkedList *lst = curClassField;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                } 
            }else {
                char *tok;
                LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                }
            }
            return field?field->class:sym->class;            
        } else {
            sym = searchSymbol(name, GSymList);
            if (sym) {  // global variable
                Field* field = NULL;
                char *tok;
                LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                }
                return field?field->class:sym->class; 
            } else {
                printf("%s", sname);
                yyerror("Undefined variable");
                return NULL;
            }
        } 
    }

    LinkedList* addParam(tnode *root) {
        if(root == NULL) return NULL;
        LinkedList *res = NULL;
        if(root->type != CONN) {
            res = (LinkedList*)malloc(sizeof(LinkedList));
            // printf("%s %s\n", root->vartype->name, root->varname);
            LSymbol* tmp = malloc(sizeof(LSymbol));
            *tmp = (LSymbol){.name = root->varname, .type = root->vartype};
            res->data = (void*)tmp;
            res->next = NULL;
            return res;
        }else {
            LinkedList *right = addParam(root->right);
            LinkedList *left = addParam(root->left);
            return connectList(right, left, sizeof(LSymbol));
            // if(right) {
            //     right->next = left;
            //     return right;
            // }
            // return left;
        }

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
%type <no> param paramlist fdef return arg args funccall funcstmt init alloc params free methodcall new
%type <name> _ID type field inherit
%type <list> gvarlist lvarlist ldecl gdeclblock gdecl ldecllist ldeclblock fieldlst methodlst classmemberlst classmember classmemberblock methoddef
%type <field> fielddef

%token _DECL _ENDDECL _INT _STR _TEXT _MAIN _RET _TYPE _ENDTYPE _NULL _EXTENDS
%token _IF _WHILE _THEN _ELSE _ENDIF _ENDWHILE _DO _BREAK _CONT _AND _INIT _ALLOC _FREE _CLASS _SELF _ENDCLASS _NEW
%token _LT _GT _EQ _NE _LE _GE
%token _PLUS _MINUS _MUL _DIV _END _BEGIN _READ _WRITE _SEMI _EQUALS _Q _COMMA _MOD 
%token _ID _NUM

%nonassoc _LT _GT _EQ _NE _LE _GE _ID _AND

%left _MOD
%left _PLUS _MINUS
%left _MUL _DIV



%%

/*----------------------------------------------------------Program-----------------------------------------------------------*/

program : typedefblock classdefblock gdeclblock fdefblock mainblock
        | typedefblock classdefblock gdeclblock mainblock
        | gdeclblock fdefblock mainblock
        | typedefblock mainblock
        | gdeclblock mainblock
        | classdefblock gdeclblock mainblock
        | mainblock
        ;

 /*-----------------------------------------------------------------------------------------------------------------------------*/


 /*--------------------------------------------------------Main-----------------------------------------------------------------*/

 mainblock : _ID _MAIN '(' ')' '{' ldeclblock _BEGIN stmtList return  _END '}'      {
                                                             tnode *node = createNode(FUNC, "main", -1, connect($8,$9), NULL); $$->vartype = 0;
                                                             GSymbol* tmp = malloc(sizeof(GSymbol));
                                                             Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                             frame->Lvars = $6;
                                                             *tmp = (GSymbol){.name="main", .params=NULL, .frame=frame};
                                                             GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                             fprintf(out ,"main:\nBRKP\n");
                                                             fprintf(out, "%s", virtual);
                                                             eval_func(node, out);
                                                            } 

 /*-----------------------------------------------------------------------------------------------------------------------------*/

  /*--------------------------------------------------------Class defintion-----------------------------------------------------------------*/
  classdefblock : classdefblock classdef
                | classdef
                ;

  classdef : _CLASS _ID '{' classmemberblock methodlst '}' _ENDCLASS      { 
                                                                 if(searchClass($2, ClassList)) yyerror("Duplicate class");
                                                                ClassDef *class = (ClassDef*)malloc(sizeof(ClassDef));
                                                                 class->name = $2;
                                                                 curClassName = $2;
                                                                 class->idx = classno++;
                                                                 class->fields = curClassField;
                                                                 int begin = 1;
                                                                 LinkedList *f = class->fields;
                                                                 while(f) {
                                                                     ((Field*)f->data)->idx = begin++;
                                                                     if(((Field*)f->data)->class) begin++;
                                                                     f = f->next;
                                                                 }
                                                                 if(begin>8) yyerror("Maximum field size exceeded");
                                                                 class->methods = curClassMethod;
                                                                 LinkedList *mets = class->methods;
                                                                 int nummethods = 0;
                                                                 while(mets) {
                                                                     nummethods++;
                                                                    ((Method*)mets->data)->class = class;
                                                                    mets = mets->next;
                                                                 }
                                                                 if(nummethods>8) yyerror("Maximum number of methods exceeded");
                                                                 LinkedList *func = $5;
                                                                 ClassList = addNode(class, sizeof(ClassDef), ClassList);
                                                                 tnode *tmp;
                                                                 while(func) {
                                                                     tmp = (tnode*)func->data;
                                                                     char *s = malloc(sizeof($2)+sizeof(tmp->varname)+2);
                                                                     strcpy(s, $2);
                                                                     strcat(s, ".");
                                                                     strcat(s, tmp->varname);
                                                                     char t[50];
                                                                     sprintf(t, "MOV R19, %s\nMOV [%d], R19\n", s, class->idx*8 + 4096+searchMethod(tmp->varname, curClassMethod)->idx);
                                                                     strcat(virtual, t);
                                                                     fprintf(out, "%s:\n",s);
                                                                     eval_method(tmp, curClassMethod, out);
                                                                     func = func->next;
                                                                 }
                                                                 curClassMethod = NULL;
                                                                 curClassField = NULL;
                                                                  curMemory = (classno + 1)*8 + 4096;
                                                                  mid=0;
                                                                }
           | _CLASS _ID inherit '{' classmemberblock methodlst '}' _ENDCLASS      { 
                                                                 if(searchClass($2, ClassList)) yyerror("Duplicate class");
                                                                 ClassDef *parent = searchClass($3, ClassList);
                                                                 if(!parent) yyerror("Parent class is not defines");
                                                                 ClassDef *class = (ClassDef*)malloc(sizeof(ClassDef));
                                                                 class->name = $2;
                                                                 curClassName = $2;
                                                                 class->idx = classno++;
                                                                 class->fields = curClassField;
                                                                 int begin = 1;
                                                                 LinkedList *f = class->fields;
                                                                 while(f) {
                                                                     ((Field*)f->data)->idx = begin++;
                                                                     if(((Field*)f->data)->class) begin++;
                                                                     f = f->next;
                                                                 }
                                                                 if(begin>8) yyerror("Maximum field size exceeded");
                                                                 class->parent = parent;
                                                                 int nummethods = 0;
                                                                 LinkedList *mtds = curClassMethod;
                                                                 while(mtds) {
                                                                    nummethods++;
                                                                    ((Method*)mtds->data)->class = class;
                                                                     char s[100];
                                                                     strcpy(s, $2);
                                                                     strcat(s, ".");
                                                                     strcat(s, ((Method*)mtds->data)->name);
                                                                     char t[50];
                                                                    sprintf(t, "MOV R19, %s\nMOV [%d], R19\n",s, class->idx*8+4096+((Method*)mtds->data)->idx);
                                                                     strcat(virtual, t);
                                                                    mtds = mtds->next;
                                                                  }
                                                                  int numm = nummethods - 1;
                                                                 mtds = copyList(parent->methods, sizeof(Method));
                                                                 LinkedList* parentm = mtds;
                                                                 while(mtds) {
                                                                    Method* child = searchMethod(((Method*)mtds->data)->name, curClassMethod);
                                                                    if(child) {
                                                                        ((Method*)mtds->data)->idx += child->idx;
                                                                        checkArg(child->params, ((Method*)mtds->data)->params);
                                                                        mtds = mtds->next;
                                                                        continue;
                                                                    }
                                                                    ((Method*)mtds->data)->idx += nummethods-1;
                                                                    numm++;
                                                                     char s[100];
                                                                     strcpy(s, ((Method*)mtds->data)->class->name);
                                                                     strcat(s, ".");
                                                                     strcat(s, ((Method*)mtds->data)->name);
                                                                     char t[50];
                                                                    sprintf(t, "MOV R19, %s\nMOV [%d], R19\n",s, class->idx*8+4096+((Method*)mtds->data)->idx);
                                                                    strcat(virtual, t);
                                                                    mtds = mtds->next;
                                                                  }
                                                                  if(numm>8) yyerror("Maximum method size exceeded");
                                                                 class->methods = connectList(curClassMethod, parentm, sizeof(Method));
                                                                 class->parent = parent;
                                                                 LinkedList *func = $6;
                                                                 ClassList = addNode(class, sizeof(ClassDef), ClassList);
                                                                 tnode *tmp;
                                                                 while(func) {
                                                                     tmp = (tnode*)func->data;
                                                                     char *s = malloc(sizeof($2)+sizeof(tmp->varname)+2);
                                                                     strcpy(s, $2);
                                                                     strcat(s, ".");
                                                                     strcat(s, tmp->varname);
                                                                     fprintf(out, "%s:\n",s);
                                                                     eval_method(tmp, curClassMethod, out);
                                                                     func = func->next;
                                                                 }
                                                                 curClassMethod = NULL;
                                                                 mid = 0;
                                                                 curClassField = NULL;
                                                                  curMemory = (classno + 1)*8 + 4096;
                                                                }
 inherit : _EXTENDS _ID  {
                              curClassField = searchClass($2, ClassList)->fields;
                              $$ = $2;
                          }
         ;

  methodlst : methodlst methoddef                               {$$ = connectList($1, $2, sizeof(tnode));}
            | methoddef                                         {$$ = $1;}

  methoddef : type _ID '(' paramlist ')' '{'ldeclblock  _BEGIN stmtList return  _END'}'   {
                                                                    Method* tmp = searchMethod($2, curClassMethod);
                                                                    if(!tmp) yyerror("Method not declared");
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = $7;
                                                                    tmp->frame = frame;
                                                                    tmp->type = searchType($1, TypeList); 
                                                                    if($10->left->type == VAR) {  
                                                                        if(strcmp($1, getTypeOfSymbol($10->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp($1, $10->left->vartype->name)) yyerror("Return type is not correct");
                                                                    LinkedList *lst = (LinkedList*)malloc(sizeof(LinkedList));
                                                                    lst->data = createNode(METHOD, $2, -1, connect($9, $10), $4); ((tnode*)lst->data)->vartype = searchType($1, TypeList);
                                                                    $$ = lst;
                                                                    curLvar = NULL;
                                                                    }     
            | type _ID '(' paramlist ')' '{'  _BEGIN stmtList return  _END'}'   {
                                                                    Method* tmp = searchMethod($2, curClassMethod);
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = NULL;
                                                                    tmp->frame = frame;
                                                                    tmp->type = searchType($1, TypeList); 
                                                                    if($9->left->type == VAR) {  
                                                                        if(strcmp($1, getTypeOfSymbol($9->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp($1, $9->left->vartype->name)) yyerror("Return type is not correct");
                                                                    LinkedList *lst = (LinkedList*)malloc(sizeof(LinkedList));
                                                                    lst->data = createNode(METHOD, $2, -1, connect($8, $9), $4); ((tnode*)lst->data)->vartype = searchType($1, TypeList);
                                                                    $$ = lst;
                                                                    curLvar = NULL;
                                                                    }                                                             
                                                                    
            ;

classmemberblock : _DECL classmemberlst _ENDDECL               {$$ = $2;}
                 ;

classmemberlst : classmemberlst classmember                     {$$ = connectList($1, $2, sizeof(Field));}
                 | classmember                                      {$$ = $1;}

classmember : type gvarlist _SEMI                             {
                                                                LinkedList *fields= NULL, *methods = NULL;
                                                                LinkedList* gvars = $2;
                                                                while(gvars) {
                                                                    GVariable *var = (GVariable*)(gvars->data);
                                                                    if(var->isfunc) {
                                                                        if(searchMethod(var->name, curClassMethod)) yyerror("Duplicate method");
                                                                        Method* tmp = (Method*)malloc(sizeof(Method));
                                                                        ClassDef *class = searchClass($1, ClassList);
                                                                        Type *type = (Type*)searchType($1, TypeList);
                                                                        if(type==NULL && class==NULL) yyerror("Undefined type");
                                                                        *tmp = (Method){.name=var->name, .type=type, .class=class, .params=var->params};
                                                                        tmp->idx = mid++;
                                                                        methods = addNode(tmp, sizeof(Method), methods);
                                                                    } else {
                                                                        if(getField(var->name, curClassField)) yyerror("Duplicate field");
                                                                        Field* tmp = (Field*)malloc(sizeof(Field));
                                                                        ClassDef *class = searchClass($1, ClassList);
                                                                        Type *type = (Type*)searchType($1, TypeList);
                                                                        if(type==NULL && class==NULL) yyerror("Undefined type");
                                                                        *tmp = (Field){.name=var->name, .type=type, .class=class, .idx = curClassFieldMem++};
                                                                        if(class) curClassFieldMem++;
                                                                        fields = addNode(tmp, sizeof(Field), fields);
                                                                    }
                                                                    gvars = gvars->next;
                                                                }
                                                                curClassMethod = connectList(curClassMethod, methods, sizeof(Method));
                                                                curClassField = connectList(curClassField, fields, sizeof(Field));
                                                            }
     ;        

methodcall : field  '(' args ')'                    {   char *dup = strdup($1);
                                                        char *prev;
                                                
                                                        LOG("methodname", dup)char *s = strtok(dup, ".");
                                                        GSymbol* sym = (GSymbol*)searchSymbol(s, GSymList);
                                                        if(sym==NULL) sym = (GSymbol*)searchSymbol(s, curLvar);
                                                        if(sym== NULL) yyerror("Function is not declared");
                                                        LinkedList *lst = sym->class->fields, *methods = sym->class->methods;
                                                        Field *field;
                                                        prev = strdup(s);
                                                        while((s=strtok(NULL, "."))) {
                                                            prev = strdup(s);
                                                            field = getField(s, lst);
                                                            if(field) {
                                                                methods = field->class->methods;
                                                                lst = field->class->fields;
                                                            }
                                                        }
                                                        $$ = createNode(METHOD, $1, -1, $3, NULL);
                                                        Method* method = searchMethod(prev, methods);
                                                        if(!method) yyerror("Method not found");
                                                        $$->vartype = method->type;
                                                        }
            ;
 /*--------------------------------------------------------Type defintion-----------------------------------------------------------------*/

 typedefblock : typedefblock typedef                      {}
                | typedef                                 {}
                ;

fielddef : type _ID _SEMI                                 { Field *field = (Field*)malloc(sizeof(Field));
                                                            Type *type = searchType($1, TypeList);
                                                            *field = type?(Field){.name=$2, .type=type}:(Field){.name=$2, .ndef=$1};
                                                            $$ = field;
                                                          }
      ;

fieldlst : fieldlst fielddef                               { $2->idx = ((Field*)$1->data)->idx + 1;
                                                             if(getField($2->name, $1))
                                                                yyerror("Duplicate field entry");
                                                             $$ = addNode($2, sizeof(Field), $1);}
         | fielddef                                        { $1->idx = 1; $$ = addNode($1, sizeof(Field), NULL); }
         ;

typedef : _TYPE _ID '{' fieldlst '}' _ENDTYPE             { if(searchType($2, TypeList)!=NULL) yyerror("Type is already defined");
                                                            Type *type = (Type*)malloc(sizeof(Type));
                                                            *type = (Type){.name=$2, .fields=$4};
                                                            LinkedList *field = type->fields;
                                                            int size = 0;
                                                            while(field) {
                                                                size++;
                                                                if(!((Field*)field->data)->type) {
                                                                    if(!strcmp(((Field*)field->data)->ndef, $2)) 
                                                                        ((Field*)field->data)->type = type;
                                                                    else yyerror("Undefined Type");
                                                                }
                                                                field = field->next;
                                                            }
                                                            if(size > 7) yyerror("Size of user defined type cannot be greater than 8");
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
     | _ID                                              {$$ = createNode(VAR, $1, -1, NULL, NULL); $$->vartype = getTypeOfSymbol($1); $$->varclass = getClassOfSymbol($1);}
     | _ID '[' expr ']'                                 {$$ = createNode(VAR, $1, -1, $3, NULL);$$->vartype = getTypeOfSymbol($1);}
     | _TEXT                                            {$$ = $1; $$->vartype = searchType("str", TypeList);}
     | funccall                                         {$$ = $1;}
     | field                                            {$$ = createNode(VAR, $1, -1, NULL, NULL);$$->vartype = getTypeOfSymbol($1); $$->varclass = getClassOfSymbol($1);}
     | _NULL                                            {$$ = createNode(CONST, "\0", 0, NULL, NULL); $$->vartype = searchType("null", TypeList);} 
	 ;

return : _RET expr _SEMI                                {$$ = createNode(RET, "\0", -1, $2, NULL);}
        ;


read : _READ '(' _ID ')' _SEMI                          {tnode *tmp = createNode(VAR, $3, -1, NULL, NULL);tmp->vartype = getTypeOfSymbol($3);
                                                        $$ = createNode(READ, "", -1, tmp, NULL);}
     | _READ '(' _ID '[' expr ']' ')' _SEMI             {tnode *tmp = createNode(VAR, $3, -1, $5, NULL);tmp->vartype = getTypeOfSymbol($3);
                                                        $$ = createNode(READ, "", -1, tmp, NULL);}
     | _READ '(' field ')' _SEMI                        {tnode *tmp = createNode(VAR, $3, -1, NULL, NULL);tmp->vartype = getTypeOfSymbol($3);
                                                        $$ = createNode(READ, "", -1, tmp, NULL);}
     ;
     
init : _INIT '(' ')'                                      {$$ = createNode(INIT, "", -1, NULL, NULL);$$->vartype = searchType("int", TypeList);}
     ;

alloc : _ALLOC '(' ')'                                 {$$ = createNode(ALLOC, "", -1, NULL, NULL); $$->vartype = searchType("int", TypeList);}
      ;

new   : _NEW '(' _ID ')'                                {$$ = createNode(NEW, $3, -1, NULL, NULL); $$->vartype = searchType("int", TypeList);}
      ;
free : _FREE '(' _ID ')' _SEMI                          {tnode *tmp = createNode(VAR, $3, -1, NULL, NULL);tmp->vartype = getTypeOfSymbol($3);tmp->varclass = getClassOfSymbol($3);
                                                        $$ = createNode(FFREE, "", -1, tmp, NULL);}
     ;

write : _WRITE '(' expr ')' _SEMI                       {$$ = createNode(WRITE, "", -1, $3, NULL);}
      ;

assgn : _ID _EQUALS expr _SEMI                          { tnode *tmp = createNode(VAR, $1, -1, NULL, NULL);tmp->vartype = getTypeOfSymbol($1); tmp->varclass = getClassOfSymbol($1);
                                                         $$ = createNode(ASSN, "", -1, tmp, $3);}
      | _ID '[' expr ']' _EQUALS expr _SEMI             {tnode *tmp = createNode(VAR, $1, -1, $3, NULL);tmp->vartype = getTypeOfSymbol($1); tmp->varclass = getClassOfSymbol($1);
                                                         $$ = createNode(ASSN, "", -1, tmp, $6);}
      | field _EQUALS expr _SEMI                        { tnode *tmp = createNode(VAR, $1, -1, NULL, NULL);tmp->vartype = getTypeOfSymbol($1); tmp->varclass = getClassOfSymbol($1);
                                                        $$ = createNode(ASSN, "", -1, tmp, $3);}
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
     | funcstmt                                         {$$ = $1;}
     | free                                             {$$ = $1;}
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
         | methodcall                                    {$$ = $1;}
         | new                                          {$$ = $1;}
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

        
ldecl : type lvarlist _SEMI                             {   $$ = NULL;
                                                            LinkedList* gvars = $2;
                                                            while(gvars) {
                                                                char *var = (char*)(gvars->data);
                                                                LSymbol* tmp = (LSymbol*)malloc(sizeof(LSymbol));
                                                                Type *type = (Type*)searchType($1, TypeList);
                                                                ClassDef* class = searchClass($1, ClassList);
                                                                if(type==NULL && class==NULL) yyerror("Undefined type");
                                                                *tmp = (LSymbol){.name=var, .type=type,.class=class, .size=1};
                                                                $$ = addNode(tmp, sizeof(LSymbol), $$);
                                                                gvars = gvars->next;
                                                            }
                                                            LinkedList *copy = copyList($$, sizeof(LSymbol));
                                                            curLvar = connectList(curLvar, copy, sizeof(LSymbol));
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
                                                                ClassDef* class = searchClass($1, ClassList);
                                                                if(type==NULL && class==NULL)
                                                                    yyerror("Undefined type");
                                                                *tmp = (GSymbol){.name=var->name, .type=type, .class=class, .size=var->size, .params=var->params, .binding=curMemory};
                                                                if(class) curMemory++;
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
                                                         curLvar = NULL;
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
                                                         *tmp = (GVariable){.name=$1, .params=$3, .size=0, .isfunc=1};
                                                         $$ = addNode(tmp, sizeof(GVariable), NULL);
                                                         curLvar = NULL;
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
                                                                    GSymbol* tmp = searchSymbol($2, GSymList);                                                                    
                                                                    if(tmp == NULL) yyerror("Function is not declared");
                                                                    if(tmp->frame != NULL) yyerror("Function is already defined");
                                                                    checkArg(tmp->params, $4);
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = $7;
                                                                    tmp->frame = frame;
                                                                    tmp->type = searchType($1, TypeList); 
                                                                    if($10->left->type == VAR) {  
                                                                        if(strcmp($1, getTypeOfSymbol($10->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp($1, $10->left->vartype->name)) yyerror("Return type is not correct");
                                                                    $$ = createNode(FUNC, $2, -1, connect($9, $10), $4); $$->vartype = searchType($1, TypeList);
                                                                    curLvar = NULL;
                                                                    }                                                                  
    | type _ID '(' paramlist ')' '{' _BEGIN stmtList return  _END'}'   {
                                                                    GSymbol* tmp = searchSymbol($2, GSymList);                                                                    
                                                                    if(tmp == NULL) yyerror("Function is not declared");
                                                                    if(tmp->frame != NULL) yyerror("Function is already defined");
                                                                    checkArg(tmp->params, $4);
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = NULL;
                                                                    tmp->frame = frame;
                                                                    tmp->type = searchType($1, TypeList); 
                                                                    if($9->left->type == VAR) {  
                                                                        if(strcmp($1, getTypeOfSymbol($9->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp($1, $9->left->vartype->name)) yyerror("Return type is not correct");
                                                                    $$ = createNode(FUNC, $2, -1, connect($8, $9), $4); $$->vartype = searchType($1, TypeList);
                                                                    curLvar = NULL;
                                                                    }                                                                       
    ;

paramlist : params                                      {curLvar = addParam($1); $$ = $1; 
                                                        LinkedList* t = curLvar;
                                                        LSymbol* tmp = malloc(sizeof(LSymbol));
                                                        ClassDef *type = malloc(sizeof(ClassDef));;
                                                        *type = (ClassDef){.fields=curClassField, .methods=curClassMethod};
                                                        *tmp = (LSymbol){.name = "self", .class=type };
                                                        curLvar = addNode(tmp, sizeof(LSymbol), curLvar);
                                                        t = curLvar;
                                                        }
          ;
params : params _COMMA param                            { $$ = connect($3, $1);}
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
    // TODO: change curMemory after vft
    curClassFieldMem = 1;
    mid = 0;
    GSymList = NULL;
    LSymList = NULL;
    LVarList = NULL;
    GVarList = NULL;
    curLvar = NULL;
    curMemory = 4096;
    Type *type = (Type*)malloc(sizeof(Type));
    *type = (Type){.name="int", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);
    *type = (Type){.name="str", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);
    *type = (Type){.name="bool", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);     
    *type = (Type){.name="void", .size=0, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);
    *type = (Type){.name="null", .size=0, .fields=NULL};
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
