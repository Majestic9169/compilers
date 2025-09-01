# Lexing 

> with more complicated grammars

```bash
❯ make
flex ass1.l
g++ lex.yy.c
./a.out 1.in
+++ Line 1 processed: S1 is set to abcdefghijklmnopqrstuvwxyz
+++ Line 2 processed: S2 is set to kkkkkkkkkkkkkkkkkkkk
+++ Line 3 processed: S2 is set to abcdefijkuvwxyzuvwxyzuvwxyz
+++ Line 4 processed: S3 is set to abcdefijkuvwxyzuvwxyzuvwxyz
+++ Line 5 processed: S4 is set to abcdefijkuvwxyzuvwxyzuvwxyz
./a.out 2.in
	*** Invalid character '.' found. Line 1 cannot be processed
	*** Invalid character '=' found. Line 2 cannot be processed
	*** Invalid character '1' found. Line 3 cannot be processed
	*** Invalid lvalue. Line 4 cannot be processed
	*** Invalid character '1' found. Line 5 cannot be processed
	*** Invalid lvalue. Line 6 cannot be processed
	*** Assignment operator missing. Line 7 cannot be processed
	*** No r-value. Line 8 cannot be processed
	*** Reference to undefined variable "$abc"
+++ Line 9 processed: E is set to defdef
	*** Invalid character 'd' found. Line 10 cannot be processed
	*** Invalid character ']' found. Line 11 cannot be processed
	*** Invalid character '-' found. Line 12 cannot be processed
	*** No r-value. Line 13 cannot be processed
	*** Invalid character 'x' found. Line 14 cannot be processed
+++ Line 15 processed: E is set to abcabcyzyzyzyz
	*** No r-value. Line 16 cannot be processed
	*** Invalid exponent specifier. Line 17 cannot be processed
	*** Invalid range selector. Line 18 cannot be processed
```
