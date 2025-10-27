#include "parser.tab.h"
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

  if (argc > 1) {
    yyin = (FILE *)fopen(argv[1], "r");
  }
  yyparse();

  return 0;
}
