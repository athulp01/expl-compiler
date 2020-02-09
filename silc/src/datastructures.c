#include "datastructures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "evaluators.h"

int yyerror(char*);
int label = 0;

tnode* createNode(enum TYPE type, char* s, int n, tnode* l,
                  tnode* r) {
  tnode* tmp = (tnode*)malloc(sizeof(tnode));
  tmp->type = type;
  tmp->varname = s;
  tmp->val = n;
  tmp->left = l;
  tmp->right = r;
  return tmp;
}

tnode* connect(tnode* first, tnode* second) {
  tnode* tmp = (tnode*)malloc(sizeof(tnode));
  tmp->type = CONN;
  tmp->left = first;
  tmp->right = second;
  return tmp;
}

LinkedList* addNode(void *data, size_t size, LinkedList *front) {
    LinkedList *tmp = (LinkedList*)malloc(sizeof(LinkedList*));
    tmp->data = (void*)malloc(size);
    memcpy(tmp->data, data, size);
    tmp->next = front;
    return tmp;
}

void* searchSymbol(char* name, LinkedList* front) {
    while(front) {
        LSymbol* tmp = (LSymbol*)front->data;
        if(!strcmp(tmp->name, name)) return front->data;
        front = front->next;
    }
    return NULL;
}

LinkedList* copyList(LinkedList* start, size_t dataSize) {
  LinkedList* tmp = NULL;
  while(start) {
    LinkedList* prev = (LinkedList*)malloc(sizeof(LinkedList));
    prev->data = (void*)malloc(dataSize);
    memcpy(prev->data, start->data, dataSize);
    prev->next = tmp;
    tmp = prev;
    start = start->next;
  }
  return tmp;
}

LinkedList* connectList(LinkedList* first, LinkedList *second, size_t size) {
  LinkedList *tmp = first;
  while(first->next) {
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

int getLabel() { return label++; }

reg_index getReg(Frame *frame) {
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

void freeReg(Frame *frame) {
  for (reg_index i = 19; i >= 0; i--)
    if (frame->registers[i] == IN_USE) {
      frame->registers[i] = FREE;
      return;
    }
}

void pushRegToStack(Frame *frame, FILE *out) {
  for (reg_index i = 0; i < 20; i++) {
    if (frame->registers[i] == IN_USE) {
      fprintf(out, "PUSH R%d \\register\n", i);
      frame->registers[i] = RESV;
    }
  }
}

void getRegFromStack(Frame *frame, FILE *out) {
  for (reg_index i = 19; i >= 0; i--) {
    if (frame->registers[i] == RESV) {
      fprintf(out, "POP R%d \\register\n", i);
      frame->registers[i] = IN_USE;
    }
  }
}
