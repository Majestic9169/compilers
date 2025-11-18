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
  int ir_index;

  int intconst;
  double fltconst;
  struct addr* addr;
  struct stmt* stmt;
  struct bool_stmt* bool_stmt;
}

%type <type_index> basic prog declist varlist decl dim _st_push var
%type <sym_index> _fetch_sym
%type <addr> factor term expr smplitem item aref
%type <stmt> stmt stmtlist _push_goto
%type <bool_stmt> bool
%type <st_index> asgn 
%type <ir_index> _ir_index
%type <str> relop

%token <intconst> NUM;
%token <fltconst> FLTNUM;
%token <str> LNG INT FLT DBL ID STRUCT LOG_OR LOG_AND LOG_NOT IF ELSE WHILE EQUAL NOT_EQUAL LT LTE GT GTE
%token <chr> SEMICOLON LSQUARE RSQUARE COMMA DEREF LBRACE RBRACE DOT PLUS MINUS MUL DIV ASG LPN RPN MOD

%left LOG_OR
%left LOG_AND
%right LOG_NOT

%% 

prog      : <sym_index>{ $$ = push_st("main"); } 
            declist 
                    { 
                      printf("+++ All declarations read\n\n");
                      print_tt();
                      print_sym();
                      printf("\n\n");
                    }            
            stmtlist                                                        { 
                                                                              // printf("stmtlist ir:\n"); print_ir(); 
                                                                              // printf("debug: backpatching list %p\n", $4->nextlist);
                                                                              backpatch($4->nextlist, IR.size); 
                                                                            }
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
stmtlist  : stmtlist 
                    _ir_index
            stmt                                                            {
                                                                              backpatch($1->nextlist, $2);
                                                                              $$ = $3;
                                                                            }
          |                                                                 { $$ = push_stmt(NULL); }
          ;
stmt      : asgn                                                            { $$ = push_stmt(NULL); }
          | IF LPN bool RPN LBRACE 
                    _ir_index
            stmtlist RBRACE                                                 {
                                                                              backpatch($3->truelist, $6);
                                                                              $$ = push_stmt(merge($3->falselist, $7->nextlist));
                                                                            }
          | IF LPN bool RPN LBRACE 
                    _ir_index
            stmtlist RBRACE                                                          
                    _push_goto 
            ELSE LBRACE 
                    _ir_index
            stmtlist RBRACE                                                 {
                                                                              backpatch($3->truelist, $6);
                                                                              backpatch($3->falselist, $12);
                                                                              struct list_node* tmp = merge($7->nextlist, $9->nextlist);
                                                                              $$ = push_stmt(merge(tmp, $13->nextlist));
                                                                            }
          | WHILE LPN 
                    _ir_index
            bool RPN LBRACE 
                    _ir_index
            stmtlist RBRACE                                                 {
                                                                              backpatch($8->nextlist, $3);
                                                                              backpatch($4->truelist, $7);
                                                                              $$ = push_stmt($4->falselist);
                                                                              push_goto($3);
                                                                            }
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
          | MINUS term                                                      { $$ = unary_op($2); }
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
smplitem  : ID                                                              { $$ = push_id($<sym_index>0, $1); }
          | aref
          ;
aref      : aref LSQUARE expr RSQUARE                                       { $$ = offset_calc($1, $3); }
          | ID LSQUARE expr RSQUARE                                         { $$ = push_array($<sym_index>0, $1, $3); }
          ;
bool      : bool LOG_OR 
                    _ir_index
            bool                                                            {
                                                                              backpatch($1->falselist, $3);
                                                                              $$ = push_bool(merge($1->truelist, $4->truelist), $4->falselist);
                                                                            }
          | bool LOG_AND 
                    _ir_index
            bool                                                            {
                                                                              backpatch($1->truelist, $3);
                                                                              $$ = push_bool($4->truelist, merge($1->falselist, $4->falselist));
                                                                            }
          | LOG_NOT bool                                                    { $$ = push_bool($2->falselist, $2->truelist); }
          | LPN bool RPN                                                    { $$ = $2; }
          | expr relop expr                                                 {
                                                                              push_if($1, $2, $3);
                                                                              $$ = push_bool(makelist(IR.size - 1), makelist(IR.size));
                                                                              push_goto(-1);
                                                                            }
          ;
relop     : EQUAL
          | NOT_EQUAL
          | LT
          | LTE 
          | GT 
          | GTE
          ;
// MARKERS
_st_push  :                                                                 { $$ = push_st($<str>-1); }
          ;
_fetch_sym:                                                                 { $$ = TT.tt[$<type_index>0].reference; }
          ;
_ir_index :                                                                 { $$ = IR.size; }
          ;
_push_goto:                                                                 { $$ = push_stmt(makelist(IR.size)); push_goto(-1); }
          ;


%% 
