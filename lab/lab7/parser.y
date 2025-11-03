%{
  #include <stdio.h>
  #include "symtable.c"
  #include "three_ac.c"
  #include "main.c"

  extern int yylex();
  extern void yyerror(char* );
%}

%start prog

%union {
  char *str;
  char chr;

  int type_index;
  int sym_index;
  int st_index;

  int intconst;
  double fltconst;
  struct addr* addr;
}

%type <type_index> basic prog declist varlist decl dim _st_push var
%type <sym_index> _fetch_sym
%type <addr> factor term expr smplitem item aref
%type <st_index> stmtlist stmt asgn

%token <intconst> NUM;
%token <fltconst> FLTNUM;
%token <str> LNG INT FLT DBL ID STRUCT
%token <chr> SEMICOLON LSQUARE RSQUARE COMMA DEREF LBRACE RBRACE DOT PLUS MINUS MUL DIV ASG LPN RPN MOD

%% 

prog      : <sym_index>{ $$ = push_st("main"); } 
            declist 
                    { 
                      printf("+++ All declarations read\n\n");
                      print_tt();
                      print_sym();
                      printf("\n\n");
                    }            
            stmtlist                                                        
          ;
// DECLARATION GRAMMAR
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
basic     : INT                                                             { $$ = 0; }
          | LNG                                                             { $$ = 1; }
          | FLT                                                             { $$ = 2; }
          | DBL                                                             { $$ = 3; }
          ;
varlist   : varlist COMMA 
                    <sym_index>{ $$ = $<sym_index>-1; }
                    <type_index>{ $$ = $<type_index>0; } 
            var                      
          | var                                                             {}
          ;
var       : ID 
                    <type_index>{ $$ = $<type_index>0; } 
            dim                                                             { add_var($<sym_index>-1, $1, $3); }
          ;
dim       : LSQUARE NUM RSQUARE 
                    <type_index>{ $$ = $<type_index>0; } 
            dim                                                             { $$ = add_array($2, $5); }
          |                                                                 { $$ = $<type_index>0; }
          ;
// STATEMENT GRAMMAR
stmtlist  : stmtlist stmt
          |
          ;
stmt      : asgn                                                            { printf("\n"); }
          ;
asgn      : <sym_index>{ $$ = 0; }
            item ASG expr SEMICOLON                                         { $$ = store($2, $4); }
          ;
expr      : expr PLUS term                                                  { $$ = binary_op($1, $2, $3); }
          | expr MINUS term                                                 { $$ = binary_op($1, $2, $3); }
          | term                                                            { $$ = $1; }
          ;
term      : term MUL factor                                                 { $$ = binary_op($1, $2, $3); }
          | term DIV factor                                                 { $$ = binary_op($1, $2, $3); }
          | term MOD factor                                                 { $$ = binary_op($1, $2, $3); }
          | factor                                                          { $$ = $1; }
          ;
factor    : NUM                                                             { $$ = push_int($1); }
          | FLTNUM                                                          { $$ = push_flt($1); }
          | <sym_index>{ $$ = 0; } 
            item                                                            { load($2); $$ = $2; }
          | LPN expr RPN                                                    { $$ = $2; }
          ;
item      : smplitem                                                        { $$ = $1; }
          | item DOT 
                    <sym_index>{ $$ = TT.tt[$1->type].reference; }
            smplitem                                                        { $$ = push_struct($3, $1, $4); }
          ;
smplitem  : ID                                                              { $$ = push_id($<sym_index>0, $1); } // TODO: handle st_index
          | aref
          ;
aref      : aref LSQUARE expr RSQUARE                                       { $$ = offset_calc($1, $3); }
          | ID LSQUARE expr RSQUARE                                         { $$ = push_array($<sym_index>0, $1, $3); } // TODO: handle st_index and diff expr types
          ;
// MARKERS
_st_push  :                                                                 { $$ = push_st($<str>-1); }
          ;
_fetch_sym:                                                                 { $$ = TT.tt[$<type_index>0].reference; }


%% 
