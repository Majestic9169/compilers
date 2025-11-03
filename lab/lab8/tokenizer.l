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
nneg_flt                        [0-9]+[.][0-9]*
nl 								"\n"

equal                           "=="
not_equal                       "!="
less_than                       "<"
less_than_equal                 "<="
greater_than                    ">"
greater_than_equal              ">="

logical_or                      "||"
logical_and                     "&&"
logical_not                     "!"

left_brace                      "{"
right_brace                     "}"
left_parantheses                "("
right_parantheses               ")"
left_square                     "["
right_square                    "]"
comma                           ","
mul                             "*"
div                             "/"
plus                            "+"
minus                           "-"
asg                             "="
mod                             "%"
semicolon                       ";"
struct_ref                      "."

l_val							{letter_}{letterdig_}*
variable 					    {ref_op}{l_val}
alphabet_string 	            [a-zA-Z]+

if                              "if"
else                            "else"
while                           "while"

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

{if} {
    yylval.str = strdup(yytext);
    return IF;
}
{else} {
    yylval.str = strdup(yytext);
    return ELSE;
}
{while} {
    yylval.str = strdup(yytext);
    return WHILE;
}
{long}({wsnl}{int})? {
    yylval.type_index = 6;
    return LNG;
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

{equal} {
    yylval.str = strdup(yytext);
    return EQUAL;
}
{not_equal} {
    yylval.str = strdup(yytext);
    return NOT_EQUAL;
}
{less_than} {
    yylval.str = strdup(yytext);
    return LT;
}
{less_than_equal} {
    yylval.str = strdup(yytext);
    return LTE;
}
{greater_than} {
    yylval.str = strdup(yytext);
    return GT;
}
{greater_than_equal} {
    yylval.str = strdup(yytext);
    return GTE;
}

{logical_or} {
    yylval.str = strdup(yytext);
    return LOG_OR;
}
{logical_and} {
    yylval.str = strdup(yytext);
    return LOG_AND;
}
{logical_not} {
    yylval.str = strdup(yytext);
    return LOG_NOT;
}

{asg} {
    yylval.chr = '=';
    return ASG;
}
{plus} {
    yylval.chr = '+';
    return PLUS;
}
{minus} {
    yylval.chr = '-';
    return MINUS;
}
{mul} {
    yylval.chr = '*';
    return MUL;
}
{div} {
    yylval.chr = '/';
    return DIV;
}
{mod} {
    yylval.chr = '%';
    return MOD;
}
{comma} {
    yylval.chr = ',';
    return COMMA;
}
{struct_ref} {
    yylval.chr = '.';
    return DOT;
}

{left_square} {
    yylval.chr = '[';
    return LSQUARE;
}

{right_square} {
    yylval.chr = ']';
    return RSQUARE;
}

{left_parantheses} {
    yylval.chr = ')';
    return LPN;
}
{right_parantheses} {
    yylval.chr = '(';
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

{nneg_flt} {
    yylval.fltconst = (double)atof(yytext);
    return FLTNUM;
}
{nneg_int} {
    yylval.intconst = atoi(yytext);
    return NUM;
}


<*>.|{nl} {
    fprintf(stderr, "*** warning(lexer): invalid character %c\n", yytext[0]);
}

<<EOF>> {
    return EOF;
}

%% 
