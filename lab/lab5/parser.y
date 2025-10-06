%{
  #include <stdio.h>
  #include "main.c"
  #include "symtable.c"

  extern int yylex();
  extern void yyerror(char* );
%}

%start prog

%union {
  char *str;
  char chr;
  int num;
  int type_index;
}

%type <type_index> basic prog declist varlist decl dim

%token <num> NUM
%token <str> VOID UCHR CHR SRT USRT LNG ULNG UINT INT FLT DBL ID
%token <chr> SEMICOLON LPN RPN COMMA DEREF

%% 

prog      : declist
          ;
declist   : declist decl
          | decl
          ;
decl      : basic varlist SEMICOLON
basic     : VOID                                                            { $$ = 0;  }
          | UCHR                                                            { $$ = 1;  }
          | CHR                                                             { $$ = 2;  }
          | USRT                                                            { $$ = 3;  }
          | SRT                                                             { $$ = 4;  }
          | ULNG                                                            { $$ = 5;  }
          | LNG                                                             { $$ = 6;  }
          | UINT                                                            { $$ = 7;  }
          | INT                                                             { $$ = 8;  }
          | FLT                                                             { $$ = 9;  }
          | DBL                                                             { $$ = 10; }
          ;
varlist   : varlist COMMA <type_index>{ $$ = $1; } var 
          | var                                                             { $$ = $<type_index>0; } 
          ;
var       : DEREF <type_index>{ $$ = add_pointer($<type_index>0); } var
          | ID <type_index>{ $$ = $<type_index>0; } dim                     { add_var($1, $3); }
          ;
dim       : LPN NUM RPN <type_index>{ $$ = $<type_index>0; } dim            { $$ = add_array($2, $5); }
          |                                                                 { $$ = $<type_index>0; }
          ;

%% 
