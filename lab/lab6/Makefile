all:
	bison -d -Wall --report counterexamples parser.y
	flex tokenizer.l
	gcc -g -pedantic  -Wall parser.tab.c lex.yy.c -o parser
	./parser decl_1.c
	./parser decl_2.c

clean: 
	-@rm *.out *.*.c *.*.h parser 2>/dev/null || true
