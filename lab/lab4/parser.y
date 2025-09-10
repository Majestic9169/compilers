%{
  #include <stdio.h>
  #include "main.c"

  extern int yylex();
  extern void yyerror(char* );

  typedef struct {
    char* name;
    char* value;
  } stpair;
  stpair ST[4096];
  int nstentry = 0;
  extern int STindex(char*);
  extern char* STload(char*);
  extern void STstore(char* name, char* value);
%}

%start prog

%union {
  char* str;
  char chr;
  int num;
  int *range;
}

%type <num> xpnt
%type <range> range
%type <str> prog line expr term base

%token <num> NUM
%token <str> ID REF STR
%token <chr> EOL ASG ADD SUB RANGE_OPEN RANGE_CLOSE CRT LPN RPN COMMA LT GT

%% 

prog      : line EOL                                {}
          | line EOL prog                           {}
          ;
line      : ID ASG expr                             {STstore($1, $3);}
          ;
expr      : expr ADD term                           {$$ = str_cat($1, $3);}
          | expr SUB term                           {$$ = str_sub($1, $3);}
          | term                                    {$$ = $1;}
          ;
term      : term RANGE_OPEN range RANGE_CLOSE       {$$ = range_selector($1, $3);}
          | term CRT xpnt                           {$$ = str_xpnt($1, $3);}
          | base                                    {$$ = $1;}
          ;
base      : STR                                     {$$ = $1;} 
          | REF                                     {$$ = STload(&$1[1]);}
          | LPN expr RPN                            {$$ = $2;}
          ;
range     : NUM                                     {$$ = handle_range_m($1);}
          | NUM COMMA NUM                           {$$ = handle_range_mn($1, $3);}
          | LT NUM                                  {$$ = handle_range_ltn($2);}
          | GT NUM                                  {$$ = handle_range_gtn($2);}
          ;
xpnt      : NUM                                
          ;

%% 

void yyerror(char* s) { fprintf(stderr, "\033[91m*** %s\n\033[0m", s); }
int yywrap() { return(1); }

int STindex ( char *name ) {
  int i;
  for (i=0; i<nstentry; ++i)
    if (!strcmp(name,ST[i].name)) return i;
  return -1;
}

char* STload ( char *name ) {
  int i;
  char errmsg[STR_MAX_LEN];
  i = STindex(name);
  if (i == -1) {
    sprintf(errmsg, "Undefined variable %s", name);
    yyerror(errmsg);
    return strdup("");
  }
  return ST[i].value;
}

void STstore ( char *name, char* value ) {
  int i;
  i = STindex(name);
  if (i == -1) {
    i = nstentry;
    ST[i].name = strdup(name);
    ++nstentry;
  }
  ST[i].value = strdup(value);
  printf("    Stored %s = %s\n", ST[i].name, ST[i].value);
  // printf("debug: len of val = %d\n", strlen(value));
}


int main(int argc, char** argv) {
  // test();
  yyparse();
  return 0;
}
