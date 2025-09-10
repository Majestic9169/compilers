#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX_LEN 999

// string concatenation
char* str_cat(char* arg1, char* arg2) {
  char* res = (char*) malloc(sizeof(char)*(strlen(arg1) + strlen(arg2) + 1));
  strcpy(res, arg1);
  strcat(res, arg2);
  // printf("%s + %s = %s\n", arg1, arg2, res);
  return res;
}

// string subtraction
char* str_sub(char* arg1, char* arg2) {
  if (arg1 == NULL) return NULL;
  char* result = (char*) calloc(strlen(arg1) + 1, sizeof(char));
  int counter = 0;
  int len_a = strlen(arg1);
  while(*arg1) {
    if (*arg1 == *arg2) {
      arg1++;
      arg2++;
    } else {
      if (counter < len_a) {
        result[counter++] = *arg1;
      }
      arg1++;
    }
  }
  // printf("%s - %s = %s\n", arg1, arg2, result);
  return result;
}

// string xpnt
char* str_xpnt(char* arg1, int xpnt) {
  if (arg1 == NULL) {
    return NULL;
  }
  char* result = (char*) malloc((strlen(arg1)*xpnt + 1) * sizeof(char));
  while(xpnt--) {
    strcat(result, arg1);
  }
  // printf("%s ^ %d = %s\n", arg1, xpnt, result);
  return result;
}

// handle_range functions
int* handle_range_m(int m) {
  int* result = (int*) malloc(2*sizeof(int));
  result[0] = m;
  result[1] = m;
  return result;
}

int* handle_range_mn(int m, int n) {
  int* result = (int*) malloc(2*sizeof(int));
  result[0] = m;
  result[1] = n;
  return result;
}

int* handle_range_ltn(int n) {
  int* result = (int*) malloc(2*sizeof(int));
  result[0] = 0;
  result[1] = n-1;
  return result;
}

int* handle_range_gtn(int n) {
  int* result = (int*) malloc(2*sizeof(int));
  result[0] = -n;
  result[1] = -1;
  return result;
}

// range_selector
char* range_selector(char* arg1, int range[2]) {
  char* result = (char*) calloc(strlen(arg1) + 1, sizeof(char));

  range[0] += range[0] < 0 ? strlen(arg1) : 0;
  range[1] += range[1] < 0 ? strlen(arg1) : 0;

  if (range[0] < 0) range[0] = 0;
  if (range[1] > strlen(arg1)) range[1] = strlen(arg1);

  // printf("arg = %s, r0 = %d, r1 = %d\n", arg1, range[0], range[1]);
  strncpy(result, &arg1[range[0]], range[1] - range[0] + 1);
  return result;
}

int test() {
  char* arg1 = "computer";
  char* arg2 = "copter";

  printf("%s\n", str_cat(arg1, arg2));
  printf("%s\n", str_sub(arg1, arg2));
  printf("%s\n", str_xpnt(arg1, 3));

  int* range = handle_range_m(3);
  printf("%s\n", range_selector(arg1, range));
  range = handle_range_mn(1, 7);
  printf("%s\n", range_selector(arg1, range));
  range = handle_range_ltn(9);
  printf("%s\n", range_selector(arg1, range));
  range = handle_range_gtn(6);
  printf("%s\n", range_selector(arg1, range));

  return 0;
}
