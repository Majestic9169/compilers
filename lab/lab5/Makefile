all:
	bison -d parser.y
	flex tokenizer.l
	gcc -g -pedantic  -Wall -lfl -ly parser.tab.c lex.yy.c -o parser
	./parser decl.c

clean: 
	-@rm *.out *.*.c *.*.h parser 2>/dev/null || true
