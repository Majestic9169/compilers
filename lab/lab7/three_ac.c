#include "symtable.c"
#include "typetable.c"

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
  struct addr *addr = malloc(sizeof(struct addr));
  addr->fltval = val;
  addr->type = 3;
  addr->offset = -1;
  addr->cat = FLTCONST;
  return addr;
}

struct addr *push_id(int st_index, char *id) {
  /* printf("debug: pushing id from st %d\n", st_index); */
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
    printf("    [%s] t%d = (%s2%s)%s\n", short_type(type), temp_counter++,
           short_type(expr->type), short_type(type), print_val(expr));
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
  printf("    [%s] t%d = %s\n", short_type(ref->type), temp_counter++,
         print_memblk(ref));
  ref->cat = TEMP;
  ref->offset = temp_counter - 1;
}

int store(struct addr *lval, struct addr *expr) {
  if (lval->type > 3) {
    fprintf(stderr, "\033[91m*** codegen: invalid type of lval %s\n\033[0m",
            TT.tt[lval->type].category);
    exit(1);
  } else {
    if (lval->type != expr->type) {
      widen(expr, lval->type);
    }
    printf("    [%s] %s = %s\n", short_type(lval->type), print_memblk(lval),
           print_val(expr));
  }
  return 0;
}

struct addr *binary_op(struct addr *op1, char operator, struct addr *op2) {
  if (op1->type != op2->type) {
    handle_conversion(op1, op2);
  }
  printf("    [%s] t%d = t%d %c %s\n", short_type(op1->type), temp_counter++,
         op1->offset, operator, print_val(op2));
  op1->offset = temp_counter - 1;
  return op1;
}

struct addr *push_array(int st_index, char *id, int num) {
  struct addr *addr = malloc(sizeof(struct addr));
  struct Node symbol = SC.st[st_index].st[get_var(st_index, id)];
  addr->offset = symbol.offset;
  printf("    [int] t%d = %d * %d\n", temp_counter++,
         TT.tt[TT.tt[symbol.type_index].reference].width, num);
  printf("    [int] t%d = %d + t%d\n", temp_counter, addr->offset,
         temp_counter - 1);
  addr->type = TT.tt[symbol.type_index].reference;
  addr->offset = temp_counter++;
  addr->cat = TOFFSET;
  return addr;
}

struct addr *offset_calc(struct addr *aref, struct addr *expr) {
  printf("    [int] t%d = %d * %d\n", temp_counter++,
         TT.tt[TT.tt[aref->type].reference].width, expr->intval);
  printf("    [int] t%d = t%d + t%d\n", temp_counter, aref->offset,
         temp_counter - 1);
  aref->type = TT.tt[aref->type].reference;
  aref->offset = temp_counter++;
  aref->cat = TOFFSET;
  return aref;
}

struct addr *push_struct(int st_index, struct addr *struct_parent,
                         struct addr *struct_child) {
  struct addr *addr = malloc(sizeof(struct addr));
  addr->offset = struct_child->offset;
  printf("    [int] t%d = t%d + %s\n", temp_counter, temp_counter - 1,
         print_val(struct_parent));
  addr->type = struct_child->type;
  addr->offset = temp_counter++;
  addr->cat = TOFFSET;
  return addr;
}
