# Assignment 3: SLR parser

```bash
❯ make compile
flex tokenizer.l
g++ -ggdb ass3.cpp -o parser

compilers/lab/lab3 on  main [!?] via C v15.2.1-gcc
❯ make run
make: RUNNING TEST 1
./parser 1.in	
debug: entered main
debug: reading from file 1.in
+++ Going to parse next statement
   0 [s2] -> 2 [s5] -> 5 [s11] -> 11 [r8] <- 5 -> 15
     [r6] -> 14
     [r5] <- 15 <- 5 -> 6
     [r3] -> 3
     [r2] <- 6 <- 5 -> 4
     [r1] <- 5 <- 2 <- 0 -> 1
     -> ACCEPT
+++ Stored S1 = a
+++ Going to parse next statement
   0 [s2] -> 2 [s5] -> 5 [s11] -> 11 [r8] <- 5 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 5 -> 6
     [s7] -> 7 [s11] -> 11 [r8] <- 7 -> 15
     [r6] -> 14
     [r5] <- 15 <- 7 -> 6
     [r3] -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 5 -> 4
     [r1] <- 5 <- 2 <- 0 -> 1
     -> ACCEPT
+++ Stored S2 = bbc
+++ Going to parse next statement
   0 [s2] -> 2 [s5] -> 5 [s10] -> 10 [s12] -> 12 [r9] <- 10 -> 15
     [r6] -> 14
     [r5] <- 15 <- 10 -> 6
     [s7] -> 7 [s10] -> 10 [s11] -> 11 [r8] <- 10 -> 15
     [r6] -> 14
     [r5] <- 15 <- 10 -> 6
     [s7] -> 7 [s11] -> 11 [r8] <- 7 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 7 -> 6
     [r3] -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 10 -> 8
     [s9] -> 9 [r10] <- 8 <- 10 <- 7 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 7 -> 6
     [r3] -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 10 -> 8
     [s9] -> 9 [r10] <- 8 <- 10 <- 5 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 5 -> 6
     [r3] -> 3
     [r2] <- 6 <- 5 -> 4
     [r1] <- 5 <- 2 <- 0 -> 1
     -> ACCEPT
+++ Stored S3 = abccccbccccabccccbccccabccccbcccc
+++ Going to parse next statement
   0 [s2] -> 2 [s5] -> 5 [s10] -> 10 [s10] -> 10 [s12] -> 12 [r9] <- 10 -> 15
     [r6] -> 14
     [r5] <- 15 <- 10 -> 6
     [s7] -> 7 [s11] -> 11 [r8] <- 7 -> 15
     [r6] -> 14
     [r5] <- 15 <- 7 -> 6
     [r3] -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 10 -> 8
     [s9] -> 9 [r10] <- 8 <- 10 <- 10 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 10 -> 6
     [s7] -> 7 [s12] -> 12 [r9] <- 7 -> 15
     [r6] -> 14
     [r5] <- 15 <- 7 -> 6
     [s7] -> 7 [s11] -> 11 [r8] <- 7 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 7 -> 6
     [r3] -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 10 -> 8
     [s9] -> 9 [r10] <- 8 <- 10 <- 5 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 5 -> 6
     [s7] -> 7 [s10] -> 10 [s11] -> 11 [r8] <- 10 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 10 -> 6
     [s7] -> 7 [s11] -> 11 [r8] <- 7 -> 15
     [r6] -> 14
     [r5] <- 15 <- 7 -> 6
     [r3] -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 10 -> 8
     [s9] -> 9 [r10] <- 8 <- 10 <- 7 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 7 -> 6
     [r3] -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 5 -> 4
     [r1] <- 5 <- 2 <- 0 -> 1
     -> ACCEPT
+++ Stored S4 = adadadbbcddadadadbbcddadadadbbcddadadadbbcddEFEFGHEFEFGHEFEFGHEFEFGHEFEFGH
make: RUNNING TEST 2
./parser 2.in	
debug: entered main
debug: reading from file 2.in
+++ Going to parse next statement
   0 [s2] -> 2 [s5] -> 5 [s11]
	*** lexer: invalid character _

	*** lexer: invalid character _
           -> 11 [r8] <- 5 -> 15
     [r6] -> 14
     [r5] <- 15 <- 5 -> 6
     [r3] -> 3
     [r2] <- 6 <- 5 -> 4
     [r1] <- 5 <- 2 <- 0 -> 1
     -> ACCEPT
+++ Stored S1 = a
+++ Going to parse next statement
   0 [s2] -> 2 [s5] -> 5 [s11] -> 11 [r8] <- 5 -> 15
     [s16] -> 16 [s17] -> 17 [r7] <- 16 <- 15 -> 14
     [r5] <- 15 <- 5 -> 6
     [s7] -> 7 [s11] -> 11 [r8] <- 7 -> 15
     [r6] -> 14
     [r5] <- 15 <- 7 -> 6
     [s7] -> 7 [s12]
	*** parser: error in undefined variable R1
           -> 12 [r9] <- 7 -> 15
     [r6] -> 14
     [r5] <- 15 <- 7 -> 6
     [r3] -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 7 -> 13
     [r4] <- 7 <- 6 -> 3
     [r2] <- 6 <- 5 -> 4
     [r1] <- 5 <- 2 <- 0 -> 1
     -> ACCEPT
+++ Stored S2 = bbc
+++ Going to parse next statement
   0 [s2] -> 2 [s5] -> 5 [s12]
	*** lexer: invalid character .
           -> 12 [r9] <- 5 -> 15
     [s16]
	*** lexer: invalid character ?
           -> 16
	*** parser: error in s16
```
