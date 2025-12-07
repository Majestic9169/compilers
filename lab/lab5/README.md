# Using `yacc` for L-attributed grammars

grammars with only synthesized attributes

```C
char roll[10], name[101];
unsigned short int yob;
float CGPA;
char gender;

long int a, *p, **q, ***r, A[5], B[6][7], **C[2][3][4];

double points[100][3], *P;

void *vptr;

unsigned char letters[26];
```

```bash
❯ make
bison -d parser.y
flex tokenizer.l
gcc -g -pedantic  -Wall -lfl -ly parser.tab.c lex.yy.c -o parser
./parser decl.c
+++ 27 Types
    Type   0: 		 0     	 void
    Type   1: 		 1     	 unsigned char
    Type   2: 		 1     	 char
    Type   3: 		 2     	 unsigned short
    Type   4: 		 2     	 short
    Type   5: 		 8     	 unsigned long
    Type   6: 		 8     	 long
    Type   7: 		 4     	 unsigned int
    Type   8: 		 4     	 int
    Type   9: 		 4     	 float
    Type  10: 		 8     	 double
    Type  11: 		 10    	 array(10, char)
    Type  12: 		 101   	 array(101, char)
    Type  13: 		 8     	 pointer(long)
    Type  14: 		 8     	 pointer(pointer(long))
    Type  15: 		 8     	 pointer(pointer(pointer(long)))
    Type  16: 		 40    	 array(5, long)
    Type  17: 		 56    	 array(7, long)
    Type  18: 		 336   	 array(6, array(7, long))
    Type  19: 		 32    	 array(4, pointer(pointer(long)))
    Type  20: 		 96    	 array(3, array(4, pointer(pointer(long))))
    Type  21: 		 192   	 array(2, array(3, array(4, pointer(pointer(long)))))
    Type  22: 		 24    	 array(3, double)
    Type  23: 		 2400  	 array(100, array(3, double))
    Type  24: 		 8     	 pointer(double)
    Type  25: 		 8     	 pointer(void)
    Type  26: 		 26    	 array(26, unsigned char)

+++ Symbol Table
    roll     	     0 - 9 	 type =  11 = array(10, char)
    name     	    12 - 112 	 type =  12 = array(101, char)
    yob      	   116 - 117 	 type =   3 = unsigned short
    CGPA     	   120 - 123 	 type =   9 = float
    gender   	   124 - 124 	 type =   2 = char
    a        	   128 - 135 	 type =   6 = long
    p        	   136 - 143 	 type =  13 = pointer(long)
    q        	   144 - 151 	 type =  14 = pointer(pointer(long))
    r        	   152 - 159 	 type =  15 = pointer(pointer(pointer(long)))
    A        	   160 - 199 	 type =  16 = array(5, long)
    B        	   200 - 535 	 type =  18 = array(6, array(7, long))
    C        	   536 - 727 	 type =  21 = array(2, array(3, array(4, pointer(pointer(long)))))
    points   	   728 - 3127 	 type =  23 = array(100, array(3, double))
    P        	  3128 - 3135 	 type =  24 = pointer(double)
    vptr     	  3136 - 3143 	 type =  25 = pointer(void)
    letters  	  3144 - 3169 	 type =  26 = array(26, unsigned char)
    Total width = 3172
```
