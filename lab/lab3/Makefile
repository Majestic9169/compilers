compile: ./tokenizer.l ./ass3.cpp lex
	g++ -ggdb ass3.cpp -o parser

lex: ./tokenizer.l
	flex tokenizer.l

run: ./tokenizer.l ./ass3.cpp ./parser
	@echo "make: RUNNING TEST 1"
	-./parser 1.in	
	@echo "make: RUNNING TEST 2"
	-./parser 2.in	
	@echo "make: RUNNING TEST 3"
	-./parser ./A3-Guidelines/input.txt

clean:
	-rm lex.yy.c parser
