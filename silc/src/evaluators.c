#include "evaluators.h"

#include "datastructures.h"
#include "string.h"

int yyerror(char*);

/* TODO: fImplement type checking, array bound checking */

LabelList* llist;
LinkedList* GSymList;

// Evaluates a tree by calling the respective functions according to node type
void eval_tree(tnode* root, Frame* frame, FILE* out) {
  if (root == NULL)
    return;
  eval_tree(root->left, frame, out);
  switch (root->type) {
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
    if (root->vartype == INT)
      fprintf(out, "MOV R%d, %d\n", cur, root->val);
    else if (root->vartype == STRING)
      fprintf(out, "MOV R%d, %d\n", cur, root->varname);
    return cur;
  } else if (root->type == VAR) {
    int binding;
    LSymbol* sym = searchSymbol(root->varname, frame->Lvars);
    if (sym) {  // local variable
      binding = sym->binding;
      reg_index tmp = getReg(frame);
      fprintf(out, "MOV R%d, BP\nADD R%d, %d\n", tmp, tmp, binding);
      fprintf(out, "MOV R%d, [R%d]\n", cur, tmp);
      freeReg(frame);
      return cur;
    } else {
      sym = searchSymbol(root->varname, GSymList);
      if (sym) {  // global variable
        binding = sym->binding;
        reg_index num = getReg(frame);
        fprintf(out, "MOV R%d, %d\n", num, binding);
        if (root->left) {
          fprintf(out, "ADD R%d, R%d\n", num,
                  eval_expr(root->left, frame, out));
          freeReg(frame);
        }
        fprintf(out, "MOV R%d, [R%d]\n", cur, num);
        freeReg(frame);
        return cur;
      } else {
        yyerror("Undefined variable");
      }
    }
  } else if (root->type == FUNC) {
    return call_func(root, frame, out);
  }
  freeReg(frame);

  reg_index left = eval_expr(root->left, frame, out);
  reg_index right = eval_expr(root->right, frame, out);

  if (root->type == OP) {
    if (root->left->vartype != root->right->vartype) {
      yyerror("Type mismatch");
    }
    if ((root->left->vartype == root->right->vartype)  && (root->left->vartype == BOOL)) {
      yyerror("Type mismatch");
    }

    if (root->right->vartype == INT) {
      if (!strcmp(root->varname, "+")) {
        fprintf(out, "ADD R%d, R%d\n", left, right);
      } else if (!strcmp(root->varname, "-")) {
        fprintf(out, "SUB R%d, R%d\n", left, right);
      } else if (!strcmp(root->varname, "*")) {
        fprintf(out, "MUL R%d, R%d\n", left, right);
      } else if (!strcmp(root->varname, "/")) {
        fprintf(out, "DIV R%d, R%d\n", left, right);
      }
    }

    // Compare operators for type string and int
    if (root->right->vartype != BOOL) {
      if (!strcmp(root->varname, "<")) {
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
      }
    }
    freeReg(frame);
  }
  return left;
}

void eval_write(tnode* root, Frame* frame, FILE* out) {
  pushRegToStack(frame, out);
  reg_index tmp = getReg(frame);
  reg_index writereg = eval_expr(root->left, frame, out);
  fprintf(out, "MOV R%d, \"Write\"\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\n", tmp,
          tmp, tmp, tmp);
  fprintf(out, "PUSH R%d\nPUSH R%d\nPUSH R%d\n", writereg, tmp, tmp);
  fprintf(out, "CALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",
          writereg, tmp, tmp, tmp, tmp);
  freeReg(frame);
  freeReg(frame);
  getRegFromStack(frame, out);
}

void eval_read(tnode* root, Frame* frame, FILE* out) {
  pushRegToStack(frame, out);
  int binding;
  reg_index mem;
  LSymbol* sym = searchSymbol(root->left->varname, frame->Lvars);
  if (sym) {  // local variable
    binding = sym->binding;
    reg_index tmp = getReg(frame);
    fprintf(out, "MOV R%d, BP\nADD R%d, %d\n", mem, mem, binding);
  } else {
    sym = searchSymbol(root->left->varname, GSymList);
    if (sym) {  // global variable
      binding = sym->binding;
      fprintf(out, "MOV R%d, %d\n", mem, binding);
      if (root->left)
        fprintf(out, "ADD R%d, R%d\n", mem,
                eval_expr(root->left->left, frame, out));
    }
  }
  reg_index comm = getReg(frame);
  fprintf(out, "MOV R%d, \"Read\"\nPUSH R%d\nMOV R%d, -1\n", comm, comm, comm);
  fprintf(out, "PUSH R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\n", comm, mem, comm,
          comm);
  fprintf(out, "CALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", mem,
          comm, comm, comm, comm);
  freeReg(frame);
  freeReg(frame);
  getRegFromStack(frame, out);
}

void eval_assgn(tnode* root, Frame* frame, FILE* out) {
  LSymbol* sym = (LSymbol*)searchSymbol(root->left->varname, frame->Lvars);
  reg_index binding = getReg(frame);
  if (sym) {
    if (sym->type != root->right->vartype)
      yyerror("Type mismatch");
    fprintf(out, "MOV R%d, BP\nADD R%d, %d\n", binding, binding, sym->binding);
  } else {
    GSymbol* sym = (GSymbol*)searchSymbol(root->left->varname, GSymList);
    if (sym) {
      if (sym->type != root->right->vartype)
        yyerror("Type mismatch");
      fprintf(out, "MOV R%d, %d\n", binding, sym->binding);
      if (root->left->left) {
        fprintf(out, "ADD R%d, R%d\n", binding,
                eval_expr(root->left->left, frame, out));
        freeReg(frame);
      }
    } else {
      yyerror("Variable not found");
    }
  }
  reg_index right = eval_expr(root->right, frame, out);
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
  pushRegToStack(frame, out);
  pushArgToStack(root->left, frame, out);
  reg_index tmp = getReg(frame);
  fprintf(out, "PUSH R%d\n", tmp);
  freeReg(frame);
  fprintf(out, "CALL %s\n", root->varname);
  tmp = getReg(frame);
  fprintf(out, "POP R%d \\return\n", tmp);
  popArgFromStack(root->left, frame, out);
  getRegFromStack(frame, out);
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
    list = list->next;
    fprintf(out, "PUSH R%d \\local variables\n",
            tmp);  // push a free space for the variable
  }
  freeReg(frame);
  addArgSymbol(root->right, frame, -3, out);
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
    fprintf(out, "PUSH R%d\n", call_func(root, frame, out));
    return;
  }
  reg_index value = eval_expr(root, frame, out);
  fprintf(out, "PUSH R%d \\Argument\n", value);
  freeReg(frame);
  return;
}

int addArgSymbol(tnode* root, Frame* frame, int mem, FILE* out) {
  if (root == NULL)
    return mem;
  int left = addArgSymbol(root->left, frame, mem, out);
  LSymbol* tmp = malloc(sizeof(LSymbol));
  *tmp = (LSymbol){
      .name = root->varname, .type = root->vartype, .binding = left--};
  frame->Lvars = addNode(tmp, sizeof(LSymbol), frame->Lvars);
  int right = addArgSymbol(root->right, frame, left, out);
  return right;
}

// pop all the arg from the stack
void popArgFromStack(tnode* root, Frame* frame, FILE* out) {
  if (root == NULL)
    return;
  if (root->type == CONN) {
    pushArgToStack(root->left, frame, out);
    pushArgToStack(root->right, frame, out);
    return;
  }
  reg_index value = getReg(frame);
  fprintf(out, "POP R%d \\Argument\n", value);
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
      fprintf(out, "POP R%d \\local varibales\n", tmp);
    list = list->next;
  }
  fprintf(out, "POP R%d\nMOV BP, R%d\nRET\n", tmp, tmp);
  freeReg(frame);
}
