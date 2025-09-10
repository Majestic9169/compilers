all:
	yacc -d parser.y
	flex tokenizer.l
	gcc -g y.tab.c lex.yy.c -o calc
	./calc < 1.in

test:
	gcc main.c && ./a.out
	rm a.out

clean: 
	-rm *.out *.*.c *.*.h calc
