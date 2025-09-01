%{
#include <iostream>
#include <string>
using namespace std;

#define ID                      257
#define NUM                     258
#define STR                     259
#define REF                     260
#define LPN                     261
#define RPN                     262
#define CRT                     263
#define DOT                     264
#define ASG                     265
#define EOL                     266
#define END                     267

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
dot_op 						    "."
ref_op                          "$"
assignment_op 		            "="
left_parantheses                "("
right_parantheses                ")"

l_val							{letter_}{letterdig_}*
variable 					    {ref_op}{l_val}
alphabet_string 	            [a-zA-Z]+

%% 

<*>{wsop} {}

{nl} {}

exit { 
	printf("exiting\n");
	exit(0);
}

{l_val} {
    return ID;
}

{assignment_op} {
    BEGIN(R_VALUE);
    return ASG;
}

<R_VALUE>{variable} {
     return REF;
}

<R_VALUE>{alphabet_string} {
     return STR;
}

<R_VALUE>{caret_op} {
    return CRT;
}

<R_VALUE>{nneg_int} {
    return NUM;
}

<R_VALUE>{dot_op} {
    return DOT;
}

<R_VALUE>{left_parantheses} {
    return LPN;
}

<R_VALUE>{right_parantheses} {
    return RPN;
}

<R_VALUE>{nl} {
    BEGIN(INITIAL);
    return EOL;
}

<*><<EOF>> {
    return END;
}

<*>. {
      cerr << "\t\033[31m*** lexer: unrecognized character" + string{yytext} + "\n\033[0m";
}

%% 

int yywrap() {
	return 1;
}
