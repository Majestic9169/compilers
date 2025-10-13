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
  int index;
  char *name;
  int size;
  int curr_width;
  struct Node st[MAX_TABLE_SIZE];
};

struct SymCluster {
  int size;
  struct SymTable st[MAX_TABLE_SIZE];
};

struct SymCluster SC = {
    .size = 0,
};

int push_st(char *name) {
  printf("debug: adding struct %d with name %s\n", SC.size, name);
  int tt_index = 0;

  SC.st[SC.size].index = SC.size;
  SC.st[SC.size].name = strdup(name);
  SC.st[SC.size].size = 0;
  SC.st[SC.size].curr_width = 0;

  if (SC.size != 0) {
    tt_index = add_struct(SC.st[SC.size].name, SC.size);
  }

  SC.size++;
  printf("debug: SC.size = %d\n", SC.size);

  return tt_index;
}

int find_struct(char *name) {
  for (int i = 1; i < SC.size; i++) {
    if (!strcmp(SC.st[i].name, name)) {
      printf("debug: found st %d with name %s\n", i, SC.st[i].name);
      return i;
    }
  }
  fprintf(stderr, "\033[91m*** warn: struct %s not defined\n\033[0m", name);
  exit(1);
  return -1;
}

int get_var(int st_id, const char *id) {
  for (int i = 0; i < SC.st[st_id].size; i++) {
    struct Node index = SC.st[st_id].st[i];
    if (strcmp(index.id, id) == 0) {
      return i;
    }
  }
  return -1;
}

int add_var(int st_id, const char *id, int type) {
  printf("debug: add variable %s with type %d to st %d\n", id, type, st_id);

  if (get_var(st_id, id) == -1) {
    struct Node var;

    var.id = strdup(id);
    var.type_index = type;
    var.offset = SC.st[st_id].curr_width;

    SC.st[st_id].curr_width = SC.st[st_id].curr_width + TT.tt[type].width;
    SC.st[st_id].curr_width =
        SC.st[st_id].curr_width % 4 == 0
            ? SC.st[st_id].curr_width
            : SC.st[st_id].curr_width + 4 - (SC.st[st_id].curr_width % 4);

    SC.st[st_id].st[SC.st[st_id].size] = var;

    return SC.st[st_id].size++;
  } else {
    fprintf(stderr, "\033[91m*** warn: duplicate variable name %s\n\033[0m",
            id);
    return -1;
  }
}

void print_sym() {
  for (int j = 0; j < SC.size; j++) {
    struct SymTable ST = SC.st[j];
    printf("\n+++ Symbol Table %d [%s]\n", ST.index, ST.name);
    for (int i = 0; i < ST.size; i++) {
      struct Node index = ST.st[i];
      printf("    %-8s \t %6d - %-6d \t type = %3d = %s\n", index.id,
             index.offset, TT.tt[index.type_index].width + index.offset - 1,
             index.type_index, TT.tt[index.type_index].category);
    }
    printf("    Total width = %d\n", ST.curr_width);
  }
}

#endif
