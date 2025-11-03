all:
	bison -d -Wno-midrule-values parser.y
	flex tokenizer.l
	gcc -g -pedantic -lm -Wall parser.tab.c lex.yy.c -o parser
	./parser decl_1.c

clean: 
	-@rm *.out *.*.c *.*.h *.output parser 2>/dev/null || true
