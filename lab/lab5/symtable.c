#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "typetable.c"
#include <stdio.h>
#include <string.h>

struct Node {
  char *id;
  int type_index;
  int offset;
};

struct SymTable {
  int size;
  int curr_width;
  struct Node st[MAX_TABLE_SIZE];
};

static struct SymTable ST = {
    .size = 0,
    .curr_width = 0,
};

int get_var(const char *id) {
  for (int i = 0; i < ST.size; i++) {
    struct Node index = ST.st[i];
    if (strcmp(index.id, id) == 0) {
      return i;
    }
  }
  return -1;
}

int add_var(const char *id, int type) {
  if (get_var(id) == -1) {
    struct Node var;

    var.id = strdup(id);
    var.type_index = type;
    var.offset = ST.curr_width;

    ST.curr_width = ST.curr_width + TT.tt[type].width;
    ST.curr_width = ST.curr_width % 4 == 0
                        ? ST.curr_width
                        : ST.curr_width + 4 - (ST.curr_width % 4);

    ST.st[ST.size++] = var;

    return ST.size - 1;
  } else {
    fprintf(stderr, "\033[91m*** warn: duplicate variable name %s\n\033[0m",
            id);
    return -1;
  }
}

void print_sym() {
  printf("\n+++ Symbol Table\n");
  for (int i = 0; i < ST.size; i++) {
    struct Node index = ST.st[i];
    printf("    %-8s \t %5d - %d \t type = %3d = %s\n", index.id, index.offset,
           TT.tt[index.type_index].width + index.offset - 1, index.type_index,
           TT.tt[index.type_index].category);
  }
  printf("    Total width = %d\n", ST.curr_width);
}

#endif
