#include "../include/datastructures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/evaluators.h"

int yyerror(char*);
int label = 0;

Type* searchType(char* s, LinkedList* front) {
  while (front) {
    Type* type = (Type*)front->data;
    if (!strcmp(type->name, s))
      return type;
    front = front->next;
  }
  return NULL;
}

int searchField(char* s, LinkedList* front) {
  while (front) {
    Field* field = (Field*)front->data;
    if (!strcmp(field->name, s)) {
      return field->idx;
    }
    front = front->next;
  }
  return 0;
}

ClassDef* searchClass(char* s, LinkedList* front) {
  while (front) {
    ClassDef* class = (ClassDef*)front->data;
    if (!strcmp(class->name, s))
      return class;
    front = front->next;
  }
  return NULL;
}

Method* searchMethod(char* s, LinkedList* front) {
  while (front) {
    Method* method = (Method*)front->data;
    if (!strcmp(method->name, s))
      return method;
    front = front->next;
  }
  return 0;
}
Field* getField(char* s, LinkedList* front) {
  while (front) {
    Field* field = (Field*)front->data;
    if (!strcmp(field->name, s))
      return field;
    front = front->next;
  }
  return NULL;
}

tnode* createNode(enum TYPE type, char* s, int n, tnode* l, tnode* r) {
  tnode* tmp = (tnode*)malloc(sizeof(tnode));
  tmp->type = type;
  tmp->varname = s;
  tmp->val = n;
  tmp->left = l;
  tmp->right = r;
  switch (type) {
    case FFREE:
      if (!strcmp(l->vartype ? l->vartype->name : l->varclass->name, "int") ||
          !strcmp(l->vartype ? l->vartype->name : l->varclass->name, "str"))
        yyerror("Only for user defined types can be freed");
      break;
    case ASSN:
    case OP:
      if (r->type == ALLOC || r->type == NEW) {
        if (!strcmp(l->vartype ? l->vartype->name : l->varclass->name, "int") ||
            !strcmp(l->vartype ? l->vartype->name : l->varclass->name, "str"))
          yyerror("Dynamic allocation is possible only for user defined types");
        break;
      }
      if (!strcmp(r->vartype ? r->vartype->name : r->varclass->name, "null")) {
        if (!strcmp(l->vartype ? l->vartype->name : l->varclass->name, "int") ||
            !strcmp(l->vartype ? l->vartype->name : l->varclass->name, "str"))
          yyerror("Null can be only assigned to user defined type");
      } else if (l->varclass && r->varclass) {
        ClassDef* right = searchClass(r->varclass->name, ClassList);
        while (right) {
          if (!strcmp(r->varclass->name, l->varclass->name))
            break;
          right = right->parent;
        }
      } else if (strcmp(l->vartype ? l->vartype->name : l->varclass->name,
                        r->vartype ? r->vartype->name : r->varclass->name))
        yyerror("Type mismatch");
      break;
    case READ:
      if (strcmp(l->vartype ? l->vartype->name : l->varclass->name, "int") &&
          !strcmp(l->vartype ? l->vartype->name : l->varclass->name, "str"))
        yyerror("Type mismatch");
      break;
  }
  return tmp;
}

tnode* connect(tnode* first, tnode* second) {
  tnode* tmp = (tnode*)malloc(sizeof(tnode));
  tmp->type = CONN;
  tmp->left = first;
  tmp->right = second;
  return tmp;
}

LinkedList* addNode(void* data, size_t size, LinkedList* front) {
  LinkedList* tmp = (LinkedList*)malloc(sizeof(LinkedList*));
  tmp->data = (void*)malloc(size);
  memcpy(tmp->data, data, size);
  tmp->next = front;
  return tmp;
}

void* searchSymbol(char* name, LinkedList* front) {
  while (front) {
    LSymbol* tmp = (LSymbol*)front->data;
    if (!strcmp(tmp->name, name))
      return front->data;
    front = front->next;
  }
  return NULL;
}

LinkedList* copyList(LinkedList* start, size_t dataSize) {
  LinkedList* tmp = NULL;
  while (start) {
    LinkedList* prev = (LinkedList*)malloc(sizeof(LinkedList));
    prev->data = (void*)malloc(dataSize);
    memcpy(prev->data, start->data, dataSize);
    prev->next = tmp;
    tmp = prev;
    start = start->next;
  }
  return tmp;
}

LinkedList* connectList(LinkedList* first, LinkedList* second, size_t size) {
  LinkedList* tmp = first;
  if (!first)
    return second;
  while (first->next) {
    first = first->next;
  }
  first->next = second;
  return tmp;
}

LabelList* createLlistNode(int s, int e, LabelList* root) {
  LabelList* tmp = (LabelList*)malloc(sizeof(LabelList));
  tmp->start_label = s;
  tmp->end_label = e;
  tmp->next = root;
  if (root)
    root->prev = tmp;
  return tmp;
}

LabelList* deleteLlistNode(LabelList* root) {
  if (root->next) {
    root->next->prev = NULL;
    return root->next;
  } else
    return NULL;
}

int getLabel() {
  return label++;
}

reg_index getReg(Frame* frame) {
  for (reg_index i = 0; i < 20; i++) {
    if (frame->registers[i] == FREE) {
      frame->registers[i] = IN_USE;
      return i;
    }
  }
  printf("All the registers are occupied\n");
  exit(1);
  return 1;
}

void freeReg(Frame* frame) {
  for (reg_index i = 19; i >= 0; i--)
    if (frame->registers[i] == IN_USE) {
      frame->registers[i] = FREE;
      return;
    }
}

int pushRegToStack(Frame* frame, FILE* out) {
  int count = 0;
  for (reg_index i = 0; i < 20; i++) {
    if (frame->registers[i] == IN_USE) {
      count++;
      fprintf(out, "PUSH R%d \\register\n", i);
      frame->registers[i] = RESV;
    }
  }
  return count;
}

int getRegFromStack(Frame* frame, FILE* out, int num) {
  int count = 0;
  for (reg_index i = 19; i >= 0; i--) {
    if (num && frame->registers[i] == RESV) {
      count++;
      num--;
      fprintf(out, "POP R%d \\register\n", i);
      frame->registers[i] = IN_USE;
    }
  }
  return count;
}
