# Using YACC

fml

> [!WARNING] 
> don't forgot to update your lexer

```lex
<R_VALUE>{nneg_int} {
    yylval.num = atoi(strdup(yytext));
    return NUM;
}
```

update your `lex` file with the correct assignments to `yylval` 

> [!WARNING] 
> COPY STRINGS DON'T JUST PASS THE CHARACTER POINTER

```lex
<R_VALUE>{alphabet_string} {
    yylval.str = strdup(yytext);
    return STR;
}
```

PLEASE NEVER FORGET TO USE `strdup()`

## Demo

```text
s01 = computer - copter + empty - emu
s01 = computer - (copter + empty - emu)
s01 = computer - copter - empty + emu
s02 = abc^9 + d^9 - c^9
s03 = (abc^9 + d^9)[6,14] - c^9
s04 = (abc^9 + d^9)[<15] - c^9
s05 = (abc^9 + d^9)[>15] - c^9
s06 = $s02^3 - $s05 - $s05 - $s03^10
s07 = $s1 + ab^5
s08 = (abc^3 + d^5)[3,10] - (c + d^3)^2
s09 = (abc + d)^3[3,10] - (c + d^3)^2
s10 = (abc + d)^3[3,10]^2 - (c + d^3)^2
s11 = ab^4^3-b^2^6
s12 = abcdEFGHijklMNOPqrstUVWXyz[<12][>6]
s12 = abcdEFGHijklMNOPqrstUVWXyz[>12][<6]
```

```bash
❯ make
yacc -d parser.y
flex tokenizer.l
gcc -g y.tab.c lex.yy.c -o calc
./calc < 1.in
    Stored s01 = mupty
    Stored s01 = mue
    Stored s01 = muemu
    Stored s02 = abababababababababddddddddd
    Stored s03 = ababab
    Stored s04 = ababababab
    Stored s05 = ababddddddddd
    Stored s06 = ddddddddd
*** Undefined variable s1
    Stored s07 = ababababab
    Stored s08 = ababc
    Stored s09 = dababc
    Stored s10 = dababcabcab
    Stored s11 = aaaaaaaaaaaa
    Stored s12 = GHijkl
    Stored s12 = OPqrst
```
