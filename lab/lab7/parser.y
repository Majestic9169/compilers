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
  int sym_index;
}

%type <type_index> basic prog declist varlist decl dim _st_push var
%type <sym_index> _fetch_sym

%token <num> NUM
%token <str> VOID UCHR CHR SRT USRT LNG ULNG UINT INT FLT DBL ID STRUCT
%token <chr> SEMICOLON LPN RPN COMMA DEREF LBRACE RBRACE

%% 

prog      : <sym_index>{ $$ = push_st("main"); } 
            declist                                                         { printf("+++ All declarations read\n\n"); }
          ;
declist   : declist 
                    <sym_index>{ $$ = $<sym_index>0; } 
            decl
          | decl
          ;
decl      : basic 
                    <sym_index>{ $$ = $<sym_index>0; }
                    <type_index>{ $$ = $1; }
            varlist SEMICOLON
          | STRUCT ID LBRACE 
                    _st_push 
                    _fetch_sym
            declist RBRACE 
                    { TT.tt[$4].width = SC.st[$5].curr_width; }
                    <sym_index>{ $$ = $<sym_index>0; }
                    <type_index>{ $$ = $4; }
            varlist SEMICOLON                                               {}
          | STRUCT ID LBRACE 
                    _st_push 
                    _fetch_sym
            declist RBRACE 
                    { TT.tt[$4].width = SC.st[$5].curr_width; }
            SEMICOLON                                                       {}
          | STRUCT ID 
                    <sym_index>{ $$ = $<sym_index>0; }
                    <type_index>{ $$ = add_struct($2, find_struct($2)); }
            varlist SEMICOLON                                               {}
          ;
// basic     : VOID                                                            { $$ = 0;  }
//           | UCHR                                                            { $$ = 1;  }
//           | CHR                                                             { $$ = 2;  }
//           | USRT                                                            { $$ = 3;  }
//           | SRT                                                             { $$ = 4;  }
//           | ULNG                                                            { $$ = 5;  }
basic     : LNG                                                             { $$ = 1;  }
          // | UINT                                                            { $$ = 7;  }
          | INT                                                             { $$ = 0;  }
          | FLT                                                             { $$ = 2;  }
          | DBL                                                             { $$ = 3; }
          ;
varlist   : varlist COMMA 
                    <sym_index>{ $$ = $<sym_index>-1; }
                    <type_index>{ $$ = $<type_index>0; } 
            var                      
          | var                                                             {}
          ;
// var       : DEREF 
//                     <sym_index>{ $$ = $<sym_index>-1; }
//                     <type_index>{ $$ = add_pointer($<type_index>0); } 
//             var                                                             {}
var       : ID 
                    <type_index>{ $$ = $<type_index>0; } 
           dim                                                              { add_var($<sym_index>-1, $1, $3); }
          ;
dim       : LPN NUM RPN 
                    <type_index>{ $$ = $<type_index>0; } 
            dim                                                             { $$ = add_array($2, $5); }
          |                                                                 { $$ = $<type_index>0; }
          ;
_st_push  :                                                                 { $$ = push_st($<str>-1); }
          ;
_fetch_sym:                                                                 { $$ = TT.tt[$<type_index>0].reference; }

%% 
