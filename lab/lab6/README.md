# Parsing with yacc

```C
char letter, alphabet[26];
unsigned long int a, *p, **q, ***r, A[5], B[6][7], **C[2][3][4];
void *vptr;
struct stud {
  char roll[10];
  char name[101];
  short int yob;
  float CGPA;
  char gender;
} FooBar;
struct stud BTech[200], Dual[100], MTech[150], MS[100], PhD[250];
struct listnode {
  struct coordinates {
    int x, y, z;
  } point;
  struct listnode *next, *prev;
};
struct listnode *head, *tail;
```

```bash
❯ make
bison -d -Wno-midrule-values parser.y
flex tokenizer.l
gcc -g -pedantic  -Wall parser.tab.c lex.yy.c -o parser
./parser decl_1.c
+++ All declarations read

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

+++ Symbol Table 0 [main]
    roll     	      0 - 9      	 type =  11 = array(10, char)
    name     	     12 - 112    	 type =  12 = array(101, char)
    yob      	    116 - 117    	 type =   3 = unsigned short
    CGPA     	    120 - 123    	 type =   9 = float
    gender   	    124 - 124    	 type =   2 = char
    a        	    128 - 135    	 type =   6 = long
    p        	    136 - 143    	 type =  13 = pointer(long)
    q        	    144 - 151    	 type =  14 = pointer(pointer(long))
    r        	    152 - 159    	 type =  15 = pointer(pointer(pointer(long)))
    A        	    160 - 199    	 type =  16 = array(5, long)
    B        	    200 - 535    	 type =  18 = array(6, array(7, long))
    C        	    536 - 727    	 type =  21 = array(2, array(3, array(4, pointer(pointer(long)))))
    points   	    728 - 3127   	 type =  23 = array(100, array(3, double))
    P        	   3128 - 3135   	 type =  24 = pointer(double)
    vptr     	   3136 - 3143   	 type =  25 = pointer(void)
    letters  	   3144 - 3169   	 type =  26 = array(26, unsigned char)
    Total width = 3172
./parser decl_2.c
+++ All declarations read

+++ 32 Types
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
    Type  11: 		 26    	 array(26, char)
    Type  12: 		 8     	 pointer(unsigned long)
    Type  13: 		 8     	 pointer(pointer(unsigned long))
    Type  14: 		 8     	 pointer(pointer(pointer(unsigned long)))
    Type  15: 		 40    	 array(5, unsigned long)
    Type  16: 		 56    	 array(7, unsigned long)
    Type  17: 		 336   	 array(6, array(7, unsigned long))
    Type  18: 		 32    	 array(4, pointer(pointer(unsigned long)))
    Type  19: 		 96    	 array(3, array(4, pointer(pointer(unsigned long))))
    Type  20: 		 192   	 array(2, array(3, array(4, pointer(pointer(unsigned long)))))
    Type  21: 		 8     	 pointer(void)
    Type  22: 		 128   	 struct stud with symbol table 1
    Type  23: 		 10    	 array(10, char)
    Type  24: 		 101   	 array(101, char)
    Type  25: 		 25600 	 array(200, struct stud with symbol table 1)
    Type  26: 		 12800 	 array(100, struct stud with symbol table 1)
    Type  27: 		 19200 	 array(150, struct stud with symbol table 1)
    Type  28: 		 32000 	 array(250, struct stud with symbol table 1)
    Type  29: 		 28    	 struct listnode with symbol table 2
    Type  30: 		 12    	 struct coordinates with symbol table 3
    Type  31: 		 8     	 pointer(struct listnode with symbol table 2)

+++ Symbol Table 0 [main]
    letter   	      0 - 0      	 type =   2 = char
    alphabet 	      4 - 29     	 type =  11 = array(26, char)
    a        	     32 - 39     	 type =   5 = unsigned long
    p        	     40 - 47     	 type =  12 = pointer(unsigned long)
    q        	     48 - 55     	 type =  13 = pointer(pointer(unsigned long))
    r        	     56 - 63     	 type =  14 = pointer(pointer(pointer(unsigned long)))
    A        	     64 - 103    	 type =  15 = array(5, unsigned long)
    B        	    104 - 439    	 type =  17 = array(6, array(7, unsigned long))
    C        	    440 - 631    	 type =  20 = array(2, array(3, array(4, pointer(pointer(unsigned long)))))
    vptr     	    632 - 639    	 type =  21 = pointer(void)
    FooBar   	    640 - 767    	 type =  22 = struct stud with symbol table 1
    BTech    	    768 - 26367  	 type =  25 = array(200, struct stud with symbol table 1)
    Dual     	  26368 - 39167  	 type =  26 = array(100, struct stud with symbol table 1)
    MTech    	  39168 - 58367  	 type =  27 = array(150, struct stud with symbol table 1)
    MS       	  58368 - 71167  	 type =  26 = array(100, struct stud with symbol table 1)
    PhD      	  71168 - 103167 	 type =  28 = array(250, struct stud with symbol table 1)
    head     	 103168 - 103175 	 type =  31 = pointer(struct listnode with symbol table 2)
    tail     	 103176 - 103183 	 type =  31 = pointer(struct listnode with symbol table 2)
    Total width = 103184

+++ Symbol Table 1 [stud]
    roll     	      0 - 9      	 type =  23 = array(10, char)
    name     	     12 - 112    	 type =  24 = array(101, char)
    yob      	    116 - 117    	 type =   4 = short
    CGPA     	    120 - 123    	 type =   9 = float
    gender   	    124 - 124    	 type =   2 = char
    Total width = 128

+++ Symbol Table 2 [listnode]
    point    	      0 - 11     	 type =  30 = struct coordinates with symbol table 3
    next     	     12 - 19     	 type =  31 = pointer(struct listnode with symbol table 2)
    prev     	     20 - 27     	 type =  31 = pointer(struct listnode with symbol table 2)
    Total width = 28

+++ Symbol Table 3 [coordinates]
    x        	      0 - 3      	 type =   8 = int
    y        	      4 - 7      	 type =   8 = int
    z        	      8 - 11     	 type =   8 = int
    Total width = 12
```
