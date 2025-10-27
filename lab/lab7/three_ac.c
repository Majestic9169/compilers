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

// TODO: handle typecasting
struct addr *add_int(int val) {
  struct addr *addr = malloc(sizeof(struct addr));
  addr->intval = val;
  addr->type = 0;
  addr->offset = -1;
  addr->cat = INTCONST;
  return addr;
}

struct addr *add_flt(double val) {
  struct addr *addr = malloc(sizeof(struct addr));
  addr->fltval = val;
  addr->type = 3;
  addr->offset = -1;
  addr->cat = FLTCONST;
  return addr;
}

struct addr *add_id(int st_index, char *id) {
  struct addr *addr = malloc(sizeof(struct addr));
  struct Node symbol = SC.st[st_index].st[get_var(st_index, id)];
  addr->type = symbol.type_index;
  addr->offset = symbol.offset;
  // HACK: everything below is weird
  addr->cat = addr->type <= 1 ? INTCONST : addr->type <= 3 ? FLTCONST : TOFFSET;
  return addr;
}

void handle_conversion(struct addr *lval, struct addr *expr) {
  if (expr->cat != OFFSET && expr->cat != TOFFSET)
    printf("    [%s] t%d = (%s2%s)", short_type(lval->type), temp_counter++,
           short_type(expr->type), short_type(lval->type));
  switch (expr->cat) {
  case INTCONST:
    printf("%d\n", expr->intval);
    break;
  case FLTCONST:
    printf("%f\n", expr->fltval);
    break;
  case TEMP:
    printf("t%d\n", expr->offset);
    break;
  default:
    fprintf(stderr, "\033[91m*** codegen: unexpected expr type %d\n\033[0m",
            expr->cat);
    exit(1);
    break;
  }
  expr->cat = TEMP;
  expr->offset = temp_counter - 1;
}

void load(struct addr *ref) {
  printf("    [%s] t%d = MEM(%d, %d)\n", short_type(ref->type), temp_counter++,
         ref->offset, TT.tt[ref->type].width);
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
      handle_conversion(lval, expr);
    }
    switch (expr->cat) {
    case TEMP:
      printf("    [%s] MEM(%d, %d) = t%d\n", short_type(lval->type),
             lval->offset, TT.tt[lval->type].width, expr->offset);
      break;
    case INTCONST:
      printf("    [%s] MEM(%d, %d) = %d\n", short_type(lval->type),
             lval->offset, TT.tt[lval->type].width, expr->intval);
      break;
    case FLTCONST:
      printf("    [%s] MEM(%d, %d) = %f\n", short_type(lval->type),
             lval->offset, TT.tt[lval->type].width, expr->fltval);
      break;
    default:
      printf("debug: expr category %d not handled yet\n", expr->cat);
    }
  }
  return 0;
}

struct addr *binary_op(struct addr *op1, char operator, struct addr *op2) {
  // TODO: handle implicit conversions?
  printf("    [%s] t%d = t%d %c t%d\n", short_type(op1->type), temp_counter++,
         op1->offset, operator, op2->offset);
  op1->offset = temp_counter - 1;
  return op1;
}
