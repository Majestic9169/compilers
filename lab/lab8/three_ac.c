#ifndef THREE_AC_H
#define THREE_AC_H

#include "symtable.c"
#include "typetable.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct IR_TABLE {
  int size;
  char *table[MAX_TABLE_SIZE];
  int isLeader[MAX_TABLE_SIZE];
};
struct IR_TABLE IR = {.size = 0};

void init_ir() {
  IR.size = 0;
  for (int i = 0; i < 100; i++) {
    IR.table[i] = malloc(sizeof(char) * MAX_STR_LEN);
    IR.isLeader[i] = 0;
  }
}

void print_ir() {
  for (int i = 0; i < IR.size; i++) {
    printf("   %3d : %s", i, IR.table[i]);
  }
}

int temp_counter = 1;

enum addr_category { INTCONST, FLTCONST, TEMP, OFFSET, TOFFSET };

struct addr {
  union {
    int intval;
    double fltval;
  };
  int offset;
  int type;
  enum addr_category cat;
};

struct list_node {
  struct list_node *next;
  int ir_index;
};

struct stmt {
  struct list_node *nextlist;
};

struct bool_stmt {
  struct list_node *truelist;
  struct list_node *falselist;
};

char *print_val(struct addr *nonterminal) {
  char *val = malloc(sizeof(char) * 16);
  switch (nonterminal->cat) {
  case INTCONST:
    sprintf(val, "%d", nonterminal->intval);
    break;
  case FLTCONST:
    sprintf(val, "%f", nonterminal->fltval);
    break;
  case TEMP:
    sprintf(val, "t%d", nonterminal->offset);
    break;
  case TOFFSET:
    sprintf(val, "%d", nonterminal->offset);
    break;
  default:
    fprintf(stderr,
            "\033[91m*** codegen: unexpected nonterminal type %d\n\033[0m",
            nonterminal->cat);
    exit(1);
    break;
  }
  return val;
}

char *print_memblk(struct addr *nonterminal) {
  char *memblk = malloc(sizeof(char) * 16);
  switch (nonterminal->cat) {
  case INTCONST:
  case FLTCONST:
    sprintf(memblk, "MEM(%d, %d)", nonterminal->offset,
            TT.tt[nonterminal->type].width);
    break;
  case TOFFSET:
    sprintf(memblk, "MEM(t%d, %d)", nonterminal->offset,
            TT.tt[nonterminal->type].width);
    break;
  default:
    break;
  }
  return memblk;
}

struct addr *push_int(int val) {
  struct addr *addr = malloc(sizeof(struct addr));
  addr->intval = val;
  addr->type = 0;
  addr->offset = -1;
  addr->cat = INTCONST;
  return addr;
}

struct addr *push_flt(double val) {
  /* sprintf(IR.table[IR.size++], "debug: float value encountered %f\n", val);
   */
  struct addr *addr = malloc(sizeof(struct addr));
  addr->fltval = val;
  addr->type = 3;
  addr->offset = -1;
  addr->cat = FLTCONST;
  return addr;
}

struct addr *push_id(int st_index, char *id) {
  /* sprintf(IR.table[IR.size++], "debug: pushing id from st %d\n", st_index);
   */
  struct addr *addr = malloc(sizeof(struct addr));
  struct Node symbol = SC.st[st_index].st[get_var(st_index, id)];
  addr->type = symbol.type_index;
  addr->offset = symbol.offset;
  // HACK: everything below is weird
  addr->cat = addr->type <= 1 ? INTCONST : addr->type <= 3 ? FLTCONST : TOFFSET;
  return addr;
}

int max(int type1, int type2) {
  if (type1 == type2)
    return type1;
  if (type1 == 0 || type2 == 0)
    return type1 == 0 ? type2 : type1;
  if (type1 == 1 || type2 == 1)
    return 3;
  if (type1 == 2 || type2 == 2)
    return 3;
  return 3;
}

struct addr *widen(struct addr *expr, int type) {
  if (expr->type != type) {
    sprintf(IR.table[IR.size++], "[%s] t%d = (%s2%s)%s\n", short_type(type),
            temp_counter++, short_type(expr->type), short_type(type),
            print_val(expr));
    expr->type = type;
    expr->cat = TEMP;
    expr->offset = temp_counter - 1;
  }
  return expr;
}

void handle_conversion(struct addr *lval, struct addr *expr) {
  if (lval->type == expr->type) {
    return;
  }
  int larger_type = max(lval->type, expr->type);
  lval = widen(lval, larger_type);
  expr = widen(expr, larger_type);
}

void load(struct addr *ref) {
  sprintf(IR.table[IR.size++], "[%s] t%d = %s\n", short_type(ref->type),
          temp_counter++, print_memblk(ref));
  ref->cat = TEMP;
  ref->offset = temp_counter - 1;
}

int store(struct addr *lval, struct addr *expr) {
  if (lval->type > 3) {
    fprintf(stderr, "\033[91m*** codegen: invalid type of lval %s\n\033[0m",
            TT.tt[lval->type].category);
    exit(1);
  } else {
    widen(expr, lval->type);
    sprintf(IR.table[IR.size++], "[%s] %s = %s\n", short_type(lval->type),
            print_memblk(lval), print_val(expr));
  }
  return 0;
}

struct addr *binary_op(struct addr *op1, char operator, struct addr *op2) {
  handle_conversion(op1, op2);
  sprintf(IR.table[IR.size++], "[%s] t%d = t%d %c %s\n", short_type(op1->type),
          temp_counter++,

          op1->offset, operator, print_val(op2));
  op1->offset = temp_counter - 1;
  return op1;
}

struct addr *push_array(int st_index, char *id, struct addr *num) {
  struct addr *addr = malloc(sizeof(struct addr));
  struct Node symbol = SC.st[st_index].st[get_var(st_index, id)];
  addr->offset = symbol.offset;
  widen(num, 0);
  sprintf(IR.table[IR.size++], "[int] t%d = %d * %s\n", temp_counter++,
          TT.tt[TT.tt[symbol.type_index].reference].width, print_val(num));
  sprintf(IR.table[IR.size++], "[int] t%d = %d + t%d\n", temp_counter,
          addr->offset, temp_counter - 1);
  addr->type = TT.tt[symbol.type_index].reference;
  addr->offset = temp_counter++;
  addr->cat = TOFFSET;
  return addr;
}

struct addr *offset_calc(struct addr *aref, struct addr *expr) {
  widen(expr, 0);
  sprintf(IR.table[IR.size++], "[int] t%d = %d * %s\n", temp_counter++,
          TT.tt[TT.tt[aref->type].reference].width, print_val(expr));
  sprintf(IR.table[IR.size++], "[int] t%d = t%d + t%d\n", temp_counter,
          aref->offset, temp_counter - 1);
  aref->type = TT.tt[aref->type].reference;
  aref->offset = temp_counter++;
  aref->cat = TOFFSET;
  return aref;
}

struct addr *push_struct(int st_index, struct addr *struct_parent,
                         struct addr *struct_child) {
  struct addr *addr = malloc(sizeof(struct addr));
  addr->offset = struct_child->offset;
  sprintf(IR.table[IR.size++], "[int] t%d = t%d + %s\n", temp_counter,
          temp_counter - 1, print_val(struct_parent));
  addr->type = struct_child->type;
  addr->offset = temp_counter++;
  addr->cat = TOFFSET;
  return addr;
}

// CONTROL FLOW

struct list_node *makelist(int i) {
  struct list_node *node = malloc(sizeof(struct list_node));
  printf("debug: making list %p with val %d\n", node, i);
  node->ir_index = i;
  node->next = NULL;
  return node;
}

struct list_node *merge(struct list_node *l1, struct list_node *l2) {
  printf("debug: merging %p with %p\n", l1, l2);
  if (l1) {
    struct list_node *tmp = l1;
    /* printf("merge: traversing list %p\n", l1); */
    while (tmp->next != NULL) {
      /* printf("merge: %d %p\n", tmp->ir_index, tmp); */
      tmp = tmp->next;
    }
    tmp->next = l2;
    return l1;
  } else {
    return l2;
  }
}

void backpatch(struct list_node *p, int ir_index) {
  struct list_node *tmp = p;
  while (tmp != NULL) {
    printf("debug: no segfault here\n");
    printf("debug: backpatching %p statement %s\n", IR.table[tmp->ir_index],
           IR.table[tmp->ir_index]);
    /* char *loc = strstr(IR.table[tmp->ir_index], "---"); */
    /* printf("debug: found replacement string loc %p %s\n", loc, loc); */
    /* if (loc) */
    /*   sprintf(loc, "%d\n", ir_index); */
    tmp = tmp->next;
  }
}

struct stmt *push_stmt(struct list_node *list) {
  struct stmt *stmt = malloc(sizeof(struct stmt));
  stmt->nextlist = list;
  return stmt;
}

struct bool_stmt *push_bool(struct list_node *truelist,
                            struct list_node *falselist) {
  struct bool_stmt *stmt = malloc(sizeof(struct bool_stmt));
  stmt->truelist = truelist;
  stmt->falselist = falselist;
  printf("debug: creating boolstmt with truelist %p falselist %p\n",
         stmt->truelist, stmt->falselist);
  return stmt;
}

void push_goto(int instr) {
  if (instr < 0)
    sprintf(IR.table[IR.size++], "      goto ---");
  else
    sprintf(IR.table[IR.size++], "      goto %d\n", instr);
}

void push_if(struct addr *e1, char *relop, struct addr *e2) {
  handle_conversion(e1, e2);
  sprintf(IR.table[IR.size++], "      if %s %s %s goto ---", print_val(e1),
          relop, print_val(e2));
}

#endif
