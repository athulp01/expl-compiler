#include "../include/evaluators.h"

#include "../include/datastructures.h"
#include "string.h"
#include <stdio.h>

int yyerror(char*);

/* TODO: Implement type checking, array bound checking */

LabelList* llist;
LinkedList* GSymList;


/* Search local and global symbol table and returns the address
*/
reg_index getAddress(tnode *root, Frame *frame, FILE *out) {
    int binding;
    char *dup = strdup(root->varname);
    char *name = strtok(dup, ".");
    Field *field;
    GSymbol* sym = searchSymbol(name, frame->Lvars);
    if (sym) {  // local variable
      binding = sym->binding;
      reg_index mem = getReg(frame);
      reg_index tmp = getReg(frame);
      fprintf(out, "MOV R%d, BP\nADD R%d, %d\n", mem, mem, binding);
      int idx = 0;
      char *tok;
      if(!strcmp(name, "self")) {
        LinkedList *lst = curClassField; 
        while((tok = strtok(NULL, "."))) {
          idx = searchField(tok, lst);
          field = getField(tok, lst);
          if(field) lst = field->type?field->type->fields:field->class->fields;
          fprintf(out,"MOV R%d, [R%d]\nMOV R%d, R%d\nADD R%d, %d\n", tmp, mem,mem, tmp, mem, idx);
        }
      }else {
        LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
        while((tok = strtok(NULL, "."))) {
          idx = searchField(tok, lst);
          field = getField(tok, lst);
          if(field) lst = field->type?field->type->fields:field->class->fields;
          fprintf(out,"MOV R%d, [R%d]\nMOV R%d, R%d\nADD R%d, %d\n", tmp, mem,mem, tmp, mem, idx);
        }
      }
      freeReg(frame);
      return mem;
    } else {
      sym = searchSymbol(name, GSymList);
      if (sym) {  // global variable
        binding = sym->binding;
        reg_index mem = getReg(frame);
        reg_index tmp = getReg(frame);
        int idx = 0;
        char *tok;
        fprintf(out, "MOV R%d, %d\n", mem, binding);
        if (root->type == VAR && root->left) {
          fprintf(out, "ADD R%d, R%d\n", mem,
                  eval_expr(root->left, frame, out));
          freeReg(frame);
        }else {
          LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
          while((tok = strtok(NULL, "."))) {
            idx = searchField(tok, lst);
            field = getField(tok, lst);
            if(field) lst = lst = field->type?field->type->fields:field->class->fields;
            fprintf(out,"MOV R%d, [R%d]\nMOV R%d, R%d\nADD R%d, %d\n", tmp, mem,mem, tmp, mem, idx);
          }
        }
        freeReg(frame);
        return mem;
      } else {
        printf("%s:", root->varname);
        yyerror("Undefined variable");
      }
    }
    return -1;
}

// Evaluates a tree by calling the respective functions according to node type
void eval_tree(tnode* root, Frame* frame, FILE* out) {
  if (root == NULL)
    return;
  if(root->type == CONN) eval_tree(root->left, frame, out);
  switch (root->type) {
    case FFREE:
      eval_free(root ,frame, out);
      break;
    case READ:
      eval_read(root, frame, out);
      break;
    case WRITE:
      eval_write(root, frame, out);
      break;
    case ASSN:
      eval_assgn(root, frame, out);
      return;
    case IF:
      eval_if(root, frame, out);
      return;
    case WHILE:
      eval_while(root, frame, out);
      return;
    case BREAK:
      eval_break(out);
      break;
    case CONT:
      eval_cont(out);
      break;
    case RET:
      eval_return(root, frame, out);
      break;
    case FUNC:
      call_func(root, frame, out);
      freeReg(frame);
      break;
    case METHOD:
      call_method(root, frame, out);
      freeReg(frame);
      break;
  }
  eval_tree(root->right, frame, out);
  return;
}

// Evaluates a expression and returns the register where the result is stord
reg_index eval_expr(tnode* root, Frame* frame, FILE* out) {
  if (root == NULL)
    return 0;
  reg_index cur = getReg(frame);
  if (root->type == CONST) {
    if (!strcmp(root->vartype->name, "int"))
      fprintf(out, "MOV R%d, %d\n", cur, root->val);
    else if (!strcmp(root->vartype->name, "str"))
      fprintf(out, "MOV R%d, %s\n", cur, root->varname);
    else if (!strcmp(root->vartype->name, "null"))
      fprintf(out, "MOV R%d, 0\n", cur);
    return cur;
  } else if (root->type == VAR) {
    fprintf(out, "MOV R%d, [R%d]\n", cur, getAddress(root, frame, out));
    freeReg(frame);
    return cur;
  } else if (root->type == FUNC) {
    return call_func(root, frame, out);
  } else if(root->type == INIT) {
    eval_init(frame, out);
    fprintf(out, "MOV R%d, 0\n", cur);
    return cur;
  } else if(root->type == METHOD) {
    return call_method(root, frame, out);
  }
  freeReg(frame);

  reg_index left = eval_expr(root->left, frame, out);
  reg_index right = eval_expr(root->right, frame, out);

  if (root->type == OP) {
    if (!strcmp(root->varname, "+") ) {
        fprintf(out, "ADD R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, "-") ) {
        fprintf(out, "SUB R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, "*") ) {
        fprintf(out, "MUL R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, "/") ) {
        fprintf(out, "DIV R%d, R%d\n", left, right);
    }else if (!strcmp(root->varname, "<") ) {
        fprintf(out, "LT R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, ">")) {
        fprintf(out, "GT R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, "<=")) {
        fprintf(out, "LE R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, ">=")) {
        fprintf(out, "GE R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, "==")) {
        fprintf(out, "EQ R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, "!=")) {
        fprintf(out, "NE R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, "%")) {
        fprintf(out, "MOD R%d, R%d\n", left, right);
    } else if (!strcmp(root->varname, "AND")) {
      if(!strcmp(root->vartype->name, "bool")) {
        reg_index tmp = getReg(frame);
        fprintf(out, "ADD R%d, R%d\nMOV R%d, 2\nEQ R%d, R%d\n", left, right, tmp, left, tmp);
        freeReg(frame);
      }
    }
    freeReg(frame);
  }
  return left;
}

void eval_write(tnode* root, Frame* frame, FILE* out) {
  int reg = pushRegToStack(frame, out);
  reg_index tmp = getReg(frame);
  reg_index writereg = eval_expr(root->left, frame, out);
  fprintf(out, "MOV R%d, \"Write\"\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\n", tmp,
          tmp, tmp, tmp);
  fprintf(out, "PUSH R%d\nPUSH R%d\nPUSH R%d\n", writereg, tmp, tmp);
  fprintf(out, "CALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",
          writereg, tmp, tmp, tmp, tmp);
  freeReg(frame);
  freeReg(frame);
  getRegFromStack(frame, out, reg);
}

void eval_read(tnode* root, Frame* frame, FILE* out) {
  int reg = pushRegToStack(frame, out);
  int binding;
  reg_index mem = getAddress(root->left, frame, out);
  reg_index comm = getReg(frame);
  fprintf(out, "MOV R%d, \"Read\"\nPUSH R%d\nMOV R%d, -1\n", comm, comm, comm);
  fprintf(out, "PUSH R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\n", comm, mem, comm,
          comm);
  fprintf(out, "CALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", mem,
          comm, comm, comm, comm);
  freeReg(frame);
  freeReg(frame);
  getRegFromStack(frame, out, reg);
}

void eval_assgn(tnode* root, Frame* frame, FILE* out) {
  LSymbol* sym = (LSymbol*)searchSymbol(root->left->varname, frame->Lvars);
  if(!sym) sym = (LSymbol*)searchSymbol(root->left->varname, GSymList);
  reg_index binding = getAddress(root->left, frame, out), right;
  if(root->right->type == ALLOC) {
    right = eval_alloc(4, frame, out);
  } else if(root->right->type == NEW) {
    right = eval_new(root->right, frame, out);
    int idx = searchClass(root->right->varname, ClassList)->idx;
    reg_index vtp = getReg(frame);
    fprintf(out, "MOV R%d, R%d\nADD R%d, 1\nMOV [R%d], %d\n",vtp, binding, vtp, vtp, idx*8+4096);
    freeReg(frame);
  }else right = eval_expr(root->right, frame, out);
  if(root->right->varclass) {
    reg_index vtp = getReg(frame);
    fprintf(out, "MOV R%d, R%d\nADD R%d, 1\nMOV [R%d], R%d\n",vtp, binding, vtp, vtp, root->right->varclass->idx*8+4096);
    freeReg(frame);
  }
  fprintf(out, "MOV [R%d], R%d\n", binding, right);
  freeReg(frame);
  freeReg(frame);
}

void eval_if(tnode* root, Frame* frame, FILE* out) {
  reg_index res = eval_expr(root->left, frame, out);
  if (root->right->right) {
    int start = getLabel(), end = getLabel();
    fprintf(out, "JZ R%d, L%d\n", res, start);
    freeReg(frame);
    eval_tree(root->right->left, frame, out);
    fprintf(out, "JMP L%d\n", end);
    fprintf(out, "L%d:\n", start);
    eval_tree(root->right->right, frame, out);
    fprintf(out, "L%d:\n", end);
  } else {
    int end = getLabel();
    fprintf(out, "JZ R%d, L%d\n", res, end);
    freeReg(frame);
    eval_tree(root->right->left, frame, out);
    fprintf(out, "L%d:\n", end);
  }
}

void eval_while(tnode* root, Frame* frame, FILE* out) {
  int loop_start = getLabel(), loop_end = getLabel();
  llist = createLlistNode(loop_start, loop_end, llist);
  fprintf(out, "L%d:\n", loop_start);
  reg_index res = eval_expr(root->left, frame, out);
  fprintf(out, "JZ R%d, L%d\n", res, loop_end);
  freeReg(frame);
  eval_tree(root->right, frame, out);
  fprintf(out, "JMP L%d\n", loop_start);
  fprintf(out, "L%d:\n", loop_end);
  llist = deleteLlistNode(llist);
}

void eval_break(FILE* out) {
  if (llist) {
    fprintf(out, "JMP L%d\n", llist->end_label);
    deleteLlistNode(llist);
  }
}

void eval_cont(FILE* out) {
  if (llist) {
    fprintf(out, "JMP L%d\n", llist->start_label);
    deleteLlistNode(llist);
  }
}

reg_index call_func(tnode* root, Frame* frame, FILE* out) {
  int reg = pushRegToStack(frame, out);
  pushArgToStack(root->left, frame, out);
  reg_index tmp = getReg(frame);
  fprintf(out, "PUSH R%d\n", tmp);
  freeReg(frame);
  fprintf(out, "CALL %s\n", root->varname);
  tmp = getReg(frame);
  fprintf(out, "POP R%d\n", tmp);
  popArgFromStack(root->left, frame, out);
  //Only pop if pushed
  if(reg) getRegFromStack(frame, out, reg);
  return tmp;
}

reg_index call_method(tnode* root, Frame* frame, FILE* out) {
  char *dup = strdup(root->varname);
  char *s = strtok(dup, ".");
  char *prev;
  GSymbol* sym = (GSymbol*)searchSymbol(s, GSymList);
  if(!sym) sym = (GSymbol*)searchSymbol(s, frame->Lvars);
  LinkedList *lst = sym->class->fields, *plst = NULL;
  Field *field = NULL;
  while((s = strtok(NULL, "."))) {
    prev = strdup(s);
    field = getField(s, lst)?getField(s, lst):field;
    if(field) lst = field->class->fields;
  }
  char *callname = (char*)malloc(sizeof(field?field->class->name:sym->class->name) + 2 + sizeof(s));
  ClassDef* class = searchClass(field?field->class->name:sym->class->name, ClassList);
  int reg = pushRegToStack(frame, out);
  pushArgToStack(root->left, frame, out);
  reg_index self = getAddress(root, frame, out);
  fprintf(out, "PUSH R%d\n", self); //Push self pointer as an argument
  fprintf(out, "MOV R%d, %d\nPUSH R%d\n", self, class->idx*8+4096, self); //Push self pointer as an argument
  freeReg(frame);
  reg_index tmp = getReg(frame);
  fprintf(out, "PUSH R%d\n", tmp);
  freeReg(frame);
  int i;
  for(i=strlen(root->varname)-1; i>=0; i--) {
    if(root->varname[i]=='.') break;
  }
  char *ff = strndup(root->varname, i);
  tnode tde = {.varname=ff};
  reg_index addr = getAddress(&tde, frame, out);
  reg_index vte = getReg(frame);
  fprintf(out, "ADD R%d, 1\nMOV R%d, [R%d]\n", addr, vte, addr);
  fprintf(out, "ADD R%d, %d\n", vte, searchMethod(prev, class->methods)->idx);
  freeReg(frame);
  freeReg(frame);
  fprintf(out, "MOV R%d, [R%d]\n", addr, vte);
  fprintf(out, "CALL R%d\n", addr);
  tmp = getReg(frame);
  fprintf(out, "POP R%d\n", tmp);
  popArgFromStack(root->left, frame, out);
  reg_index val = getReg(frame);
  fprintf(out, "POP R%d\n", val);
  fprintf(out, "POP R%d\n", val);
  freeReg(frame);
  //Only pop if pushed
  if(reg) getRegFromStack(frame, out, reg);
  return tmp;
}

void eval_func(tnode* root, FILE* out) {
  // create a frame for the function
  GSymbol* sym = (GSymbol*)searchSymbol(root->varname, GSymList);
  Frame* frame = (Frame*)malloc(sizeof(Frame));
  frame->Lvars = copyList(sym->frame->Lvars, sizeof(LSymbol));

  /* PUSH the current base pointer and set it as the current stack pointer */
  fprintf(out, "PUSH BP\nMOV BP, SP\n");
  // create a space for local variables in the stack and set the binding
  LinkedList* list = frame->Lvars;
  int count = 1;
  reg_index tmp = getReg(frame);
  while (list) {
    ((LSymbol*)list->data)->binding = count++;
    if(((LSymbol*)list->data)->class) {
      fprintf(out, "PUSH R%d\n",tmp);  // push a free space for the variable
      count++;
    }
    fprintf(out, "PUSH R%d\n",tmp);  // push a free space for the variable
    list = list->next;
  }
  freeReg(frame);
  addArgSymbol(root->right, frame, -3, out);
  eval_tree(root->left, frame, out);
}

void eval_method(tnode* root, LinkedList *methods, FILE* out) {
  // create a frame for the function
  Method* method = searchMethod(root->varname, methods);
  Frame* frame = (Frame*)malloc(sizeof(Frame));
  frame->Lvars = copyList(method->frame->Lvars, sizeof(LSymbol));

  /* PUSH the current base pointer and set it as the current stack pointer */
  fprintf(out, "PUSH BP\nMOV BP, SP\n");
  // create a space for local variables in the stack and set the binding
  LinkedList* list = frame->Lvars;
  int count = 1;
  reg_index tmp = getReg(frame);
  while (list) {
    ((LSymbol*)list->data)->binding = count++;
    if(((LSymbol*)list->data)->class) {
      fprintf(out, "PUSH R%d\n",tmp);  // push a free space for the variable
      count++;
    }
    fprintf(out, "PUSH R%d\n",
            tmp);  // push a free space for the variable
    list = list->next;
  }
  freeReg(frame);
  ClassDef *type = malloc(sizeof(ClassDef));;
  *type = (ClassDef){.fields=curClassField, .methods=curClassMethod};
  LSymbol *var = (LSymbol*)malloc(sizeof(LSymbol));
  *var = (LSymbol){.name="self", .class=searchClass(curClassName, ClassList), .size=1, .binding=-4};
  frame->Lvars = addNode(var, sizeof(LSymbol), frame->Lvars);
  addArgSymbol(root->right, frame, -5, out);
  eval_tree(root->left, frame, out);
}
// Do a inorder traversal and set the binding of the function arguments
void pushArgToStack(tnode* root, Frame* frame, FILE* out) {
  if (root == NULL)
    return;
  if (root->type == CONN) {
    pushArgToStack(root->left, frame, out);
    pushArgToStack(root->right, frame, out);
    return;
  }
  if (root->type == FUNC) {
    fprintf(out, "PUSH R%d \\arg\n", call_func(root, frame, out));
    freeReg(frame);
    return;
  }
  reg_index value = eval_expr(root, frame, out);
  fprintf(out, "PUSH R%d \\arg\n", value);
  freeReg(frame);
  return;
}

int addArgSymbol(tnode* root, Frame* frame, int mem, FILE* out) {
  if (root == NULL)
    return mem;
  int left = addArgSymbol(root->right, frame, mem, out);
  if(root->type != CONN) {
    LSymbol* tmp = malloc(sizeof(LSymbol));
    *tmp = (LSymbol){
        .name = root->varname, .type = root->vartype, .binding = left--};
    frame->Lvars = addNode(tmp, sizeof(LSymbol), frame->Lvars);
  }
  int right = addArgSymbol(root->left, frame, left, out);
  return right;
}

// pop all the arg from the stack
void popArgFromStack(tnode* root, Frame* frame, FILE* out) {
  if (root == NULL)
    return;
  if (root->type == CONN) {
    popArgFromStack(root->left, frame, out);
    popArgFromStack(root->right, frame, out);
    return;
  }
  reg_index value = getReg(frame);
  fprintf(out, "POP R%d \\argu\n", value);
  freeReg(frame);
  return;
}

void eval_return(tnode* root, Frame* frame, FILE* out) {
  reg_index result = eval_expr(root->left, frame, out);
  reg_index tmp = getReg(frame);
  fprintf(out, "MOV R%d, BP\nADD R%d, -2\nMOV [R%d], R%d\n", tmp, tmp, tmp,
          result);
  freeReg(frame);
  LinkedList* list = frame->Lvars;
  while (list) {
    if (((LSymbol*)list->data)->binding >= 0)
      fprintf(out, "POP R%d\n", tmp);
    list = list->next;
  }
  fprintf(out, "POP R%d\nMOV BP, R%d\nRET\n", tmp, tmp);
  freeReg(frame);
}

/* Initialize the heap 
 * */
void eval_init(Frame *frame, FILE *out) {
  int reg = pushRegToStack(frame, out);
  reg_index init = getReg(frame);
  fprintf(out, "MOV R%d, \"Heapset\"\nPUSH R%d\nPUSH R%d\nPUSH R%d\n", init, init, init, init);
  fprintf(out,"PUSH R%d\nPUSH R%d\nCALL 0\n", init, init);
  fprintf(out, "POP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", init, init, init, init, init);
  freeReg(frame);
  getRegFromStack(frame, out, reg);
}

/* Allocate a block of size 8 regardless of the size parameter
 */
reg_index eval_alloc(int size, Frame *frame, FILE *out) {
  int reg = pushRegToStack(frame, out);
  reg_index addr = getReg(frame);
  reg_index init = getReg(frame);
  reg_index size_reg = getReg(frame);
  fprintf(out, "MOV R%d, %d\n", size_reg, size);
  fprintf(out, "MOV R%d, \"Alloc\"\nPUSH R%d\nPUSH R%d\nPUSH R%d\n", init, init, init, init);
  fprintf(out,"PUSH R%d\nPUSH R%d\nCALL 0\n", init, init);
  freeReg(frame);
  freeReg(frame);
  reg_index tmp = getReg(frame);
  fprintf(out, "POP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", addr, tmp, tmp, tmp, tmp);
  freeReg(frame);
  getRegFromStack(frame, out, reg);
  return addr;
}

reg_index eval_new(tnode *root,  Frame *frame, FILE *out) {
  return eval_alloc(4,frame, out);
}

/* Release the block of size 8
 */
void eval_free(tnode *root, Frame *frame, FILE *out) {
  int reg = pushRegToStack(frame, out);
  reg_index mem = getAddress(root->left, frame, out);
  reg_index addr = getReg(frame), tmp = getReg(frame);
  fprintf(out, "MOV R%d, [R%d]\n",addr, mem);
  fprintf(out, "MOV R%d, \"Free\"\nPUSH R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\n", tmp, tmp, addr, addr, addr);
  fprintf(out, "PUSH R%d\nCALL 0\n", addr);
  freeReg(frame);
  freeReg(frame);
  freeReg(frame);
  tmp = getReg(frame);
  fprintf(out, "POP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", addr, tmp, tmp, tmp, tmp);
  freeReg(frame);
  getRegFromStack(frame, out, reg);
}
