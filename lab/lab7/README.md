# Intermediate Code generation


## Input 

```C
long a, b;

float c;

int A[5][10];

struct coll {
  float f;
  double d;
  long x;
  int A[100];
};

struct coll S, T[10][10];

struct bigcoll {
  long n;
  struct coll C[5][5];
} BC[10];

a = 10;
b = a;
c = a + b;

A[1][2] = 2;
A[2][3] = A[1][2] + 5;
A[4][5] = A[1][2] - A[2][3] + c * 123;

T[5][5].x = 100.;
T[5][5].d = T[5][5].x;
S.A[25] = (a + b) * (a - c);
S.x = S.A[25];

BC[5].C[4][3].x = 100;
BC[6].C[5][4].A[50] = BC[5].C[4][3].x;
BC[6].C[5][4].A[80] = BC[5].C[4][3].A[50] + 345.;
BC[A[a / b][c / b]].n =
    A[a - 6][BC[6].C[5][4].A[80] / BC[5].C[4][3].x + 1.35792468];
BC[6].C[5][4] = T[7][7];
```

## Output

```bash
❯ make
bison -d -Wno-midrule-values parser.y
parser.y:38.13-46.20: warning: type clash on default action: <type_index> != <sym_index> [-Wother]
   38 | prog      : <sym_index>{ $$ = push_st("main"); }
      |             ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
parser.y:99.12: warning: empty rule for typed nonterminal, and no action [-Wother]
   99 |           |
      |            ^
flex tokenizer.l
gcc -g -pedantic  -Wall parser.tab.c lex.yy.c -o parser
./parser decl_1.c
+++ All declarations read

+++ 14 Types
    Type   0: 		 4        	 int
    Type   1: 		 8        	 long
    Type   2: 		 4        	 float
    Type   3: 		 8        	 double
    Type   4: 		 40       	 array(10, int)
    Type   5: 		 200      	 array(5, array(10, int))
    Type   6: 		 420      	 struct coll [st = 1]
    Type   7: 		 400      	 array(100, int)
    Type   8: 		 4200     	 array(10, struct coll [st = 1])
    Type   9: 		 42000    	 array(10, array(10, struct coll [st = 1]))
    Type  10: 		 10508    	 struct bigcoll [st = 2]
    Type  11: 		 2100     	 array(5, struct coll [st = 1])
    Type  12: 		 10500    	 array(5, array(5, struct coll [st = 1]))
    Type  13: 		 105080   	 array(10, struct bigcoll [st = 2])

+++ Symbol Table 0 [main]
    a        	      0 - 7      	 type =   1 = long
    b        	      8 - 15     	 type =   1 = long
    c        	     16 - 19     	 type =   2 = float
    A        	     20 - 219    	 type =   5 = array(5, array(10, int))
    S        	    220 - 639    	 type =   6 = struct coll [st = 1]
    T        	    640 - 42639  	 type =   9 = array(10, array(10, struct coll [st = 1]))
    BC       	  42640 - 147719 	 type =  13 = array(10, struct bigcoll [st = 2])
    Total width = 147720

+++ Symbol Table 1 [coll]
    f        	      0 - 3      	 type =   2 = float
    d        	      4 - 11     	 type =   3 = double
    x        	     12 - 19     	 type =   1 = long
    A        	     20 - 419    	 type =   7 = array(100, int)
    Total width = 420

+++ Symbol Table 2 [bigcoll]
    n        	      0 - 7      	 type =   1 = long
    C        	      8 - 10507  	 type =  12 = array(5, array(5, struct coll [st = 1]))
    Total width = 10508


    [lng] t1 = (int2lng)10
    [lng] MEM(0, 8) = t1

    [lng] t2 = MEM(0, 8)
    [lng] MEM(8, 8) = t2

    [lng] t3 = MEM(0, 8)
    [lng] t4 = MEM(8, 8)
    [lng] t5 = t3 + t4
    [flt] t6 = (lng2flt)t5
    [flt] MEM(16, 4) = t6

    [int] t7 = 40 * 1
    [int] t8 = 20 + t7
    [int] t9 = 4 * 2
    [int] t10 = t8 + t9
    [int] MEM(t10, 4) = 2

    [int] t11 = 40 * 2
    [int] t12 = 20 + t11
    [int] t13 = 4 * 3
    [int] t14 = t12 + t13
    [int] t15 = 40 * 1
    [int] t16 = 20 + t15
    [int] t17 = 4 * 2
    [int] t18 = t16 + t17
    [int] t19 = MEM(t18, 4)
    [int] t20 = t19 + 5
    [int] MEM(t14, 4) = t20

    [int] t21 = 40 * 4
    [int] t22 = 20 + t21
    [int] t23 = 4 * 5
    [int] t24 = t22 + t23
    [int] t25 = 40 * 1
    [int] t26 = 20 + t25
    [int] t27 = 4 * 2
    [int] t28 = t26 + t27
    [int] t29 = MEM(t28, 4)
    [int] t30 = 40 * 2
    [int] t31 = 20 + t30
    [int] t32 = 4 * 3
    [int] t33 = t31 + t32
    [int] t34 = MEM(t33, 4)
    [int] t35 = t29 - t34
    [flt] t36 = MEM(16, 4)
    [flt] t37 = (int2flt)123
    [flt] t38 = t36 * t37
    [flt] t39 = (int2flt)t35
    [flt] t40 = t39 + t38
    [int] t41 = (flt2int)t40
    [int] MEM(t24, 4) = t41

    [int] t42 = 4200 * 5
    [int] t43 = 640 + t42
    [int] t44 = 420 * 5
    [int] t45 = t43 + t44
    [int] t46 = t45 + 45
    [lng] t47 = (dbl2lng)100.000000
    [lng] MEM(t46, 8) = t47

    [int] t48 = 4200 * 5
    [int] t49 = 640 + t48
    [int] t50 = 420 * 5
    [int] t51 = t49 + t50
    [int] t52 = t51 + 51
    [int] t53 = 4200 * 5
    [int] t54 = 640 + t53
    [int] t55 = 420 * 5
    [int] t56 = t54 + t55
    [int] t57 = t56 + 56
    [lng] t58 = MEM(t57, 8)
    [dbl] t59 = (lng2dbl)t58
    [dbl] MEM(t52, 8) = t59

    [int] t60 = 4 * 25
    [int] t61 = 20 + t60
    [int] t62 = t61 + 220
    [lng] t63 = MEM(0, 8)
    [lng] t64 = MEM(8, 8)
    [lng] t65 = t63 + t64
    [lng] t66 = MEM(0, 8)
    [flt] t67 = MEM(16, 4)
    [dbl] t68 = (lng2dbl)t66
    [dbl] t69 = (flt2dbl)t67
    [dbl] t70 = t68 - t69
    [dbl] t71 = (lng2dbl)t65
    [dbl] t72 = t71 * t70
    [int] t73 = (dbl2int)t72
    [int] MEM(t62, 4) = t73

    [int] t74 = t73 + 220
    [int] t75 = 4 * 25
    [int] t76 = 20 + t75
    [int] t77 = t76 + 220
    [int] t78 = MEM(t77, 4)
    [lng] t79 = (int2lng)t78
    [lng] MEM(t74, 8) = t79

    [int] t80 = 10508 * 5
    [int] t81 = 42640 + t80
    [int] t82 = 2100 * 4
    [int] t83 = 8 + t82
    [int] t84 = 420 * 3
    [int] t85 = t83 + t84
    [int] t86 = t85 + 81
    [int] t87 = t86 + 86
    [lng] t88 = (int2lng)100
    [lng] MEM(t87, 8) = t88

    [int] t89 = 10508 * 6
    [int] t90 = 42640 + t89
    [int] t91 = 2100 * 5
    [int] t92 = 8 + t91
    [int] t93 = 420 * 4
    [int] t94 = t92 + t93
    [int] t95 = t94 + 90
    [int] t96 = 4 * 50
    [int] t97 = 20 + t96
    [int] t98 = t97 + 95
    [int] t99 = 10508 * 5
    [int] t100 = 42640 + t99
    [int] t101 = 2100 * 4
    [int] t102 = 8 + t101
    [int] t103 = 420 * 3
    [int] t104 = t102 + t103
    [int] t105 = t104 + 100
    [int] t106 = t105 + 105
    [lng] t107 = MEM(t106, 8)
    [int] t108 = (lng2int)t107
    [int] MEM(t98, 4) = t108

    [int] t109 = 10508 * 6
    [int] t110 = 42640 + t109
    [int] t111 = 2100 * 5
    [int] t112 = 8 + t111
    [int] t113 = 420 * 4
    [int] t114 = t112 + t113
    [int] t115 = t114 + 110
    [int] t116 = 4 * 80
    [int] t117 = 20 + t116
    [int] t118 = t117 + 115
    [int] t119 = 10508 * 5
    [int] t120 = 42640 + t119
    [int] t121 = 2100 * 4
    [int] t122 = 8 + t121
    [int] t123 = 420 * 3
    [int] t124 = t122 + t123
    [int] t125 = t124 + 120
    [int] t126 = 4 * 50
    [int] t127 = 20 + t126
    [int] t128 = t127 + 125
    [int] t129 = MEM(t128, 4)
    [dbl] t130 = (int2dbl)t129
    [dbl] t131 = t130 + 345.000000
    [int] t132 = (dbl2int)t131
    [int] MEM(t118, 4) = t132

    [lng] t133 = MEM(0, 8)
    [lng] t134 = MEM(8, 8)
    [lng] t135 = t133 / t134
    [int] t136 = (lng2int)t135
    [int] t137 = 40 * t136
    [int] t138 = 20 + t137
    [flt] t139 = MEM(16, 4)
    [lng] t140 = MEM(8, 8)
    [dbl] t141 = (flt2dbl)t139
    [dbl] t142 = (lng2dbl)t140
    [dbl] t143 = t141 / t142
    [int] t144 = (dbl2int)t143
    [int] t145 = 4 * t144
    [int] t146 = t138 + t145
    [int] t147 = MEM(t146, 4)
    [int] t148 = 10508 * t147
    [int] t149 = 42640 + t148
    [int] t150 = t149 + 149
    [lng] t151 = MEM(0, 8)
    [lng] t152 = (int2lng)6
    [lng] t153 = t151 - t152
    [int] t154 = (lng2int)t153
    [int] t155 = 40 * t154
    [int] t156 = 20 + t155
    [int] t157 = 10508 * 6
    [int] t158 = 42640 + t157
    [int] t159 = 2100 * 5
    [int] t160 = 8 + t159
    [int] t161 = 420 * 4
    [int] t162 = t160 + t161
    [int] t163 = t162 + 158
    [int] t164 = 4 * 80
    [int] t165 = 20 + t164
    [int] t166 = t165 + 163
    [int] t167 = MEM(t166, 4)
    [int] t168 = 10508 * 5
    [int] t169 = 42640 + t168
    [int] t170 = 2100 * 4
    [int] t171 = 8 + t170
    [int] t172 = 420 * 3
    [int] t173 = t171 + t172
    [int] t174 = t173 + 169
    [int] t175 = t174 + 174
    [lng] t176 = MEM(t175, 8)
    [lng] t177 = (int2lng)t167
    [lng] t178 = t177 / t176
    [dbl] t179 = (lng2dbl)t178
    [dbl] t180 = t179 + 1.357925
    [int] t181 = (dbl2int)t180
    [int] t182 = 4 * t181
    [int] t183 = t156 + t182
    [int] t184 = MEM(t183, 4)
    [lng] t185 = (int2lng)t184
    [lng] MEM(t150, 8) = t185

    [int] t186 = 10508 * 6
    [int] t187 = 42640 + t186
    [int] t188 = 2100 * 5
    [int] t189 = 8 + t188
    [int] t190 = 420 * 4
    [int] t191 = t189 + t190
    [int] t192 = t191 + 187
    [int] t193 = 4200 * 7
    [int] t194 = 640 + t193
    [int] t195 = 420 * 7
    [int] t196 = t194 + t195
*** codegen: invalid type of lval struct coll [st = 1]
```
