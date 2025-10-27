%{
#include "parser.tab.h"
%}

%option nounput
%option noinput

wsop							[ \t]*
ws								[ \t]+
wsnl							[ \t\n]+
wsnlop							[ \t\n]*
letter_						    [a-zA-Z_]
letterdig_				        [a-zA-Z0-9_]
nneg_int					    [0-9]+
nl 								"\n"

left_brace                      "{"
right_brace                     "}"
left_parantheses                "["
right_parantheses               "]"
comma                           ","
deref                           "*"
semicolon                       ";"

l_val							{letter_}{letterdig_}*
variable 					    {ref_op}{l_val}
alphabet_string 	            [a-zA-Z]+

void                            "void"
char                            "char"
unsigned                        "unsigned"
short                           "short"
long                            "long"
int                             "int"
float                           "float"
double                          "double"
struct                          "struct"

%% 

<*>{wsnlop} {}

{void} {
    yylval.type_index = 0;
    return VOID;
}

{unsigned}{wsnl}{char} {
    yylval.type_index = 1;
    return UCHR;
}

{char} {
    yylval.type_index = 2;
    return CHR;
}

{unsigned}{wsnl}{short}({wsnl}{int})? {
    yylval.type_index = 3;
    return USRT;
}

{short}({wsnl}{int})? {
    yylval.type_index = 4;
    return SRT;
}

{unsigned}{wsnl}{long}({wsnl}{int})? {
    yylval.type_index = 5;
    return ULNG;
}

{long}({wsnl}{int})? {
    yylval.type_index = 6;
    return LNG;
}

{unsigned}({wsnl}{int})? {
    yylval.type_index = 7;
    return UINT;
}

{int} {
    yylval.type_index = 8;
    return INT;
}

{float} {
    yylval.type_index = 9;
    return FLT;
}

{double} {
    yylval.type_index = 10;
    return DBL;
}

{struct} {
    // TODO: what is the lval?
    return STRUCT;
}

{l_val} {
    yylval.str = strdup(yytext);
    return ID;
}

{comma} {
    yylval.chr = ',';
    return COMMA;
}

{left_parantheses} {
    yylval.chr = '[';
    return LPN;
}

{nneg_int} {
    yylval.num = atoi(yytext);
    return NUM;
}

{right_parantheses} {
    yylval.chr = ']';
    return RPN;
}

{left_brace} {
    yylval.chr = '{';
    return LBRACE;
}

{right_brace} {
    yylval.chr = '}';
    return RBRACE;
}

{semicolon} {
    yylval.chr = ';';
    return SEMICOLON;
}

<*>.|{nl} {
    fprintf(stderr, "*** warning(lexer): invalid character %c\n", yytext[0]);
}

<<EOF>> {
    return EOF;
}

%% 
