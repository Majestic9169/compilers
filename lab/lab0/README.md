# Assignment 0

Build a simple interpreter for a given language

**Sample Input**: 
```text
S1 = abc
S2 = $S1 ^ 10
T = $S1 . def^2 . $S2
U = x . y^7 . z
V = $U ^ 3 . $W^2 . z^5 . $S1 . defghijklmnopqrstuvw
```

**Expected Output**: 

```text
S1 is set to abc
S2 is set to abcabcabcabcabcabcabcabcabcabc
T is set to abcdefdefabcabcabcabcabcabcabcabcabcabc
U is set to xyyyyyyyz
*** Undefined variable W
V is set to xyyyyyyyzxyyyyyyyzxyyyyyyyzzzzzzabcdefghijklmnopqrstuvw
```

**Demo**
```bash
❯ make
g++ -Wall -Werror -Weffc++ ass0.cpp -o line
./line < 1.in
line> S1 is set to abc
line> S2 is set to abcabcabcabcabcabcabcabcabcabc
line> T is set to abcdefdefabcabcabcabcabcabcabcabcabcabc
line> U is set to xyyyyyyyz
line>
*** Undefined variable W
V is set to xyyyyyyyzxyyyyyyyzxyyyyyyyzzzzzzabcdefghijklmnopqrstuvw
line>
```
