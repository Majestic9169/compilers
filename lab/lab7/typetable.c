#ifndef TYPE_TABLE_H
#define TYPE_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TABLE_SIZE 1024
#define MAX_STR_LEN 200
#define POINTER_SIZE sizeof(void *)

struct TypeNode {
  char *category;
  int dimension;
  int reference;
  int width;
};

struct TypeTable {
  int size;
  struct TypeNode tt[MAX_TABLE_SIZE];
};

static struct TypeTable TT = {
    .size = 0,
};

int get_type(struct TypeNode type) {
  for (int i = 0; i < TT.size; i++) {
    struct TypeNode index = TT.tt[i];
    if (!strcmp(index.category, type.category)) {
      /* printf("debug: found matching type %i %s\n", i, index.category); */
      return i;
    }
  }
  return -1;
}

int add_type(char *category, int dimension, int reference, int width) {

  struct TypeNode var;

  var.category = strdup(category);
  var.dimension = dimension;
  var.reference = reference;
  var.width = width;

  int type_index = get_type(var);
  if (type_index == -1) {
    TT.tt[TT.size] = var;
    TT.size++;
    return TT.size - 1;
  } else {
    return type_index;
  }
}

void init_tt() {
  /* add_type("void", -1, -1, 0); */
  /* add_type("unsigned char", -1, -1, sizeof(unsigned char)); */
  /* add_type("char", -1, -1, sizeof(char)); */
  /* add_type("unsigned short", -1, -1, sizeof(unsigned short)); */
  /* add_type("short", -1, -1, sizeof(short)); */
  /* add_type("unsigned long", -1, -1, sizeof(long)); */
  add_type("int", -1, -1, sizeof(int));
  add_type("long", -1, -1, sizeof(long));
  /* add_type("unsigned int", -1, -1, sizeof(unsigned int)); */
  add_type("float", -1, -1, sizeof(float));
  add_type("double", -1, -1, sizeof(double));
  return;
}

int add_pointer(int reference) {
  char *category = malloc(sizeof(char) * MAX_STR_LEN);
  snprintf(category, MAX_STR_LEN, "pointer(%s)", TT.tt[reference].category);
  return add_type(category, TT.tt[reference].dimension, reference,
                  POINTER_SIZE);
}

int add_array(int dimension, int reference) {
  char *category = malloc(sizeof(char) * MAX_STR_LEN);
  snprintf(category, MAX_STR_LEN, "array(%d, %s)", dimension,
           TT.tt[reference].category);
  return add_type(category, dimension, reference,
                  dimension * TT.tt[reference].width);
}

int add_struct(char *id, int sc_index) {
  char *category = malloc(sizeof(char) * MAX_STR_LEN);
  snprintf(category, MAX_STR_LEN, "struct %s [st = %d]", id, sc_index);
  return add_type(category, -1, sc_index, -69);
}

void print_tt() {
  printf("+++ %d Types\n", TT.size);
  for (int i = 0; i < TT.size; i++) {
    struct TypeNode var = TT.tt[i];
    printf("    Type %3d: \t\t %-8d \t %s\n", i, var.width, var.category);
  }
}

#endif
