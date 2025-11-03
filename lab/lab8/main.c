#include "parser.tab.h"
#include "three_ac.c"
#include "typetable.c"
#include <stdio.h>

#define STR_MAX_LEN 999

extern FILE *yyin;

void yyerror(char *s) {
  /* print_tt(); */
  /* print_sym(); */
  fprintf(stderr, "\033[91m*** parser: %s\n\033[0m", s);
}

int yywrap() { return (1); }

int main(int argc, char **argv) {

  init_tt();
  init_ir();

  if (argc > 1) {
    yyin = (FILE *)fopen(argv[1], "r");
  }
  yyparse();

  printf("\n\nFinal IR\n");
  print_ir();

  return 0;
}
