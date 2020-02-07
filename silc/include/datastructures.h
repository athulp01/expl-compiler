#pragma once
#include <stdio.h>
#include <stdlib.h>

enum STATUS { FREE, IN_USE, RESV };

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
};

enum VARTYPE { INT, BOOL, STRING };

typedef short reg_index;

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

// structure for storing the local symbol table
typedef struct _LSymbol {
  char* name;
  enum VARTYPE type;
  int size;
  int binding;
} LSymbol;

// Structure for storing variable list
typedef struct _Parameter {
  char* name;
  enum VARTYPE type;
} Parameter;

struct _GSymbol;

typedef struct _tnode {
  enum TYPE type;
  enum VARTYPE vartype;
  char* varname;
  struct _GSymbol* symbol;
  int val;
  struct _tnode *left, *right;  // left and right branches
} tnode;

typedef struct {
  char* name;
  int size;
  tnode* params;
} GVariable;

// Structure for storing the global symbol table
typedef struct _GSymbol {
  char* name;
  enum VARTYPE type;
  int size;
  int binding;
  tnode* params;
  Frame* frame;
} GSymbol;

extern LinkedList *GSymList, *LSymList;
extern enum STATUS registers[20];
extern int label;

/*Create a node tnode*/

tnode* createNode(enum TYPE type, char* s, int n, tnode* l, tnode* r);
tnode* connect(tnode* first, tnode* second);
LinkedList* addNode(void*, size_t, LinkedList*);
void* searchSymbol(char*, LinkedList*);
LinkedList* copyList(LinkedList*, size_t);
LabelList* createLlistNode(int, int, LabelList*);
LabelList* deleteLlistNode(LabelList*);
int getLabel();
// Get a register
reg_index getReg(Frame*);
// free the last used register
void freeReg(Frame*);
// Push all the registers which are in use to stack
void pushRegToStack(Frame*, FILE*);
// Get back all the registers from the stack
void getRegFromStack(Frame*, FILE*);
