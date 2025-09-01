compile: ./tokenizer.l ./ass3.cpp lex
	g++ -ggdb ass3.cpp -o parser

lex: ./tokenizer.l
	flex tokenizer.l

run: ./tokenizer.l ./ass3.cpp ./parser
	@echo "make: RUNNING TEST 1"
	./parser 1.in	
	@echo "make: RUNNING TEST 2"
	./parser 2.in	

clean:
	-rm lex.yy.c parser
