all: lex ass2.cpp
	g++ ass2.cpp -o parser

lex: tokenizer.l
	flex tokenizer.l

run: all
	./parser 1.in	

clean:
	rm lex.yy.c parser
