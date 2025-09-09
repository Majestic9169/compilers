all: lex ass2.cpp
	g++ ass2.cpp -o parser

lex: tokenizer.l
	flex tokenizer.l

run: all
	@echo "[test]: running sample test case 1"
	./parser 1.in	
	@echo "[test]: running sample test case 2"
	./parser ./A2-Guidelines/input.txt

clean:
	rm lex.yy.c parser
