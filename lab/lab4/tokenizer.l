%{
#include "y.tab.h"
%}

%x R_VALUE

wsop							[ \t]*
ws								[ \t]+
wsnl							[ \t\n]+
letter_						    [a-zA-Z_]
letterdig_				        [a-zA-Z0-9_]
nneg_int					    [0-9]+
nl 								"\n"

caret_op 					    "^"
concat_op   				    "+"
subtract_op   				    "-"
ref_op                          "$"
assignment_op 		            "="
left_parantheses                "("
right_parantheses               ")"
range_open                      "["
range_close                     "]"
comma                           ","
less_than                       "<"
greater_than                    ">"

l_val							{letter_}{letterdig_}*
variable 					    {ref_op}{l_val}
alphabet_string 	            [a-zA-Z]+

%% 

<*>{wsop} {}

{nl}* {}

exit { 
	printf("exiting\n");
	exit(0);
}

{l_val} {
    yylval.str = strdup(yytext);
    // printf("lex: %s\n", yylval.str);
    return ID;
}

{assignment_op} {
    BEGIN(R_VALUE);
    yylval.chr = strdup(yytext)[0];
    return ASG;
}

<R_VALUE>{variable} {
    yylval.str = strdup(yytext);
    // printf("lex: %s\n", yylval.str);
    return REF;
}

<R_VALUE>{alphabet_string} {
    yylval.str = strdup(yytext);
    // printf("lex: %s\n", yylval.str);
    return STR;
}

<R_VALUE>{caret_op} {
    yylval.chr = strdup(yytext)[0];
    return CRT;
}

<R_VALUE>{nneg_int} {
    yylval.num = atoi(strdup(yytext));
    return NUM;
}

<R_VALUE>{concat_op} {
    yylval.chr = strdup(yytext)[0];
    return ADD;
}

<R_VALUE>{subtract_op} {
    yylval.chr = strdup(yytext)[0];
    return SUB;
}

<R_VALUE>{left_parantheses} {
    yylval.chr = strdup(yytext)[0];
    return LPN;
}

<R_VALUE>{right_parantheses} {
    yylval.chr = strdup(yytext)[0];
    return RPN;
}

<R_VALUE>{range_open} {
    yylval.chr = strdup(yytext)[0];
    return RANGE_OPEN;
}

<R_VALUE>{range_close} {
    yylval.chr = strdup(yytext)[0];
    return RANGE_CLOSE;
}

<R_VALUE>{comma} {
    yylval.chr = strdup(yytext)[0];
    return COMMA;
}

<R_VALUE>{less_than} {
    yylval.chr = strdup(yytext)[0];
    return LT;
}

<R_VALUE>{greater_than} {
    yylval.chr = strdup(yytext)[0];
    return GT;
}

<*>{nl} {
    BEGIN(INITIAL);
    return EOL;
}

<*><<EOF>> {
    return EOF;
}

<*>.|{nl} {
    fprintf(stderr, "*** warning: invalid character %c\n", yytext[0]);
}

%% 
