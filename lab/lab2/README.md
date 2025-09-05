# Assignment 2

simple LL(1) parser using lex for lexing

```bash
❯ make run
flex tokenizer.l
g++ ass2.cpp -o parser
./parser 1.in	
debug: entered main
debug: reading from file 1.in
+++ parser: set $S1 = a
+++ parser: set $S2 = bbc
+++ parser: set $S3 = abccccbccccabccccbccccabccccbcccc
+++ parser: set $S4 = adadadbbcddadadadbbcddadadadbbcddadadadbbcddEFEFGHEFEFGHEFEFGHEFEFGHEFEFGH
```
