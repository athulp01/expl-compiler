/*
Contains all data structures and helper methods to 
create the abstract symbol treee
*/
#pragma once
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define LOG(s1, s2) printf("DEBUG | %s : %s\n", s1, s2);
#else
#define LOG(s1, s2) if(0);
#endif

/* -----------------Enums--------------*/

//Register use status
enum STATUS { FREE, IN_USE, RESV };

//Type of the AST node
enum TYPE {
  CONST,
  READ,
  WRITE,
  CONN,
  VAR,
  OP,
  ASSN,
  IF,
  WHILE,
  IF_BODY,
  BREAK,
  CONT,
  FUNC,
  RET,
  INIT,
  ALLOC,
  FFREE,
  METHOD,
  NEW,
};

//Deprecated! Use Type instead.
enum VARTYPE { INT, BOOL, STRING };
//--------------------------------------

/* -----------------typedefs--------------*/

typedef short reg_index;

//Generic linkedlist definition
typedef struct _LinkedList {
  void* data;
  struct _LinkedList* next;
} LinkedList;

// structure to keep track of the current label
typedef struct _LabelList {
  int start_label;
  int end_label;
  struct _LabelList *next, *prev;
} LabelList;

typedef struct _Frame {
  enum STATUS registers[20];
  LinkedList* Lvars;
} Frame;

// Structure for storing variable list
typedef struct _Parameter {
  char* name;
  enum VARTYPE type;
} Parameter;

struct _GSymbol;
struct _Type;
struct _Classdef;
//AST node
typedef struct _tnode {
  enum TYPE type;
  struct _Type *vartype;
  struct _ClassDef *varclass;
  char* varname;
  struct _GSymbol* symbol;    //can also refer to local symbol by type casting
  int val;
  struct _tnode *left, *right;  // left and right branches
} tnode;

//helper struct to create global symtable
typedef struct {
  char* name;
  int size;
  tnode* params;
  int isfunc;
} GVariable;

//For storing user and inbuilt types
typedef struct _Type {
  char *name;
  int size;
  LinkedList *fields;
}Type;

union {
  Type *a;
}T;

//For storing class table
typedef struct _ClassDef {
  char *name;
  LinkedList *fields;
  LinkedList *methods;
  struct _ClassDef *parent;
  unsigned int idx;
  int fieldCount;
  int methodCount;
} ClassDef;

//For storing fields of user defined types
typedef struct _Field {
  char *name;
  int idx;
  struct _Type *type;
  char *ndef;
  ClassDef *class;   //Only for OExpl
}Field;

typedef struct _Method {
  char *name;
  Type *type;
  tnode *params;
  int idx;
  ClassDef *class;
  Frame *frame;
}Method;

// Structure for storing the global symbol table
typedef struct _GSymbol {
  char* name;
  Type *type;
  int size;
  int binding;
  ClassDef *class;
  tnode* params;
  Frame* frame;
} GSymbol;

// structure for storing the local symbol table
typedef struct _LSymbol {
  char* name;  
  Type *type;
  int size;
  int binding;
  ClassDef *class;
} LSymbol;

//---------------------------------------------

extern LinkedList *GSymList, *LSymList, *TypeList, *ClassList, *curClassField, *curClassMethod;
extern char *curClassName;
//Last label used
extern int label;


//----------------Helper functions---------------------

/*Create a node AST node*/
tnode* createNode(enum TYPE type, char* s, int n, tnode* l, tnode* r);
//connect two AST nodes
tnode* connect(tnode* first, tnode* second);


//Merge two linkedlist
LinkedList* connectList(LinkedList* first, LinkedList *second, size_t size);
//Adds a node to a linkedlist
LinkedList* addNode(void*, size_t, LinkedList*);
//copy a linkedlist
LinkedList* copyList(LinkedList*, size_t);

//search in the symbol table
void* searchSymbol(char*, LinkedList*);
//search in the type table
Type* searchType(char*, LinkedList*);
//search and return field index
int searchField(char*, LinkedList*);
//
Method* searchMethod(char*, LinkedList*);
Field* getField(char*, LinkedList*);
//Search int the class table
ClassDef* searchClass(char*, LinkedList*);

//create a label node (if time permits use generic addNode())
LabelList* createLlistNode(int, int, LabelList*);
LabelList* deleteLlistNode(LabelList*);
//Get a label from the labelstore
int getLabel();
// Get a register
reg_index getReg(Frame*);
// free the last used register
void freeReg(Frame*);
// Push all the registers which are in use to stack
int pushRegToStack(Frame*, FILE*);
// Get back all the registers from the stack
int getRegFromStack(Frame*, FILE*, int);
